use crate::bindings::cvar_s;
use std::collections::HashMap;
use std::ffi::CString;

#[repr(transparent)]
struct CvarWrapper(cvar_s);
impl CvarWrapper {
    pub(crate) fn new(cvar: cvar_s) -> Self {
        Self(cvar)
    }
    pub fn get_cvar(&self) -> *const cvar_s {
        &self.0 as _
    }
    pub fn get_cvar_mut(&mut self) -> *mut cvar_s {
        &mut self.0 as _
    }
}
impl Drop for CvarWrapper {
    fn drop(&mut self) {
        if !self.0.name.is_null() {
            let _ = unsafe { CString::from_raw(self.0.name) };
        }
        if !self.0.string.is_null() {
            let _ = unsafe { CString::from_raw(self.0.string) };
        }
        if !self.0.resetString.is_null() {
            let _ = unsafe { CString::from_raw(self.0.resetString) };
        }
        if !self.0.latchedString.is_null() {
            let _ = unsafe { CString::from_raw(self.0.latchedString) };
        }
        if !self.0.description.is_null() {
            let _ = unsafe { CString::from_raw(self.0.description) };
        }

        let next = self.0.next;
        let prev = self.0.prev;
        if let Some(next) = unsafe { next.as_mut() } {
            next.prev = prev;
        }
        if let Some(prev) = unsafe { prev.as_mut() } {
            prev.next = next;
        }

        let next_hash = self.0.hashNext;
        let prev_hash = self.0.hashPrev;
        if let Some(next) = unsafe { next_hash.as_mut() } {
            next.hashPrev = prev_hash;
        }
        if let Some(prev) = unsafe { prev_hash.as_mut() } {
            prev.hashNext = next_hash;
        }
    }
}

pub struct CvarValue<'a, T, E> {
    value: T,
    default: T,
    validate: Box<dyn Fn(&T) -> Result<(), E> + 'a>,
    cvar: CvarWrapper,
}
impl<'a, E> CvarValue<'a, i64, E> {
    fn new<F>(value: i64, default: i64, validate: F, cvar: CvarWrapper) -> Self
    where
        F: Fn(&i64) -> Result<(), E> + 'a,
    {
        Self {
            value,
            default,
            validate: Box::new(validate),
            cvar,
        }
    }
}

pub enum CvarVariant<'a, E> {
    Integer(CvarValue<'a, i64, E>),
    Float(CvarValue<'a, f32, E>),
    Boolean(CvarValue<'a, bool, E>),
}

pub struct Cvar<'a, E> {
    table: HashMap<&'a str, CvarVariant<'a, E>>,
    cvar_head: *mut cvar_s,
}
