#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <filesystem>
#include <system_error>
#include <fstream>
#include <vector>
#include <chrono>

//https://makefiletutorial.com/

#include "dependencies/toml.hpp"
#include "dependencies/date.h"
 
namespace photo {

    class photoSettings {
        private:
            std::string cfgName = "settings.toml";

        public:
            photoSettings();
            std::vector<std::string> addCamera();
            toml::array addCamera(std::vector<std::string> cameraInfo);

            toml::table setMetadata(std::string key, std::string value);
    };
    
}

namespace photo {

    class photoSort {
        private:
            std::filesystem::path root_;
            std::filesystem::path CWD_;
            std::vector<std::filesystem::directory_entry> directory_CWD;

        public:
            photoSort();
            void populateCWD();
    };

}
