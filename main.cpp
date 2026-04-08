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
} */
struct ParsedCommand {
    std::string flag;
    std::vector<std::string> args;
};

int main(int argc, char** argv) {
    //if no argument, display a usage line, error line and "Type photorina -h to see a list of options"
    if (argc == 1) {
        std::cout << "Usage: photorina [OPTION] <path>" << std::endl;
        std::cerr << "photorina.exe: error: you must provide a command and path to your folder" << std::endl;
        std::cout << "Type photorina -h or photorina --help to see a list of options and usage guide" << std::endl;
        return 0;
    }
}
