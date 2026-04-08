#include <iostream>
#include <thread>
#include "photoReset.h"

//TODO: make a program for each operating system instead of one universal check.
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

int draftmain_WIN(int __argc, char** __argv) {
	bool settingFileValid = false;
    //const char* username = std::getenv("USERNAME");
    //std::filesystem::path config = "C" / "Users" / username / "appdata" / "locallow";
    //for testing purposes, scan for settings.toml inside program directory.
    //in the future, create the file inside a generated folder in %appdata%/locallow
    
    std::filesystem::path test = std::filesystem::current_path();

    for (std::filesystem::directory_iterator it(test); it != std::filesystem::directory_iterator(); ++it) {
        if (std::filesystem::exists(test / "settings.toml")) {
            settingFileValid = true;
            break;
        }
    }

    if (!settingFileValid) {
        //debug
        //std::cerr << "Settings file not found. Creating default settings.toml..." << std::endl;
        std::cout << "First time configuration:" << std::endl;
        photo::photoSettings initPhotos;
        
        settingFileValid = true;
    } 
    if (settingFileValid) {
        //debug
        //std::cout << "Verified settings.toml." << std::endl;
        photo::photoSort newSort;
        
        
    }

    return 0;
}

int draftmain_LINUX(int __argc, char** __argv) {
    
}


//other notes / concepted functions:
int moveWithMutexPrototype() {
    std::filesystem::path safeTest = std::filesystem::path("C:\\Users\\marbl\\Desktop\\photorina test");
    if (std::filesystem::exists("C:\\Users\\marbl\\Desktop\\photorina test\\test1") 
            && std::filesystem::exists("C:\\Users\\marbl\\Desktop\\photorina test\\test2")) {
        std::mutex moveMutex;
        //TODO: get file names inside directory
        //simulate two threads moving files at the same time
        //from test1 to test2
        moveMutex.lock();
        for (const auto& entry : std::filesystem::directory_iterator("C:\\Users\\marbl\\Desktop\\photorina test\\test1")) {
            //std::cout << entry.path() << std::endl;
            
            std::filesystem::rename(entry.path(), safeTest / "test2" / entry.path().filename());
        }
        moveMutex.unlock();

        for (const auto& entry : std::filesystem::directory_iterator("C:\\Users\\marbl\\Desktop\\photorina test\\test2")) {
            std::cout << entry.path() << std::endl;
        }
        return 1;
    } else {
        std::cerr << "Cannot find folders" << std::endl;
        std::filesystem::create_directory("C:\\Users\\marbl\\Desktop\\photorina test\\test1");
        std::filesystem::create_directory("C:\\Users\\marbl\\Desktop\\photorina test\\test2");
        
        std::cout << "Rerun mutex prototype" << std::endl;
        return -1;
    }

    throw std::runtime_error("end of mutex prototype");
}