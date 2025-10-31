#include "photoSort.h"

namespace photo {
	//begin photoSort_settings
	
	photoSort_settings::photoSort_settings() : cfgName("settings.cfg"), cfgFile(cfgName){
		//TODO: check if cfg file exists in program folder
		if (!std::filesystem::exists(cfgName)) {
			std::cout << "Settings file not found. Creating default settings.cfg..." << std::endl;
			cfgFile.open(cfgName);

			if (cfgFile.is_open()) {
				cfgFile << "[System]\n";
				#if defined(_WIN32_) || defined(_WIN64_)
					cfgFile << "OS=Windows\n";
				#elif defined(__linux__) || defined(__LINUX)
					cfgFile << "OS=Linux\n";
				#elif defined(__APPLE__) || defined(__MACH__)
					cfgFile << "OS=MacOS\n";
				#else 
					cfgFile << "OS=Unknown\n";
				#endif
				//TODO: add more default settings
				cfgFile << "\n";

				cfgFile << "[Sorting]\n";
				//Set default CWD to user's photo folder
				cfgFile << "DefaultCWD=" << std::filesystem::path(std::getenv("HOME")) / "Pictures" << "\n";
				cfgFile.close();
			} else {
				std::cerr << "Error creating settings file!" << std::endl;
			}
		}


		//Close file if open, somehow
		if(cfgFile.is_open()) {
			cfgFile.close();
		}
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

	std::filesystem::path photoSort::setFolder(std::filesystem::path newPath) {
		if (std::filesystem::is_directory(newPath)) {
			CWD_ = newPath;
		} else {
			std::cerr << "Error: path is not a directory" << std::endl;
		}
		return CWD_;
	}
}
