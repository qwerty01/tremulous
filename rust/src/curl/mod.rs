mod exports {
    use std::os::raw::c_char;

    #[no_mangle]
    extern "C" fn CL_cURL_Init() -> bool {
        true
    }
    #[no_mangle]
    extern "C" fn CL_cURL_Shutdown() {}
    #[no_mangle]
    extern "C" fn CL_cURL_BeginDownload(_local_name: *const c_char, _remote_url: *const c_char) {}
    #[no_mangle]
    extern "C" fn CL_cURL_PerformDownload() {}
    #[no_mangle]
    extern "C" fn CL_cURL_Cleanup() {}
}
