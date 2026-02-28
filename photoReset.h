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
        protected:
            std::string cfgName = "settings.toml";

        public:
            photoSettings();
            std::vector<std::string> addCamera();
            toml::array addCamera(std::vector<std::string> cameraInfo);
            int deleteCamera(std::string cameraModel);

            toml::table setMetadata(std::string key, std::string value);
            toml::table setParameter(std::string key, std::string value);
            toml::table setSort(std::string key, bool value);
    };
    
}

//NOTES:
//- Pattern metering is called "Evaluative" in Fujifilm and Canon cameras, "Matrix" in Nikon, "Pattern" in Sony.
namespace photo {

    class photoSort : public photoSettings {
        private:
            std::filesystem::path root_;
            std::filesystem::path CWD_;
            std::vector<std::filesystem::directory_entry> directory_CWD;

        public:
            photoSort();
            void populateCWD();

            std::filesystem::path getCWD_();
    };

}
