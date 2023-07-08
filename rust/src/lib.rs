#![feature(c_variadic)]
use std::os::raw::c_int;

//use log::error;

pub mod client;
pub mod cmd;
pub mod common;
pub mod console;
pub mod curl;
pub mod logging;

mod bindings;

#[no_mangle]
pub extern "C" fn FS_LoadStack() -> c_int {
    unsafe { bindings::FS_LoadStack2() }
}

//#[no_mangle]
//extern "C" fn rust_main(argc: c_int, argv: *mut *mut c_char) -> c_int {
//    match init(argc, argv) {
//        Ok(v) => v,
//        Err(e) => {
//            error!("{}", e);
//            -1
//        }
//    }
//}
//
//fn init(argc: c_int, argv: *mut *mut c_char) -> anyhow::Result<c_int> {
//    pretty_env_logger::init();
//    crossterm::terminal::enable_raw_mode()?;
//    //Ok(unsafe { bindings::main2(argc, argv) })
//}
//
