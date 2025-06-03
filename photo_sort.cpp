#include "photo_sort.h"
#include <cassert>
#include <cstdlib>

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
	bool existingPath = std::filesystem::exists(root_directory_, variableConstructEC); //exists returns bool. adding an error code variable sets a error print out, platform specific 
	if (variableConstructEC) {
		throw std::filesystem::filesystem_error(
			"Error verifying existence of specified root directory: " + root_directory_.string(), root_directory_, variableConstructEC
		);
	}

	if (!existingPath) {
		throw std::filesystem::filesystem_error ("Specified root directory does not exist: " + root_directory_.string(), root_directory_, std::make_error_code(std::errc::no_such_file_or_directory));
	}
	
	//if variable 
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
    std::string photoSorter::switchRoot(const std::string directorySwitch) {
	root_directory_ = directorySwitch;
	return root_directory_.string();
    }

    // Operations functions
    void photoSorter::createDirectory(std::string dirName) {
	//utilize directory::createDirectory command
    	//1) check if current path is CWD
	//1a) if not, force std::filesystem::current_path to current_working_directory_ and throw warning
	//2) call std::filesystem::create_directory for one new directory path
	//- Names for directory path should be either specified or chosen from options of items that need to be sorted i.e any metadata
    	bool inPath = std::filesystem::current_path(std::filesystem::temp_directory_path());
	if (current_working_directory_ != inPath) {
		current_working_directory_ = std::filesystem::current_path(std::filesystem::temp_directory_path);
		//TODO: throw warning that directory is not correct and CWD is now changed to reflect change

	}
	std::filesystem::create_directory(dirName);	
    }

    void photoSorter::movePhoto() {
    	
    }
}

