# Changelog

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