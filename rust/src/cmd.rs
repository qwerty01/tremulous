use std::collections::HashMap;
use std::fmt;
use std::ops::{Deref, DerefMut};
use std::sync::{Arc, RwLock};

pub enum CommandType<'a> {
    ServerCommand,
    ClientCommand {
        callback: Box<dyn Fn() + Send + Sync + 'a>,
        completion_callback: Option<Box<dyn Fn(&str, usize) + Send + Sync + 'a>>,
    },
}
#[allow(dead_code)]
impl<'a> CommandType<'a> {
    pub fn new_client<F, C>(callback: F, completion_callback: Option<C>) -> Self
    where
        F: Fn() + Send + Sync + 'a,
        C: Fn(&str, usize) + Send + Sync + 'a,
    {
        let completion_callback = completion_callback.map(|v| {
            let b: Box<dyn Fn(&str, usize) + Send + Sync + 'a> = Box::new(v);
            b
        });
        Self::ClientCommand {
            callback: Box::new(callback),
            completion_callback,
        }
    }
    pub fn new_server() -> Self {
        Self::ServerCommand
    }
    pub fn set_completion<F: Fn(&str, usize) + Send + Sync + 'a>(
        &mut self,
        callback: F,
    ) -> Option<Box<dyn Fn(&str, usize) + Send + Sync + 'a>> {
        match self {
            Self::ServerCommand => None,
            Self::ClientCommand {
                completion_callback,
                ..
            } => completion_callback.replace(Box::new(callback)),
        }
    }
    pub fn complete(&self, args: &str, arg_num: usize) {
        if let Self::ClientCommand {
            completion_callback: Some(callback),
            ..
        } = self
        {
            (*callback)(args, arg_num);
        }
    }
    pub fn call(&self) -> bool {
        match self {
            Self::ServerCommand => false,
            Self::ClientCommand { callback, .. } => {
                (*callback)();
                true
            }
        }
    }
    pub fn is_client(&self) -> bool {
        matches!(self, Self::ClientCommand { .. })
    }
    pub fn is_server(&self) -> bool {
        matches!(self, Self::ServerCommand)
    }
}
impl<'a> fmt::Debug for CommandType<'a> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::ServerCommand => write!(f, "ServerCommand"),
            Self::ClientCommand { .. } => write!(f, "ClientCommand"),
        }
    }
}

#[derive(Debug, Default)]
pub struct CommandList<'a>(HashMap<String, Arc<RwLock<CommandType<'a>>>>);
impl<'a> CommandList<'a> {
    pub fn new() -> Self {
        Self::default()
    }
}
impl<'a> Deref for CommandList<'a> {
    type Target = HashMap<String, Arc<RwLock<CommandType<'a>>>>;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}
impl<'a> DerefMut for CommandList<'a> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.0
    }
}

mod external {
    use lazy_static::lazy_static;
    use log::error;
    use std::borrow::BorrowMut;
    use std::ffi::{CStr, CString};
    use std::marker::PhantomData;
    use std::mem;
    use std::num::NonZeroU8;
    use std::os::raw::{c_char, c_int};
    use std::sync::{Arc, RwLock};

    use crate::cmd::CommandList;

    use super::CommandType;

    lazy_static! {
        static ref COMMANDS: RwLock<CommandList<'static>> = RwLock::new(CommandList::new());
    }

    #[allow(non_camel_case_types)]
    pub type xcommand_t = extern "C" fn();
    #[allow(non_camel_case_types)]
    pub type completionFunc_t = extern "C" fn(*mut c_char, c_int);

    /// # Safety
    ///
    /// `callback` must be valid for `'a` and be [`Send`] and [`Sync`]
    pub unsafe fn set_completion_raw<'a>(
        command: &mut CommandType<'a>,
        callback: completionFunc_t,
    ) -> Option<Box<dyn Fn(&str, usize) + Send + Sync + 'a>> {
        let callback = RawCompletion::new(callback);
        let callback = move |args: &str, arg_num: usize| {
            let args = CString::from(
                args.as_bytes()
                    .iter()
                    .take_while(|&v| *v != 0)
                    .map(|&v| unsafe { NonZeroU8::new_unchecked(v) })
                    .collect::<Vec<_>>(),
            );
            let mut args = args.into_bytes_with_nul();
            callback.call(args.as_mut_ptr() as _, arg_num as _);
        };
        command.set_completion(callback)
    }

    /// # Safety
    ///
    /// `callback` must be `null` or valid for `'a` and be [`Sync`] and [`Send`]
    /// `completion_callback` must be `null` or valid for `'a` and be [`Sync`] and [`Send`]
    unsafe fn command_type_from_ptr<'a>(
        callback: Option<xcommand_t>,
        completion_callback: Option<completionFunc_t>,
    ) -> CommandType<'a> {
        if let Some(callback) = callback {
            let callback = RawCommand::new(callback);
            let completion_callback = if let Some(completion_callback) = completion_callback {
                let completion_callback = RawCompletion::new(completion_callback);
                let completion_callback: Box<dyn Fn(&str, usize) + Send + Sync + 'a> =
                    Box::new(move |args, arg_num| {
                        let args = CString::from(
                            args.as_bytes()
                                .iter()
                                .take_while(|&v| *v != 0)
                                .map(|&v| unsafe { NonZeroU8::new_unchecked(v) })
                                .collect::<Vec<_>>(),
                        );
                        let mut args = args.into_bytes_with_nul();
                        completion_callback.call(args.as_mut_ptr() as _, arg_num as _);
                    });
                Some(completion_callback)
            } else {
                None
            };
            CommandType::ClientCommand {
                callback: Box::new(move || callback.call()),
                completion_callback,
            }
        } else {
            CommandType::ServerCommand
        }
    }

    struct RawCommand<'a>(xcommand_t, PhantomData<&'a xcommand_t>);
    impl<'a> RawCommand<'a> {
        /// # Safety
        ///
        /// `command` must be valid for `'a` and be [`Sync`] and [`Send`]
        pub unsafe fn new(command: xcommand_t) -> Self {
            Self(command, Default::default())
        }
        pub fn call(&self) {
            (self.0)()
        }
    }
    unsafe impl<'a> Send for RawCommand<'a> {}
    unsafe impl<'a> Sync for RawCommand<'a> {}

    struct RawCompletion<'a>(completionFunc_t, PhantomData<&'a completionFunc_t>);
    impl<'a> RawCompletion<'a> {
        /// # Safety
        ///
        /// `callback` must be valid for `'a` and be [`Sync`] and [`Send`]
        pub unsafe fn new(callback: completionFunc_t) -> Self {
            Self(callback, Default::default())
        }
        /// # Safety
        ///
        /// `args` must be a valid c string
        /// `arg_count` must be the number of arguments in `args`
        pub unsafe fn call(&self, args: *mut c_char, arg_count: c_int) {
            (self.0)(args, arg_count)
        }
    }
    unsafe impl<'a> Send for RawCompletion<'a> {}
    unsafe impl<'a> Sync for RawCompletion<'a> {}

    #[no_mangle]
    #[allow(non_snake_case)]
    /// # Safety
    /// name must be valid
    pub unsafe fn rust_Cmd_CommandExists(name: *const c_char) -> bool {
        if name.is_null() {
            return false;
        }
        let name = CStr::from_ptr(name);
        let name = name.to_string_lossy();

        let commands = COMMANDS.read().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary
        commands.contains_key(name.as_ref())
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// # Safety
    ///
    /// `name` must be valid
    /// `func` must be `null` or valid for `'static`
    pub unsafe fn rust_Cmd_AddCommand(
        name: *const c_char,
        func: Option<xcommand_t>,
        completion_callback: Option<completionFunc_t>,
    ) {
        if name.is_null() {
            return;
        }
        let name = CStr::from_ptr(name);
        let name = name.to_string_lossy();

        let function = command_type_from_ptr(func, completion_callback);

        let mut commands = COMMANDS.write().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary
        commands.insert(name.into_owned(), Arc::new(RwLock::new(function)));
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// # Safety
    ///
    /// `name` must be valid
    pub unsafe fn rust_Cmd_RemoveCommand(name: *const c_char) {
        if name.is_null() {
            return;
        }
        let name = CStr::from_ptr(name);
        let name = name.to_string_lossy();

        let mut commands = COMMANDS.write().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary
        commands.remove(name.as_ref());
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// # Safety
    ///
    /// `name` must be valid
    pub unsafe fn rust_Cmd_RemoveCommandSafe(name: *const c_char) {
        if name.is_null() {
            return;
        }
        let name = CStr::from_ptr(name);
        let name = name.to_string_lossy();

        let mut commands = COMMANDS.write().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary

        if let Some(command) = commands.get(name.as_ref()) {
            if !command.read().unwrap().is_client() {
                // TODO: Unsound, unwinding panics cannot cross FFI boundary
                error!("Restricted source tried to remove system command \"{name}\"");
                return;
            }
        }

        commands.remove(name.as_ref());
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// # Safety
    ///
    /// `command` must be valid
    /// `callback` must be valid for `'static` and be [`Send`] and [`Sync`]
    pub unsafe fn rust_Cmd_SetCommandCompletionFunc(
        command: *const c_char,
        callback: completionFunc_t,
    ) {
        if command.is_null() {
            return;
        }
        let name = CStr::from_ptr(command);
        let name = name.to_string_lossy();

        let mut commands = COMMANDS.write().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary

        if let Some(command) = commands.get_mut(name.as_ref()) {
            // TODO: Unsound, unwinding panics cannot cross FFI boundary
            set_completion_raw(command.write().unwrap().borrow_mut(), callback);
        }
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// Iterates through the command list and calls a callback on each name
    ///
    /// # Safety
    ///
    /// `callback` must be a valid pointer
    pub unsafe fn rust_Cmd_CommandCompletion(callback: fn(*const c_char)) {
        let commands = COMMANDS.read().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary

        for (name, _) in commands.iter() {
            let name = CString::from(
                name.as_bytes()
                    .iter()
                    .take_while(|&b| *b != 0)
                    .map(|&b| unsafe { NonZeroU8::new_unchecked(b) })
                    .collect::<Vec<_>>(),
            );
            (callback)(name.as_ptr());
        }
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// Iterates through the command list and calls a callback on each name
    ///
    /// # Safety
    ///
    /// `callback` must be a valid pointer
    pub unsafe fn rust_Cmd_CommandCompletionSafe(callback: fn(*mut c_char)) {
        let commands = COMMANDS.read().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary

        for (name, _) in commands.iter() {
            let name = CString::from(
                name.as_bytes()
                    .iter()
                    .take_while(|&b| *b != 0)
                    .map(|&b| unsafe { NonZeroU8::new_unchecked(b) })
                    .collect::<Vec<_>>(),
            );
            let mut name = name.into_bytes_with_nul();
            (callback)(name.as_mut_ptr() as _);
        }
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// # Safety
    ///
    /// `command` must be a valid C string
    /// `args` must be a valid C string
    /// `arg_num` must be a valid argument position
    pub unsafe fn rust_Cmd_CompleteArgument(
        name: *const c_char,
        args: *mut c_char,
        arg_num: c_int,
    ) {
        if name.is_null() || args.is_null() {
            return;
        }
        let name = CStr::from_ptr(name);
        let name = name.to_string_lossy();

        let args = CStr::from_ptr(args);
        let args = args.to_string_lossy();

        let commands = COMMANDS.read().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary
        for (cname, callback) in commands.iter() {
            if cname == name.as_ref() {
                callback
                    .read()
                    .unwrap()
                    .complete(args.as_ref(), arg_num as _); // TODO: Unsound, unwinding panics cannot cross FFI boundary
                return;
            }
        }
    }

    #[no_mangle]
    #[allow(non_snake_case)]
    /// # Safety
    ///
    /// `command` must be a valid C string
    pub unsafe fn rust_Cmd_ExecuteCommand(name: *const c_char) -> bool {
        if name.is_null() {
            return false;
        }

        let name = CStr::from_ptr(name);
        let name = name.to_string_lossy();

        let commands = COMMANDS.read().unwrap(); // TODO: Unsound, unwinding panics cannot cross FFI boundary
        let cmd = match commands.get(name.as_ref()) {
            Some(cmd) => cmd.clone(),
            None => return false,
        };
        mem::drop(commands);
        let ret = cmd.read().unwrap().call(); // TODO: Unsound, unwinding panics cannot cross FFI boundary
        ret
    }
}
