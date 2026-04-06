#include <iostream>
#include <thread>
#include "photoReset.h"

/* Step by step draft of procedure:
 * 1) on first run, initiate photosettings constructor
 * 2) verify settings.toml is created before proceeding to run command
 * 3) take argv list
 * */

/* CLI command list
 * -b -> moveToDestination, takes 2 string path values. 
 * -a -> autoMove.
 * -m -> selectMove. takes multiple arguments from argv
 * -sd -> setDestination, takes 1 string path.
 * -sc -> setCWD, takes 1 string path.
 * */

int draftmain(int __argc, char** __argv) {
	bool settingFileValid = false;
	int OS_val = 0; //win = 0, linux = 1, mac = 2
			//
	#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		OS_val = 0;
	#elif defined(__linux__) || defined(__LINUX__)
		OS_val = 1;
	#elif defined(__APPLE__) || defined(__MACH__)
		OS_val = 2;
	#else
		OS_val = -1;
	#endif
	
	//in the future, set to check for file in appdata config folder.
	switch (OS_val) 
	case 0: 
	std::filesystem::path config = "C" / "Users" / username / "appdata" / roaming;
	if (!std::filesystem::exists("")) {
		std::cout << "windows confirmed" << std::endl;
		const char* username = std::getenv("USERNAME");

		if (username != nullptr) {
			std::filesystem::path config = "C" / "Users" / username / "appdata" / roaming;
			
		}
	}

	case 1:
	if (!std::filesystem::exists("")) {

	}

	case 2:
	if (!std::filesystem::exists("")) {

	}
	photo::photoSettings initPhotos;

}
