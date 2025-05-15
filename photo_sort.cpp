#include "photo_sort.h"

namespace photo {

    photoSorter::photoSorter() {
	//explicitly call for windows at start
	root_directory = "C:";
	num_files = 0;
	file_position = nullptr;
	file_path = "";
	swap = false;
    }   

    explicit photoSorter::photoSorter(const std::filesystem::path& root_directory) {
        root_directory_ = root_directory;
        num_files = 0;
        file_position = nullptr;
        file_path = "";
        swap = false;
    }
    
    photoSorter::~photoSorter() {

    }

    std::string photoSorter::getFileName(const std::filesystem::path& fileName) {
        return fileName.filename().string();
	
    }

    std::string photoSorter::getFilePath(const std::filesystem::path& pathName) {
	
    }
}

