#include <iostream>
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
struct ParsedCommand {
    std::string flag;
    std::vector<std::string> args;
};

std::vector<ParsedCommand> parseArguments(int argc, char** argv) {
    std::vector<ParsedCommand> commands;
    for (int i = 1; i < argc;) {

    }

    return commands;
}

void printHelp(const char* programName) {
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

int main(int argc, char** argv) {
    //if no argument, display a usage line, error line and "Type photorina -h to see a list of options"
    if (argc == 1) {
        std::cout << "Usage:" << argv[0] << " [OPTION] <path>" << std::endl;
        std::cerr << argv[0] << ": error: you must provide a command and path to your folder" << std::endl;
        std::cout << "Type " << argv[0] << " -h or " << argv[0] << " --help to see a list of options and the usage guide" << std::endl;
        std::cout << "Note: first time run initiates a configuration set up. Please make sure to follow the instructions when prompted" << std::endl;
        return 0;
    }

    bool settingFileValid = false;

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
        }
    }

    if (settingFileValid) {
//  std::cout << "Verified settings.toml." << std::endl;
        photo::photoSort newSort;
        std::vector<ParsedCommand> commands;
        for (int i = 1; i < argc; ++i){
            std::cout << "arguments: " << argc << std::endl;           
            ParsedCommand cmd;
            std::string args = argv[i];
            cmd.flag = args;
            cmd.args.push_back(args);
            commands.push_back(cmd);
            
        }
        try {
            for (const auto& cmd : commands) {
//              std::cout << "Flag: " << cmd.flag << std::endl;
                if (cmd.flag =="-sd") {

                    if (cmd.args.size() < 2) {
                        std::cerr << "Error: Missing argument for -sd flag. Please provide a path." << std::endl;
                        return 1;
                    }
                    std::string destinationPath = cmd.args[1];
                    newSort.setDestination(destinationPath);
                } else if (cmd.flag == "-sc") {
                    if (cmd.args.size() < 2) {
                        std::cerr << "Error: Missing argument for -sc flag. Please provide a path." << std::endl;
                        return 1;
                    }
                    std::string sourcePath = cmd.args[1];
                    newSort.setCWD(sourcePath);
                } else if (cmd.flag == "-b") {
                    if (cmd.args.size() < 3) {
                        std::cerr << "Error: Missing arguments for -b flag. Please provide source and destination paths." << std::endl;
                        return 1;
                    }
                    std::string sourcePath = cmd.args[1];
                    std::string destinationPath = cmd.args[2];
                    newSort.moveToDestination(sourcePath, destinationPath);
                } else if (cmd.flag == "-a") {
                    newSort.autoMove();
                    //TODO: add manual selection of parameters to sort photos by
                } else if (cmd.flag == "-h" || cmd.flag == "--help") {
                    printHelp(argv[0]);
                } else if (cmd.flag == "-c") {
                    photo::photoSettings addCamera;
                } else if (cmd.flag == "-dc") {
                    //logic for deleting camera from list
                    //-print out current camera list
                    //-specify that user must input the "camera model" to be removed from list
                    //-give option to delete all cameras (and go to a double check option to make sure they want to remove all)
                    //-run deleteCamera function
                } else if (cmd.flag == "-lc") {
                    photo::photoSettings listCamera;
                } else {
                    std::cerr << "Error: Unknown flag '" << cmd.flag << "'. Use -h or --help for usage information." << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Fatal error: " << e.what() <<std::endl;
            return 2;
        }
    }

    
    return 0;
}


