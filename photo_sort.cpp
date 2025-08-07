#include "photo_sort.h"
#include <cassert>
#include <cstdlib>

namespace photo {
    // Constructors
    photoSorter::photoSorter() {
		root_directory_ = std::filesystem::path();
		num_files = 0;
		file_position = nullptr;
		file_path_ = "";
		swap = false;

		std::error_code constructEC;
		const auto currentPath = std::filesystem::current_path(constructEC);	//current_path() handles search, returns a _win_error if path cannot be found;

		if (constructEC) {
			throw std::filesystem::filesystem_error("Failed to get current working directory.", constructEC);
		};

		root_directory_ = currentPath;

    }   

    photoSorter::photoSorter(const std::filesystem::path& root_directory) {
        root_directory_ = root_directory;
        num_files = 0;
        file_position = nullptr;
        file_path_ = "";
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
			throw std::filesystem::filesystem_error (
				"Specified root directory does not exist: " + root_directory_.string(), root_directory_, std::make_error_code(std::errc::no_such_file_or_directory));
		}
		
		//if variable is not empty, check if it is a directory
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

    // Getters
    std::string photoSorter::getFileName() {
        return file_name;
    }

    std::string photoSorter::getFilePath() {
        return file_path_.string();
    }

    std::uint32_t photoSorter::getNumFiles() {
		return num_files.load();
    }

	std::string photoSorter::getRoot()  {
		return root_directory_.string();
	}

    std::string photoSorter::getCWD() {
		return current_working_directory_.string();
    }

	std::filesystem::path photoSorter::getNewFolder() {
		return folder_path_;
	}

	std::string photoSorter::getFilesInDirectory(std::filesystem::path& directory) {
		for (const auto & entry : std::filesystem::directory_iterator(directory)) {
			std::cout << entry.path().string() << std::endl;
		}

		return "Completed listing files in directory: " + directory.string();
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

	//TODO: find a better way to handle folder switching
	/*std::filesystem::path photoSorter::switchFolder() {

		return folder_path_;
	}*/

		//sets the number of files in the current working directory only
	uint32_t photoSorter::setNumFiles() {
		num_files = std::distance(std::filesystem::directory_iterator(current_working_directory_), std::filesystem::directory_iterator());
		return num_files;
	}

	int photoSorter::setCurrentListedFile(std::string fileName) {
		try
		{
			//NOTES: The / operator for filesystem::path handles path separators correctly
			// Construct the full path by joining the current working directory and the file name
			std::filesystem::path potentialFilePath = current_working_directory_ / fileName;
			
			//std::cout << "PFP: " << potentialFilePath.string() << std::endl;

			std::error_code potentialFileNF;
			if (!std::filesystem::exists(potentialFilePath, potentialFileNF)) {
				if (potentialFileNF) {	//os error occurred
					throw std::filesystem::filesystem_error(
					"Error checking existence of file: " + potentialFilePath.string(),
					potentialFilePath,
					potentialFileNF);
					std::cout << std::endl;
				}
				throw std::filesystem::filesystem_error( //file 100% does not exist
					"File does not exist: " + potentialFilePath.string(),
					potentialFilePath,
					std::make_error_code(std::errc::no_such_file_or_directory));
					std::cout << std::endl;
			}

			file_path_ = potentialFilePath;
			file_name = file_path_.filename().string();
			//std::cout << "FP: " << file_path_.string() << std::endl;
		}
		catch(const std::filesystem::filesystem_error& CLF_Error)
		{
			std::cerr << "Filesystem Error in setCurrentListedFile: " << CLF_Error.what() << std::endl;
			if (!CLF_Error.path1().empty()) {
				std::cerr << " Path1: " << CLF_Error.path1().string() << std::endl;
			}
			if (!CLF_Error.path2().empty()) {
				std::cerr << " Path2: " << CLF_Error.path2().string() << std::endl;
			}
			return 0;
		}
		catch(const std::exception& e)
		{
			std::cerr << "Error in setCurrentListedFile:" << e.what() << std::endl;
			return 0;
		}

		return 1;
	}

	int photoSorter::setCurrentListedFilePATH(std::filesystem::path fullPath) {
		try
		{
			std::error_code CLFPathErr;
			if (!std::filesystem::exists(fullPath, CLFPathErr)) {
				if (CLFPathErr) {
					throw std::filesystem::filesystem_error(
						"Error checking existence of path: " + fullPath.string(),
						fullPath,
						CLFPathErr);
				}
				throw std::filesystem::filesystem_error(
					"Specified file path does not exist: " + fullPath.string(),
					fullPath,
					std::make_error_code(std::errc::no_such_file_or_directory));
			}

			file_path_ = fullPath;
		}
		catch(const std::filesystem::filesystem_error& CLFPathErr) 
		{
			std::cerr << "Filesystem Error in setCurrentListedFilePATH: " << CLFPathErr.what() << std::endl;
			if (!CLFPathErr.path1().empty()) {
				std::cerr << " Path1: " << CLFPathErr.path1().string() << std::endl;
			}
			if (!CLFPathErr.path2().empty()) {
				std::cerr << " Path2: " << CLFPathErr.path2().string() << std::endl;
			}
			return 0;
		}
		catch(const std::exception& e)
		{
			std::cerr << "Error in setCurrentListedFilePATH: " << e.what() << '\n';
			return 0;
		}
		
		return 1;
	}

	int photoSorter::setFileList() {
		file_list.clear();

		try 
		{
			std::error_code listSetEC;
			if (!std::filesystem::is_directory(current_working_directory_, listSetEC) || listSetEC) {
				if (listSetEC) {
					throw std::filesystem::filesystem_error(
						"Error accessing current working directory",
						current_working_directory_,
						listSetEC
					);
				}
				throw std::filesystem::filesystem_error(
					"Current working directory is not a valid directory",
					current_working_directory_,
					std::make_error_code(std::errc::not_a_directory)
				);
			}
			
			for (const auto& entry : std::filesystem::directory_iterator(current_working_directory_)) {
				if (entry.is_regular_file()) {
					file_list.push_back(entry.path());
				}
			}
		} catch (const std::filesystem::filesystem_error& newErr) {
			std::cerr << "Error populating file list: " << newErr.what() << std::endl;
			if (!newErr.path1().empty()) {
				std::cerr << " Path: " << newErr.path1().string() << std::endl;
			}
			file_list.clear();
		}
		
		return file_list.size();
	}

    // Operations functions
    void photoSorter::createDirectory(std::string dirName) {
	//utilize directory::createDirectory command
    //1) check if current path is CWD
	//2) call std::filesystem::create_directory for one new directory path
	//- Names for directory path should be either specified or chosen from options of items that need to be sorted i.e any metadata

		std::filesystem::path newDirPath = current_working_directory_ / dirName;
		try 
		{
			if (std::filesystem::create_directory(newDirPath)) {
				std::cout << "Directory created successfully: " << newDirPath.string() << std::endl;
				folder_path_ = newDirPath; // Update folder_path_ to the newly created directory
			} else {
				std::cout << "Directory already exists or could not be created: " << newDirPath.string() << std::endl;
				folder_path_ = newDirPath;
			}
		} catch (const std::filesystem::filesystem_error& e)
		{
			std::cerr << "Error creating directory: " << e.what() << std::endl;
			if (!e.path1().empty()) {
				std::cerr << " Path1: " << e.path1().string() << std::endl;
			}
			if (!e.path2().empty()) {
				std::cerr << " Path2: " << e.path2().string() << std::endl;
			}
		}
    }

	void photoSorter::movePhoto(std::filesystem::path& destination) {	
		//this command assumes a set condition was made for what files are specified, EX. given file_name_ in the private variables
    	if (file_path_.empty()) {
        	std::cerr << "Error: No file has been set to move. Set current file using setCurrentListedFile()";
        	return;
      	}

      	try
      	{
        	std::error_code moveFileErr;
        	if(!std::filesystem::is_directory(destination, moveFileErr)){
          		if (moveFileErr) {
            		throw std::filesystem::filesystem_error("Error checking destination path", destination, moveFileErr);
          	}
          	throw std::filesystem::filesystem_error("Destination is not a valid directory", destination,
            	std::make_error_code(std::errc::not_a_directory));
        	}

        	const auto destinationPath = destination / file_path_.filename();
        	std::filesystem::rename(file_path_, destinationPath, moveFileErr);

			if (moveFileErr) {
				throw std::filesystem::filesystem_error("Failed to move file", file_path_, destinationPath, moveFileErr);
			}

			std::cout << "Moved: '" << file_path_.string() << "' to '" << destinationPath.string() << "'" << std::endl;

			file_path_.clear();
			file_name.clear();
        
        	//todo: use setFileList to match actual directory structure
        	setFileList();
      	} catch (const std::filesystem::filesystem_error& newErr) {
			std::cerr << "Error moving file: " << newErr.what() << std::endl;
			if (!newErr.path1().empty()) { std::cerr << " Source: " << newErr.path1().string() << std::endl; }
			if (!newErr.path2().empty()) { std::cerr << " Destination: " << newErr.path2().string() << std::endl; }
		}
    }

	void photoSorter::movePhoto(std::filesystem::path& destination, std::string name) {
		//debug: std::cout << "movePhoto: " << sourcePath.string() << " to " << destination.string() << std::endl;
		try {
      		const auto sourcePath = current_working_directory_ / name;
      		std::error_code moveDestErr;

      		if (!std::filesystem::exists(sourcePath, moveDestErr)) {
        		if (moveDestErr) {
          		throw std::filesystem::filesystem_error("Error checking existance of source file.", sourcePath, moveDestErr);
        		}
        	throw std::filesystem::filesystem_error("Source file does not exist.", sourcePath, std::make_error_code(std::errc::no_such_file_or_directory));
			}
      
			if (!std::filesystem::is_directory(destination, moveDestErr)) {
				if (moveDestErr) {
				throw std::filesystem::filesystem_error("Error checking existance of destination path.", destination, moveDestErr);
				}
				throw std::filesystem::filesystem_error("Destination is not a valid directory.", destination, std::make_error_code(std::errc::not_a_directory));
			}

			const auto destinationPath = destination / name;
			std::filesystem::rename(sourcePath, destinationPath, moveDestErr);

			if (moveDestErr) {
				throw std::filesystem::filesystem_error("Failed to move file. ", sourcePath, destinationPath, moveDestErr); 
			}

			std::cout << "Successfully moved '" << sourcePath.string() << "' to '" << destinationPath.string() << "'" << std::endl;

		} catch (const std::filesystem::filesystem_error& newError) {
			std::cerr << "Error moving file: " << newError.what() << std::endl;
			if (!newError.path1().empty()) { std::cerr << " Source: " << newError.path1().string() << std::endl; }
			if (!newError.path2().empty()) { std::cerr << " Destination: " << newError.path2().string() << std::endl; }
		}
	}

	std::filesystem::path photoSorter::fileListSpot(int position) {
		if (position < 0 || position >= static_cast<int>(file_list.size())) {
			throw std::out_of_range("Position is out of range of the file list.");
		}
		return file_list[position];
	}

	void photoSorter::multiTransfer() {
		
	}
}

