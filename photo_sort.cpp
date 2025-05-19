#include "photo_sort.h"

namespace photo {

    // Constructors
    photoSorter::photoSorter() {
        root_directory_{};
        num_files = 0;
        file_position = nullptr;
        file_path = "";
        swap = false;

	std::error_code constructEC;
	const auto currentPath = std::filesystem::current_path(constructEC);	//current_path() handles search, returns a _win_error if path cannot be found;

	if (constructEC) {
		throw std::filesystem::filesystem_error("Failed to get current working directory.", ec);
	};

	root_directory_ = currentPath;

    }   

    explicit photoSorter::photoSorter(const std::filesystem::path& root_directory) {
        root_directory_ = root_directory;
        num_files = 0;
        file_position = nullptr;
        file_path = "";
        swap = false;
	if (root_directory_.empty()) {
		throw std::invalid_argument("Root directory path cannot be empty.");
	}

	std::error_code variableConstructEC;
	bool existingPath = std::filesystem::exists(root_directory_, ec);
	if (variableConstructEC) {
		throw std::filesystem::filesystem_error(
			"Error verifying existence of specified root directory: " + root_directory_.string(), root_directory_, variableConstructEC
		);
	}

	if (!path_exists) {
		throw std::filesystem::filesystem_error ("Specified root directory does not exist: " + root_directory_.string(), root_directory_, std::make_error_code(std::errc::no_such_file_or_directory));
	}
	
	bool isDir = std::filesystem::is_directory(root_directory_, variableConstructEC);
	if (variableConstructEC) {
		throw std::filesystem::filesystem_error(
			"Error checking specified root path is a directory: " + root_directory_.string(), root_directory_, variableConstructEC
		);
	}

	if(!isDir) {
		throw std::filesystem::filesystem_error(
			"Specified root path is not a directory: " + root_directory_.string(), root_directory_, std::make_error_code(std::errc::not_a_directory));
	}
    }
    
    // Destructor
    photoSorter::~photoSorter() {
      	delete file_position;
    }

    //TODO: add error handling for all functions below

    // Getters
    std::string photoSorter::getFileName() {
        return file_name;
	
    }

    std::string photoSorter::getFilePath() {
        return file_path_.string();
    }

    std::atomic<uint32_t> photoSorter::getNumFiles() {
	return num_files;
    }

    std::string photoSorter::getCWD() {
	return current_working_directory_.string();
    }

    // Setters
    std::string photoSorter::switchCWD(const std::filesystem::path& workingDirectory) {
	current_working_directory_ = workingDirectory;
	return current_working_directory_.string();
    }

    //This function will assume that directory to be worked in is on a different hard drive 
    std::string photoSorter::switchRoot() {
	
    }

    // Operations functions
    void photoSorter::createDirectory(std::string dirName) {

    }

    void photoSorter::movePhoto() {

    }
}

