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

//helper functions
struct ParseCommand {
    std::string flag;
    std::vector<std::string> args;
};

void printHelp(const char* programName) {
    std::cout << "Photorina - Photo Sorting and Organization Tool \n\n";
    std::cout << "Usage: " << programName << " [OPTIONS]\n\n";
    
    std::cout << "Examples:\n";
    std::cout << " " << programName << " -sd \"C:\\Photos\\Sorted\"\n";
}

std::vector<ParseCommand> parseArguments(int argc, char** argv) {
    std::vector<ParseCommand> commands;

    for (int i = 1; i < argc; ) {
        std::string arg = argv[i];
        ParseCommand cmd;
        cmd.flag = arg;

        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            std::exit(0);
        }
        else if (arg == "-sd") {
            if (i + i >= argc) {
                std::cerr << "Error: -sd requires a path argument" << std::endl;
            }
        }
    }
}

int draftmain_WIN(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "Usage:" <<  argv[0] << " [OPTION] <path>" << std::endl;
        std::cerr << argv[0] << ": error: you must provide a command and path to your folder" << std::endl;
        std::cout << "Type " << argv[0] << " -h or " << argv[0] << " --help to see a list of options and usage guide" << std::endl;
        std::cout << "Note: first time run initiates a configuration set up. Please make sure to follow the instructions when prompted" << std::endl;
        return 0;
    }
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
        std::cout << "Warning - First time configuration:" << std::endl;
        photo::photoSettings initPhotos;
        
        for (std::filesystem::directory_iterator it(test); it != std::filesystem::directory_iterator(); ++it) {
            if (!std::filesystem::exists(test / "settings.toml")) {
                throw std::runtime_error("Error: Failed to create settings.toml. Please check your file permissions and try again.");
            }
        }
        settingFileValid = true;
        std::cout << "'settings.toml' created in APPDATA/locallow/photorina" << std::endl;

    } 

    if (settingFileValid) {
        //debug
        //std::cout << "Verified settings.toml." << std::endl;
        photo::photoSort newSort;
               
        
    }

    return 0;
}

int draftmain_LINUX(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "Usage: photorina [OPTION] <path>" << std::endl;
        std::cerr << "photorina.exe: error: you must provide a command and path to your folder" << std::endl;
        std::cout << "Type photorina -h or photorina --help to see a list of options and usage guide" << std::endl;
        return 0;
    }
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
