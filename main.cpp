#include <iostream>
#include <thread>
#include "photoReset.h"

using namespace std::string_view_literals;

//Notes:
//In the future, move toml to Appdata or ~/.config for better user experience and to avoid issues with permissions in CWD.

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

int main() {

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

    return 0;
}


