#include <iostream>
#include <iomanip>
#include <thread>
#include "photoReset.h"

using namespace std::string_view_literals;

//Notes:
//In the future, move toml to Appdata or ~/.config for better user experience and to avoid issues with permissions in CWD.

/* int main(int __argc, char** __argv) {

    photo::photoSettings newSort;

    toml::table settingsTable = toml::parse_file("settings.toml");

    static constexpr std::string_view cameraTest = R"(      
        CameraBrand = 'Fujifilm'
        CameraModel = 'X-T30 III'
        Formats = [ '.RAF', '.JPEG' ]
    )"sv; //keep for insert_or_assign tests
    
    toml::table newTable = toml::parse(cameraTest);
    std::string key, value;

    key = cameraTest.substr(0, cameraTest.find('=') - 1);
    value = cameraTest.substr(cameraTest.find('=') + 2, cameraTest.find('\n') - cameraTest.find('=') - 3);
 
    //----------------------------------------------------------------------------//

    photo::photoSort newSort2;

    std::filesystem::path testUnit = newSort2.setCWD("C:\\Users\\marbl\\Desktop\\photorina test\\test1");
    std::filesystem::path unitDestination = newSort2.setDestination("C:\\Users\\marbl\\Desktop\\photorina test\\test2");
       

    //newSort2.moveToDestination("C:\\Users\\marbl\\Desktop\\photorina test\\test1", "C:\\Users\\marbl\\Desktop\\photorina test\\test2");

    newSort2.autoMove();
    return 0;
} 
make run ARGS=    
*/
struct inputStorage {
    std::vector<int> additionals;
    std::vector<std::string> parsedArgv;
};


void help(const char* programName) {
    std::cout << "Photorina - Photo Sorting and Organization Utility\n\n";
    std::cout << "Usage: " << programName << " [OPTIONS]\n\n";

    std::cout << "On first run, the program automatically detects and sets the \n";
    std::cout << "main designated photos folder as the 'destination directory.'\n";

    std::cout << "Set your designated destination folder by using '-sd' followed by the path in the command line\n";
    std::cout << "(Right click on a folder on your File Explorer and click 'Copy as Path' to get the full path name)\n";

    std::cout << "Options:\n";
    std::cout << "  -h, --help              Display this help message\n";
    std::cout << "  -c                      Activate 'Add Camera' utility command\n";
    std::cout << "  -lc                     Lists cameras that are added\n";
    std::cout << "  -dc <camera name>       Delete camera listed in settings\n";
    std::cout << "  -sd <path>              Set destination directory for photo sorting\n";
    std::cout << "  -sc <path>              Set source directory that contains current photos\n";
    std::cout << "  -b <source> <dest>      Move all files from source to destination\n";
    std::cout << "  -a                      Automatically move files from your settings\n";
    std::cout << "  -setting                Print out the current settings\n";
}

//command line interface parsing basic algorithm
//Receive commands stored in argv array, and process them
//Store string to int into struct array and process on main function (make appropriate command calls)
int CLI_Parse(int argc, char** argv, inputStorage& storage) {
    //debug
    std::cout << argc << std::endl;
    //if no argument, display a usage line, error line and "Type photorina -h to see a list of options"
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [OPTION] <path>" << std::endl;
        std::cerr << argv[0] << ": error: you must provide a command and path to your folder" << std::endl;
        std::cout << "Type " << argv[0] << " -h or " << argv[0] << " --help to see a list of options and the usage guide" << std::endl;
        std::cout << "Note: first time run initiates a configuration set up. Please make sure to follow the instructions when prompted" << std::endl;
        return -1;
    }

    if (argv[1] == std::string("-h") || argv[1] == std::string("--help")) {
        help(argv[0]);
        return 0;
    }

    if (argv[1] == std::string("-c")) {
        storage.parsedArgv.push_back(argv[1]);
        return 1;
    }

    if (argv[1] == std::string("-lc")) {
        storage.parsedArgv.push_back(argv[1]);
        return 2;
    }

    if (argv[1] == std::string("-dc")) {
        storage.parsedArgv.push_back(argv[1]);
        //TODO: combine camera details with argv[2] and after to form the full camera name in one string
        if (argc > 2) {
            //TODO: figure logic for multiple camera inputs, also for delimiters i.e inputs with "quotation marks"
            //Require commas to split between multiple camera names
            
        }
        return 3;
    }

    throw std::runtime_error("Unknown command. Use -h or --help for a list of options.");
}


int main(int argc, char** argv) {
    inputStorage storage;
    bool settingFileValid = false;
    int parseReturn = CLI_Parse(argc, argv, storage);
    std::cout << "CLI_Parse returned: " << parseReturn << std::endl;

    std::filesystem::path test = std::filesystem::current_path();

    for (std::filesystem::directory_iterator it(test); it != std::filesystem::directory_iterator(); ++it) {
        if (std::filesystem::exists(test / "settings.toml")) {
            settingFileValid = true;

//            std::cout << "DEBUG: settingFileValid = " << settingFileValid << std::endl;
            break;
        }
    }

    if (!settingFileValid) {
        std::cout << "Warning - Settings file not found." << std::endl;
        std::cout << "Warning - First time configuration starting..." << std::endl;
        std::cout << "Warning - PLease follow the incoming prompts as they appear." << std::endl;
        photo::photoSettings initPhotos;
        
        for (std::filesystem::directory_iterator it(test); it != std::filesystem::directory_iterator(); ++it) {
            if (!std::filesystem::exists(test / "settings.toml")) {
                throw std::runtime_error("Error: Failed to create settings.toml. Please check your file permissions and try again.");
            }

            settingFileValid = true;
            std::cout << "'settings.toml' created in APPDATA/locallow/photorina" << std::endl;
            break;
        }

        std::cout << "COnfiguration complete. Please run the program again: " << argv[0] << " -h for more options." << std::endl;
        return 0;
    }

    if (settingFileValid) {
        std::cout << "Input stored: " << storage.parsedArgv[0] << std::endl; 
        //TODO: rewrite if statements into switch-case structure
        photo::photoSettings initPhotos;
        switch (parseReturn) {
            
            case -1:
                throw std::runtime_error("No given option. Stop");
            case 0:
                return 0;

            case 1: {
                std::vector<std::string> cameraList = initPhotos.addCamera();
                initPhotos.addCamera(cameraList);
                break;
            }

            case 2: {
                initPhotos.listCamera();
                break;
            }
            
            case 3: {
                std::cout << storage.parsedArgv[1] << std::endl;
                //initPhotos.deleteCamera();  
            }
        }

    }

    
    return 0;
}


