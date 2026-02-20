#include <iostream>
#include "photoReset.h"

using namespace std::string_view_literals;

int main() {

    photo::photoSettings newSort;

    toml::table settingsTable = toml::parse_file("settings.toml");

    //std::cout << settingsTable << std::endl;

    //TODO: test setMetadata function
    
    static constexpr std::string_view cameraTest = R"(
        CameraBrand = 'Fujifilm'
        CameraModel = 'X-T30 III'
        Formats = [ '.RAF', '.JPEG' ]
    )"sv;
    
    toml::table newTable = toml::parse(cameraTest);
    std::string key, value;

    key = cameraTest.substr(0, cameraTest.find('=') - 1);
    value = cameraTest.substr(cameraTest.find('=') + 2, cameraTest.find('\n') - cameraTest.find('=') - 3);

    toml::table metaDataTable = newSort.setMetadata(key, value);

    std::cout << "Metadata Table after setMetadata():\n" << metaDataTable << std::endl;

    return 0;
}


