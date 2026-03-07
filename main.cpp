#include <iostream>
#include "photoReset.h"

using namespace std::string_view_literals;

//Notes:
//In the future, move toml to Appdata or ~/.config for better user experience and to avoid issues with permissions in CWD.

int main() {

    photo::photoSettings newSort;

    toml::table settingsTable = toml::parse_file("settings.toml");

    //std::cout << settingsTable << std::endl;

    //TODO: test setMetadata function
    
    static constexpr std::string_view cameraTest = R"(      
        CameraBrand = 'Fujifilm'
        CameraModel = 'X-T30 III'
        Formats = [ '.RAF', '.JPEG' ]
    )"sv; //keep for insert_or_assign tests
    
    toml::table newTable = toml::parse(cameraTest);
    std::string key, value;

    key = cameraTest.substr(0, cameraTest.find('=') - 1);
    value = cameraTest.substr(cameraTest.find('=') + 2, cameraTest.find('\n') - cameraTest.find('=') - 3);

    //std::vector <std::string> cameraInfo = newSort.addCamera();
    
    //std::cout << cameraInfo[0] << ", " << cameraInfo[1] << ", " << cameraInfo[2] << ", " << cameraInfo[3] << std::endl;

    //toml::array camera = newSort.addCamera(cameraInfo);

    //std::cout << "Array after addCamera:\n" << camera << std::endl;

    std::string metaKey = "Author";
    std::string metaValue = "BLANK";

    //toml::table photoMeta = newSort.setMetadata(metaKey, metaValue);

    //std::cout << "Metadata Table after setMetadata():\n" << photoMeta << std::endl;
 
    //init photoSort
    //std::cout << std::endl;

    photo::photoSort newSort2;

    std::filesystem::path testUnit = newSort2.setCWD("C:\'Users\'marbl\'Pictures");
    return 0;
}


