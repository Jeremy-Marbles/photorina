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
                {"Formats", toml::array{camInfo[2]} }     //TODO: get multiple formats into this array
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
                {"Author", "BLANK"},
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
                    if (oldCameraTable->contains("Formats")) {
                        newCamera.insert("Formats", (*oldCameraTable)["Formats"]);
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
                        {"Formats", toml::array{camInfo[2]} }     //TODO: get multiple formats into this array
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
        std::string brand, model, formatsInput;

        std::cout << "Enter Camera Brand: ";
        std::getline(std::cin, brand);
        cameraInfo.push_back(brand);

        std::cout << "Enter Camera Model: ";
        std::getline(std::cin, model);
        cameraInfo.push_back(model);

        std::cout << "Enter Formats (comma-separated, e.g., .CR2,.NEF,.JPEG,.HIF): ";
        std::getline(std::cin, formatsInput);

        size_t start = 0;
        size_t end = formatsInput.find(',');
        while (end != std::string::npos) {
            cameraInfo.push_back(formatsInput.substr(start, end - start));
            start = end + 1;
            end = formatsInput.find(',', start);
        }
        cameraInfo.push_back(formatsInput.substr(start));

        return cameraInfo;
    }

    toml::array photoSettings::addCamera(std::vector<std::string> cameraInfo) {
        try {
            toml::table settingsTable = toml::parse_file(cfgName);
            if (settingsTable.contains("Camera")) {
                toml::array& cameraArray = *settingsTable["Camera"].as_array();
                
                toml::table newCamera;
                newCamera.insert("CameraBrand", cameraInfo.size() > 0 ? cameraInfo[0] : "");
                newCamera.insert("CameraModel", cameraInfo.size() > 1 ? cameraInfo[1] : "");

                toml::array formats;
                if (cameraInfo.size() > 2) {
                    for (size_t i = 2; i < cameraInfo.size(); ++i) {
                        formats.push_back(cameraInfo[i]);
                    }
                }

                newCamera.insert("Formats", formats);

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
        throw std::runtime_error("end of addCamera function:");
    }

    int photoSettings::deleteCamera(std::string cameraModel) {
        try {
            toml::table settingsTable = toml::parse_file(cfgName);
            
            if (settingsTable.contains("Camera")) {
                toml::array& cameraArray = *settingsTable["Camera"].as_array();
                
                for (size_t i = 0; i < cameraArray.size(); i++) {
                    auto modelTable = cameraArray[i].as_table();
                    auto getModel = modelTable->at("CameraModel").value<std::string>();

                    if (*getModel == cameraModel) {
                        cameraArray.erase(cameraArray.begin() + i);
                        std::ofstream configOut(cfgName);
                        configOut << settingsTable;
                        configOut.close();
                        break;
                    }
                }
                return 1;
            }

        } catch (const std::exception& e){
            std::cerr << "Failed to delete camera info:\n" << e.what() << std::endl;
            return -1;
        }

        throw std::runtime_error("end of deleteCamera function:");
    }

    //Returns updated Metadata table after setting key-value pair.
    //On failure, returns empty table and prints error message to console.
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
                else {
                    metaTable->insert_or_assign(key, value);
                }

                settingsTable.insert_or_assign("Metadata", *metaTable);
                std::ofstream configOut(cfgName);
                configOut << settingsTable;
                configOut.close();

                return settingsTable["Metadata"].as_table() ? *settingsTable["Metadata"].as_table() : toml::table{};
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to get metadata table:\n" << e.what() << std::endl;

            return toml::table{};
        }

        throw std::runtime_error("end of setMetadata function:");
    }

    //Returns updated Parameter table after setting key-value pair.
    //On failure, returns empty table and prints error message to console.
    toml::table photoSettings::setParameter(std::string key, std::string value) {
        try {
            toml::table settingsTable = toml::parse_file(cfgName);

            if (settingsTable.contains("Parameter")) {
                auto paramTable = *settingsTable["Parameter"].as_table();
                paramTable.insert_or_assign(key, value);

                //TODO: brainstorm how to utilize the array in the toml for metering options
                std::ofstream configOut(cfgName);
                configOut << settingsTable;
                configOut.close();
                return paramTable;
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to get parameter table:\n" << e.what() << std::endl;

            return toml::table{};
        }

        throw std::runtime_error("end of setParameter function:");
    }

    //Returns updated Sorting table after setting key-value pair.
    //On failure, returns empty table and prints error message to console.
    toml::table photoSettings::setSort(std::string key, bool value) {
        try {
            toml::table settingsTable = toml::parse_file(cfgName);

            if (settingsTable.contains("Sorting")) {
                auto sortTable = *settingsTable["Sorting"].as_table();
                sortTable.insert_or_assign(key, value);

                std::ofstream configOut(cfgName);
                configOut << settingsTable;
                configOut.close();
                return sortTable;
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to get sorting table:\n" << e.what() << std::endl;
            return toml::table{};
        }

        throw std::runtime_error("end of setSort function:");
    }
}

namespace photo {
    /* PhotoSort
     * 1) Verifies existence of settings.toml
     * 2) Read [System] table for what system is loaded
     * 3) Set default CWD to photo folder of operating system 
     * 4) Populate vector with directory entries of CWD for sorting functions to utilize
     * */
    photoSort::photoSort() {
        //temporarily set CWD_ as photorina folder and get the settings file
        CWD_ = std::filesystem::current_path(); //change to search Appdata for windows or .config in linux
        if (!std::filesystem::exists(CWD_ / "settings.toml")) {
            throw std::runtime_error("Settings file does not exist.");
        }

        toml::table settingsTable = toml::parse_file(cfgName);
        try {
            std::string OS_string = *settingsTable["System"]["OS"].value<std::string>();
            if (OS_string == "Windows") {
                root_ = std::filesystem::path("C:");
            } else if (OS_string == "MacOS" || OS_string == "Linux") {  //test on framework
                root_ = std::filesystem::path("/");
            } else {        //unknown system. require user to specify root directory
                std::cout << "Unknown operating system. Please specify root directory: ";
                std::string rootInput;
                std::getline(std::cin, rootInput);
                root_ = std::filesystem::path(rootInput);
            }
            
            //debug
            //std::cout << root_.string() << std::endl;
            
            if (settingsTable.contains("Directories") && 
                settingsTable["Directories"].as_table()->contains("CWD")) {
                
                auto CWDValue = settingsTable["Directories"]["CWD"].value<std::string>();
                CWD_ = std::filesystem::current_path();
                
                //TODO: set CWD to default photo folder of operating system
                //compare CWDValue to CWD_ current_path(), if true change to default photo folder, i.e "Pictures"
                //fall back if folder can't be located, throw a warning that user needs to specify which folder to use
                if (CWD_.string() == CWDValue) {
                    if (OS_string == "Windows") {
                        const char* username = std::getenv("USERNAME");
                        if(username != nullptr) {
                            CWD_ = root_ / "Users" / username / "Pictures";
                        } else {
                            std::cerr << "Cannot retrieve USERNAME enviroment variable" << std::endl;
                            CWD_ = std::filesystem::current_path();
                        }
                        
                        settingsTable["Directories"].as_table()->insert_or_assign("CWD", CWD_.string());
                        std::ofstream configOut(cfgName);
                        configOut << settingsTable;
                        configOut.close();
                    } else if (OS_string == "MacOS" || OS_string == "Linux") {
                        const char* username = std::getenv("USER");
                        if (username != nullptr) {
                            CWD_ = root_ / "Users" / username / "Pictures";
                        } else {
                            std::cerr << "Could not retrieve USER environment variable" << std::endl;
                            CWD_ = std::filesystem::current_path();
                        }
 
                        settingsTable["Directories"].as_table()->insert_or_assign("CWD", CWD_.string());
                        std::ofstream configOut(cfgName);
                        configOut << settingsTable;
                        configOut.close();
                    } else {
                        //throw warning here, do not modify toml table
                        std::cerr << "Warning: OS not specified. Manually specify path to pictures folder." << std::endl;
                    }
                }
            }
        } catch (std::filesystem::filesystem_error& badCWD){
            std::cerr << "PS Construct error:\n" << badCWD.what() << '\n';
            std::cerr << "Path: " << badCWD.path1() << '\n';
            std::cerr << "Err: " << badCWD.code() << std::endl;
        }
    }

    void photoSort::populateCWD() {
        
    }

    std::filesystem::path photoSort::setCWD(std::string direct) {   //takes in name of a folder path
        std::filesystem::path projectPath = std::filesystem::current_path() / "settings.toml"; 
        std::error_code ec;
        if (!std::filesystem::exists(projectPath, ec)) {
            throw std::filesystem::filesystem_error("Cannot find settings.toml", projectPath, ec);
        }
        
        try {
            std::filesystem::path newPath = std::filesystem::path(direct);
            toml::table settingsTable = toml::parse_file(cfgName);

            settingsTable["Directories"].as_table()->insert_or_assign("CWD", newPath.string());
            std::ofstream configOut(cfgName);
            configOut << settingsTable;
            configOut.close();

            return newPath;

        } catch (std::filesystem::filesystem_error badCWD) {

        }

        throw std::runtime_error("end of setCWD function:");
    }

    std::filesystem::path photoSort::setDestination(std::string direct) {

    }

    void photoSort::moveToDestination(std::string working, std::string destination) {

    }

    void photoSort::autoMove() {

    }
}
