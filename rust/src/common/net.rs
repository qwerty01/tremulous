mod exports {
    use crate::bindings::{
        msg_t, netadr_t, netadrtype_t, netsrc_t, tNET_AdrToString, tNET_AdrToStringwPort,
        tNET_CompareAdr, tNET_CompareBaseAdr, tNET_CompareBaseAdrMask, tNET_Config,
        tNET_GetLoopPacket, tNET_Init, tNET_IsLocalAddress, tNET_JoinMulticast6,
        tNET_LeaveMulticast6, tNET_OutOfBandData, tNET_OutOfBandPrint, tNET_Restart_f,
        tNET_Shutdown, tNET_Sleep, tNET_StringToAdr,
    };

    #[no_mangle]
    pub unsafe extern "C" fn NET_Init() {
        tNET_Init()
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_Shutdown() {
        tNET_Shutdown()
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_Restart_f() {
        tNET_Restart_f()
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_Config(enable: bool) {
        tNET_Config(enable)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_OutOfBandPrint(
        net_socket: netsrc_t,
        adr: netadr_t,
        format: *const i8,
        args: ...
    ) {
        tNET_OutOfBandPrint(net_socket, adr, format, args)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_OutOfBandData(
        sock: netsrc_t,
        adr: netadr_t,
        format: *mut u8,
        len: i32,
    ) {
        tNET_OutOfBandData(sock, adr, format, len)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_CompareAdr(a: netadr_t, b: netadr_t) -> bool {
        tNET_CompareAdr(a, b)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_CompareBaseAdr(a: netadr_t, b: netadr_t) -> bool {
        tNET_CompareBaseAdr(a, b)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_CompareBaseAdrMask(
        a: netadr_t,
        b: netadr_t,
        netmask: i32,
    ) -> bool {
        tNET_CompareBaseAdrMask(a, b, netmask)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_IsLocalAddress(adr: netadr_t) -> bool {
        tNET_IsLocalAddress(adr)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_AdrToString(a: netadr_t) -> *const i8 {
        tNET_AdrToString(a)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_AdrToStringwPort(a: netadr_t) -> *const i8 {
        tNET_AdrToStringwPort(a)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_StringToAdr(
        s: *const i8,
        a: *mut netadr_t,
        family: netadrtype_t,
    ) -> i32 {
        tNET_StringToAdr(s, a, family)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_GetLoopPacket(
        sock: netsrc_t,
        net_from: *mut netadr_t,
        net_message: *mut msg_t,
    ) -> bool {
        tNET_GetLoopPacket(sock, net_from, net_message)
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_JoinMulticast6() {
        tNET_JoinMulticast6()
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_LeaveMulticast6() {
        tNET_LeaveMulticast6()
    }
    #[no_mangle]
    pub unsafe extern "C" fn NET_Sleep(msec: i32) {
        tNET_Sleep(msec)
    }
}
