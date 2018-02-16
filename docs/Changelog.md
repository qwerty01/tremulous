# Changelog

2018-02-16 Dusan Joci <dusanjocic@msn.com>
	* Fixed compiling with cMake on the Windows platform
	* Forgot to upload some files

2018-02-15 Dusan Joci <dusanjocic@msn.com>
	* Added Changelog.md, TODO.md and 
	* Changed folder structure
	* Removed old Visual Studio files
	* Updated cMake system
	* More work for the cMake system on the Windows platform
	* Fixed compiling "script_api" on the Windows platform
	* Fixed compiling most of the code on the Windows platform with Visual Studio
	* Fixed compiling engine and gamelogic on the Windows platform with Visual Studio
		NOTE : update code is turned off for now, some parts of VM code has been removed (whole VM will be gone with the wind in near future)
	* "void Sys_PlatformInit( void )" was removed by mistake from the sys_unix.cpp
	* Added ".def" files what are needed for the Windows builds
		NOTE: cMake "add_custom_command" after compiling is not working, GLSL shaders for the "renderer2" are not linked in the generated Visual Studio solution
	* Fixed copy-paste mistake