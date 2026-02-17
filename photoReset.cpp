#include "photoReset.h"

namespace photo {
    photoSettings::photoSettings()  {
        //new FIXME: settings.toml parameters are deleted each time else statement is triggered 
        //constructor is not meant to reset each time for future use.
        //Should not be calling addCamera each time exe is run.
        //FIXED: by calling toml parse OR parse_file first before fstream. 

        //RE:FIXME: set camera setup to separate function before config creation, right here -COMPLETE-

        if (!std::filesystem::exists(cfgName)) {
            std::cout << "'settings.toml' not found. Creating file..." << std::endl;

            std::cout << "Add camera info:" << std::endl;
            std::vector<std::string> camInfo = addCamera();

            std::ofstream fileOut(cfgName);
            toml::table defaultConfig;

            std::string os_string;
            #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
                os_string = "Windows";
            #elif defined(__linux__) || defined(__LINUX)
                os_string = "Linux";
            #elif defined(__APPLE__) || defined(__MACH__)
                os_string = "MacOS";
            #else
                os_string = "Unknown";
            #endif
            defaultConfig.insert("System", toml::table { { "OS", os_string } });
           
            toml::array cameras;
            cameras.push_back(toml::table{
                {"CameraBrand", camInfo[0]},
                {"CameraModel", camInfo[1]},
                {"RAWFormat", toml::array{camInfo[2]} }     //TODO: get multiple formats into this array
            });

            defaultConfig.insert("Directories", toml::table {
                {"CWD", std::filesystem::current_path().string()},
                {"Destination", "BLANK"}
            });

            defaultConfig.insert("Sorting", toml::table {
                {"ByDate", true},
                {"ByFormat", false},
                {"ByCameraModel", false},
                {"ByOtherMeta", false}
            });
            
            //Potential FIXME to uncouple AI code: https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
            //AKA USE date_v2!!!!!
            std::string date_str = date::format("%Y-%m-%d", date::floor<date::days>(std::chrono::system_clock::now())); 

            defaultConfig.insert("Metadata", toml::table {
                {"Date", date_str},
                {"LensModel", "BLANK"},
                {"Exposure", "BLANK"},
                {"Location", "BLANK"},
                {"Orientation", "BLANK"},
                {"Parameters", false}
            });

            defaultConfig.insert("Parameter", toml::table {
                {"Metering", "Matrix"},
                {"MeteringOptions", toml::array{"Spot", "Center-weighted", "Matrix", "Evaluative"}}
            });

            auto tempTable = defaultConfig.as_table();
            
            tempTable->insert_or_assign("Camera", cameras);
            

            fileOut << defaultConfig;
            fileOut.close();
            
            //lead to manual setup of parameters
        } else {
            //FIXME: toml::parse fails to read existing file
            auto setParameters = toml::parse_file(cfgName);            
            try {
                std::vector<std::string> camInfo;

                // Convert old [Camera] table to [[Camera]] array of tables
                if (setParameters.contains("Camera") && setParameters["Camera"].is_table()) {
                    auto oldCameraTable = setParameters["Camera"].as_table();
                    toml::array cameras;
                    
                    // Extract data from old table format
                    toml::table newCamera;
                    if (oldCameraTable->contains("CameraBrand")) {
                        newCamera.insert("CameraBrand", (*oldCameraTable)["CameraBrand"]);
                    }
                    if (oldCameraTable->contains("CameraModel")) {
                        newCamera.insert("CameraModel", (*oldCameraTable)["CameraModel"]);
                    }
                    if (oldCameraTable->contains("RAWFormat")) {
                        newCamera.insert("RAWFormat", (*oldCameraTable)["RAWFormat"]);
                    }
                    
                    cameras.push_back(newCamera);
                    setParameters.insert_or_assign("Camera", cameras);
                    
                    std::cout << "Converted [Camera] table to [[Camera]] array format." << std::endl;
                }

                bool needInfo = false;
                if (setParameters.contains("Camera") && setParameters["Camera"].is_array()) {
                    auto cameraArray = setParameters["Camera"].as_array();
                    if (!cameraArray->empty()) { 
                        auto firstCamera = cameraArray->get(0)->as_table();
                        if (firstCamera && firstCamera->contains("CameraBrand")) {
                            auto brand = (*firstCamera)["CameraBrand"].value<std::string>();
                            if(!brand.has_value() || brand.value().empty()) {
                                needInfo = true;
                            }
                        }
                    }
                    else {
                        needInfo = true;
                    }
                }

                if (needInfo) {
                    std::cout << "Add camera info:" << std::endl;
                    camInfo = addCamera();
                    toml::array cameras;
                    cameras.push_back(toml::table{
                        {"CameraBrand", camInfo[0]},
                        {"CameraModel", camInfo[1]},
                        {"RAWFormat", toml::array{camInfo[2]} }     //TODO: get multiple formats into this array
                    });
                    setParameters.insert_or_assign("Camera", cameras);
                }

                if (!setParameters.contains("Directories") || 
                    !setParameters["Directories"].as_table()->contains("CWD") ||
                    setParameters["Directories"]["CWD"].value<std::string>().value_or("").empty()) {
                    if (!setParameters.contains("Directories")) {
                        setParameters.insert("Directories", toml::table{});
                    }
                    setParameters["Directories"].as_table()->insert_or_assign("CWD", std::filesystem::current_path().string());
                }

                std::ofstream configOut(cfgName);
                configOut << setParameters;
                configOut.close();
            } catch (const std::exception& e) {
                std::cerr << "Error reading 'settings.toml':\n" << e.what() << std::endl;
            }
        }

    }

    std::vector<std:: string> photoSettings::addCamera() {
        std::vector<std::string> cameraInfo;
        std::string brand, model, rawFormatsInput;

        std::cout << "Enter Camera Brand: ";
        std::getline(std::cin, brand);
        cameraInfo.push_back(brand);

        std::cout << "Enter Camera Model: ";
        std::getline(std::cin, model);
        cameraInfo.push_back(model);

        std::cout << "Enter RAW Formats (comma-separated, e.g., .CR2,.NEF): ";
        std::getline(std::cin, rawFormatsInput);

        size_t start = 0;
        size_t end = rawFormatsInput.find(',');
        while (end != std::string::npos) {
            cameraInfo.push_back(rawFormatsInput.substr(start, end - start));
            start = end + 1;
            end = rawFormatsInput.find(',', start);
        }
        cameraInfo.push_back(rawFormatsInput.substr(start));

        return cameraInfo;
    }

    toml::array photoSettings::addCamera(std::vector<std::string> cameraInfo) {
        try {
            toml::table settingsTable = toml::parse_file(cfgName);
            if (settingsTable.contains("Camera")) {
                toml::array& cameraArray = *settingsTable["Camera"].as_array();
                
                toml::table newCamera;
                newCamera.insert("CameraBrand", cameraInfo.size() > 0 ? cameraInfo[0] : "");
                newCamera.insert("CameraModel", cameraInfo.size() > 1 ? cameraInfo[0] : "");

                toml::array rawFormats;
                if (cameraInfo.size() > 2) {
                    for (size_t i = 2; i < cameraInfo.size(); ++i) {
                        rawFormats.push_back(cameraInfo[i]);
                    }
                }

                newCamera.insert("RAWFormat", rawFormats);

                cameraArray.push_back(newCamera);
                std::ofstream configOut(cfgName);
                configOut << settingsTable;
                configOut.close();

                return cameraArray;
            }
        } catch (const std::exception& e) { 
            std::cerr << "Failed to add camera info:\n" << e.what() << std::endl;
            return toml::array{};
        }
    }

    toml::table photoSettings::setMetadata(std::string key, std::string value) {
        try {
            toml::table settingsTable = toml::parse_file(cfgName);
            
            if (settingsTable.contains("Metadata")) {
                auto metaTable = settingsTable["Metadata"].as_table();

                if (key == "Parameters") {
                    bool which;
                    if (value == "false") {
                        which = false;
                        metaTable->insert_or_assign("Parameters", which);
                    } else if (value == "true") {
                        which = true;
                        metaTable->insert_or_assign("Parameters", which);
                    }
                }
                return *metaTable;
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to get metadata table:\n" << e.what() << std::endl;

            return toml::table{};
        }
    }
}

namespace photo {
    photoSort::photoSort() {
        root_ = std::filesystem::path();

        try {
            CWD_ = std::filesystem::current_path();
        } catch (std::filesystem::filesystem_error& badCWD){
            std::cerr << "PS Construct error:\n" << badCWD.what() << '\n';
            std::cerr << "Path: " << badCWD.path1() << '\n';
            std::cerr << "Err: " << badCWD.code() << std::endl;
        }
    }

    void photoSort::populateCWD() {
        if (!directory_CWD.empty()) {
            directory_CWD.clear();
        }

        try {
            for (const auto& entry : std::filesystem::directory_iterator(CWD_)) {
                if (entry.is_regular_file()) {
                    directory_CWD.push_back(entry);
                }
            }
        } catch (const std::filesystem::filesystem_error& populateErr) {
            std::cerr << "Error populating current directory:\n" << populateErr.what();
        }   
    }
}
