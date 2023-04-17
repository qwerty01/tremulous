use crate::bindings::{clc, clientConnection_t};

pub fn get_clc() -> &'static mut clientConnection_t {
    unsafe { &mut clc }
}
