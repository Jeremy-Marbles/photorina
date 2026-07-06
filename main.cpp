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

int main(int argc, char** argv) {
    //if no argument, display a usage line, error line and "Type photorina -h to see a list of options"
    if (argc == 1) {
        std::cout << "Usage: photorina [OPTION] <path>" << std::endl;
        std::cerr << "photorina.exe: error: you must provide a command and path to your folder" << std::endl;
        std::cout << "Type photorina -h or photorina --help to see a list of options and the usage guide" << std::endl;
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
        for (int i = 0; i < argc; ++i){
            std::cout << "arguments: " << argc << std::endl;           
        }
        try {
            for (const auto& cmd : commands) {
                if (cmd.flag =="-sd") {
//                  std::cout << "Flag: " << cmd << std::end;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Fatal error: " << e.what() <<std::endl;
            return 2;
        }
    }

    
    return 0;
}


