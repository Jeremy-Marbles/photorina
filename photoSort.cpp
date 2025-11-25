#include "photoSort.h"

namespace photo {
	//begin photoSort_settings

	photoSort_settings::photoSort_settings() : cfgName("settings.toml"), cfgFile(cfgName){
		//TODO: check if cfg file exists in program folder
		if (!std::filesystem::exists(cfgName)) {
			std::cout << "Settings file not found. Creating default settings.toml..." << std::endl;
			//create default config file in TOML format
			toml::table defaultConfig;

			std::string os_string;
			#if defined(_WIN32_) || defined(_WIN64_)
				os_string = "OS = Windows\n";
			#elif defined(__linux__) || defined(__LINUX)
				os_string = "OS = Linux\n";
			#elif defined(__APPLE__) || defined(__MACH__)
				os_string = "OS = MacOS\n";
			#else 
				os_string = "OS = Unknown\n";
			#endif

			std::string homeDir = std::getenv("HOME") ? std::getenv("HOME") : std::getenv("USERPROFILE");
			std::filesystem::path picturesDir = std::filesystem::path(homeDir) / "Pictures";
	
			toml::array cameras;
			cameras.push_back(toml::table{
				{"CameraBrand", ""},
				{"CameraModel", ""},
				{"RAWFormat", toml::array{}}
			});
	
			defaultConfig.insert("System", toml::table{ {"OS", os_string} });
			defaultConfig.insert("Directories", toml::table{
				{"DefaultCWD", picturesDir.string()},
				{"DefaultDestination", ""}
			});
			defaultConfig.insert("Sorting",toml::table{
				{"ByDate", true},
				{"ByFormat", false},
				{"ByCameraModel", false},
				{"ByOtherMeta", false}
			});
			defaultConfig.insert("Metadata", toml::table{
				{"LensModel", ""},
				{"ExposureLevel", ""},
				{"Location", ""},
				{"Orientation", ""},
				{"Parameter", false}
			});
			defaultConfig.insert("Cameras", cameras);
			try {
				std::ofstream cfgFileStream(cfgName);
				cfgFileStream << toml::toml_formatter(defaultConfig);
				cfgFileStream.close();
				std::cout << "Default settings.toml created successfully." << std::endl;
			} catch (const std::ofstream::failure& createErr) {
				std::cerr << "Error creating settings file: " << createErr.what() << std::endl;
			}

		} else if (std::filesystem::exists(cfgName)) {
			//detect system, initiate default parameters
			//use toml parser for this part (toml.hpp)
			cfgFile.open(cfgName);

			toml::table config = toml::parse_file("settings.toml");
			toml::table* systemTable = config["System"].as_table();
			if (systemTable) {
				std::string os = (*systemTable)["OS"].value_or("Unknown");
				std::cout << "Detected OS: " << os << std::endl;
			} else {
				//handle missing system table
				std::cerr << "Warning: 'System' table missing in settings.toml." << std::endl;
				//create default system table
				toml::table defaultSystemTable;
				std::string os_string;
				#if defined(_WIN32_) || defined(_WIN64_)
					os_string = "OS = Windows\n";
				#elif defined(__linux__) || defined(__LINUX)
					os_string = "OS = Linux\n";
				#elif defined(__APPLE__) || defined(__MACH__)
					os_string = "OS = MacOS\n";
				#else 
					os_string = "OS = Unknown\n";
				#endif
				defaultSystemTable.insert("OS", os_string);
				config.insert("System", defaultSystemTable);
				// Optionally, write back to file
				std::ofstream cfgFileStream(cfgName);
				cfgFileStream << toml::toml_formatter(config);
				cfgFileStream.close();
				std::cout << "Default 'System' table created in settings.toml." << std::endl;
			}

			toml::table* dirTable = config["Directories"].as_table();
			// If table exists, pull default CWD and destination; otherwise, use fallback values and warn user
			if (dirTable) {
				std::string defaultCWD = (*dirTable)["DefaultCWD"].value_or("");
				std::string defaultDest;
				if (dirTable->contains("DefaultDestination")) {
					defaultDest = (*dirTable)["DefaultDestination"].value_or("");
				} else {
						defaultDest = "";
						std::cerr << "Warning: 'DefaultDestination' key not found in Directories table." << std::endl;
					}
				std::cout << "Default CWD: " << defaultCWD << std::endl;
			} else {
				std::cerr << "Warning: 'Directories' table missing in settings.toml. Using fallback values." << std::endl;
				std::string defaultCWD = "";
				std::string defaultDest = "";
				// Optionally, set defaultCWD to user's Pictures folder as fallback
				std::string homeDir = std::getenv("HOME") ? std::getenv("HOME") : std::getenv("USERPROFILE");
				std::filesystem::path picturesDir = std::filesystem::path(homeDir) / "Pictures";
				defaultCWD = picturesDir.string();
				std::cout << "Fallback Default CWD: " << defaultCWD << std::endl;
			}

			//TODO: metadata handling
		}

		cfgFile.close();
	}

	photoSort_settings::~photoSort_settings() {
		//deletes config (only to be used when uninstalling)
	}

	//Load from existing config file
	//I.E. pull default CWD, destination folder, etc.
	int photoSort_settings::loadSettings(int command) {
		if (!std::filesystem::exists(cfgName)) {
			std::cerr << "Config file does not exist in folder! Aborting..." << std::endl;
			return -1;
		}
	}

	int photoSort_settings::modifySettings(int command) {
		if (!std::filesystem::exists(cfgName)) {
			std::cerr << "Config file does not exist in folder! Aborting..." << std::endl;
			return -1;
		}

		//command 1: set default destination path

	}

	int photoSort_settings::populateCfg(int command) {
		if (!std::filesystem::exists(cfgName)) {
			std::cerr << "Config file does not exist in folder! Aborting..." << std::endl;
			return -1;
		}
	}

	//Two overloads for adding camera info for the following:
	//pull camera info from metadata read-from-file
	//manual selection from user via dropdown index (always represented with int function)
	std::string photoSort_settings::addCamera() {
		if (!std::filesystem::exists(cfgName)) {
			std::cerr << "Config file does not exist in folder! Aborting..." << std::endl;
			return "";
		}
	}

	//Given user input vector of camera info (3 strings: brand, model, raw formats)
	int photoSort_settings::addCamera(std::vector<std::string> cameraInfo) {
		if (!std::filesystem::exists(cfgName)) {
			std::cerr << "Config file does not exist in folder! Aborting..." << std::endl;
			return -1;
		}


	}

	std::string photoSort_settings::getLens() {
		if (!std::filesystem::exists(cfgName)) {
			std::cerr << "Config file does not exist in folder! Aborting..." << std::endl;
			return "";
		}
	}
	
	// end photoSort_settings

	// begin photoSort  
	// Constructors (or just constructor)
	// New constructors will pull from photosort settings to get basic info
	// ToDo: shorthand rename to photoSort
	photoSort::photoSort() {
		root_ = std::filesystem::path();

		try {
			CWD_ = std::filesystem::current_path();
		} catch (std::filesystem::filesystem_error badCWD) {
			std::cerr << "PS Construct error: " << badCWD.what() << '\n';
			std::cerr << "Path: " << badCWD.path1() << '\n';
			std::cerr << "Err: " << badCWD.code() << std::endl; 
		}

	}

	//Destructor
	photoSort::~photoSort() {
		directory_CWD.clear();
	}

	int photoSort::populateCWD_Vector() {
		if (!directory_CWD.empty()) {
			directory_CWD.clear();
		}

		try {
			for (const auto& entry : std::filesystem::directory_iterator(CWD_)) {
				if (entry.is_regular_file()) {
					directory_CWD.push_back(entry.path());		
				}
			}
		} catch (const std::filesystem::filesystem_error& populateErr) {
			std::cerr << "Error populating current directory: " << populateErr.what() << '\n';
			if (!populateErr.path1().empty()) {
				std::cerr << "Path: " << populateErr.path1().string();
			}
		}

		return directory_CWD.size();
	}

	void photoSort::singleMove(std::filesystem::path des, int vec_pos) {
		if (directory_CWD.empty()) {
			std::cerr << "Error: empty CWD" << std::endl;
			return;
		}

		if (!is_directory(des)) {
			std::cerr << "Path error: destination is not a directory" << std::endl;
			return;
		}

		try { 
			const std::filesystem::path& pullElement =  directory_CWD[vec_pos];
			std::filesystem::rename(pullElement, des / pullElement.filename());
		} catch (std::filesystem::filesystem_error& moveErr) { 
			std::cerr << "Unable to move file: " << moveErr.what() << std::endl;
			if (!moveErr.path1().empty()) std::cerr << "Source: " << moveErr.path1().string() << std::endl;
			if (!moveErr.path2().empty()) std::cerr << "Destination: " << moveErr.path2().string() << std::endl;
		}

	}

	void photoSort::multiMove(std::filesystem::path toMove, int command) {
		//threaded version of singleMove, moving multiple files at once
		//command based on user input:
		//1: move all
		//2: move selected (to be implemented with parameters)
		//3: move partial (user selects range before starting threads)
	
		if (!std::filesystem::is_directory(toMove)) {
			std::cerr << "Destination is not a directory: " << toMove.string() << std::endl;
			return;
		}
		
		//init thread items 
		std::vector<std::thread> threadArray;
		std::mutex turnMutex;
		std::vector<std::string> errorStorage;
		std::atomic<int> filesMoved(0);

		const unsigned int num_threads = std::thread::hardware_concurrency();
		const size_t files_per_thread = directory_CWD.size() / num_threads;

		//Logic for "move all"
		if (command == 1) {
			for (unsigned int i = 0; i < num_threads; i++) {
				threadArray.emplace_back([this, i, num_threads, files_per_thread, &toMove, &turnMutex, &errorStorage, &filesMoved]() {
				size_t begin = i * files_per_thread;
				size_t end = (i == num_threads - 1) ? directory_CWD.size() : begin + files_per_thread;
				
				for (size_t j = begin; j < end; j++) { 
					try {	
						const auto& sourcePath =  directory_CWD[j];
						std::filesystem::path desPath = toMove / sourcePath.filename();
						std::filesystem::rename(sourcePath, desPath);
						filesMoved++;
					} catch (const std::filesystem::filesystem_error& threadMoveErr) {
						std::lock_guard<std::mutex> lock(turnMutex);
						errorStorage.push_back("Failed to move '" + directory_CWD[j].string() + "': " + threadMoveErr.what());	
					}
				}
			});
		}

		//wait for threads to complete
		for (auto& t : threadArray) {
			if (t.joinable()) {
				t.join();
			}
		}

		//Report on moved files
		std::cout << "Moved " << filesMoved << " files." << std::endl;
		if (!errorStorage.empty()) {
			std::cerr << "Encountered " << errorStorage.size() << " errors:" << std::endl;
			for (const auto& err : errorStorage) {
				std::cerr << "- " << err << std::endl;
				}
			}
		}
	}

	//Writes destination folder in settings.cfg
	std::filesystem::path photoSort::setFolder(std::filesystem::path newPath) {
		if (std::filesystem::is_directory(newPath)) {
			if (std::filesystem::exists(sortSettings_.cfgPath)) {
				sortSettings_.cfgFile.open(sortSettings_.cfgPath);
				//TODO: check if "DefaultDestination" line exists; if so, overwrite instead of appending
				sortSettings_.cfgFile << "DefaultDestination=" << newPath.string() << '\n';			
				sortSettings_.cfgFile.close();
			
			}
            
        	}
		
		return; //TODO: return current path if fails
	}
    

	std::filesystem::path photoSort::setCWD_(std::string folder) {
		if (std::filesystem::exists(sortSettings_.cfgPath)) {
			sortSettings_.cfgFile.open(sortSettings_.cfgPath); 
			//TODO: check if "DefaultCWD" line exists; if so, overwrite instead of appending
			sortSettings_.cfgFile << "DefaultCWD=" << folder << '\n';
			sortSettings_.cfgFile.close();

			CWD_ = std::filesystem::path(folder);

			//Get new CWD info and place it into the vector
			populateCWD_Vector();

			return CWD_;

		}
	}

	//creates a directory on the CWD
	std::filesystem::path photoSort::createDir(std::string name) {
		try {
			std::filesystem::path createNew = name;
			if (std::filesystem::create_directory(createNew)) {
				std::cout << "Create " << name << std::endl;
			} else {
				std::cout << "Name already exists in directory!" << std::endl;
			}

			return createNew;
		} catch (std::filesystem::filesystem_error createErr) {
			std::cerr << "Error creating directory: " << createErr.what() << std::endl;
			std::cerr << "Path: " << createErr.path1().string() << std::endl;
			return std::filesystem::path();
		}

		return std::filesystem::path();
	}
}
