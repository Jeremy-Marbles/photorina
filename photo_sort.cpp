#include "photo_sort.h"

namespace photo {

    photoSorter::photoSorter() {

    }   

    explicit photoSorter::photoSorter(const std::filesystem::path& root_directory) {
        root_directory_ = root_directory;
        num_files = 0;
        file_position = nullptr;
        file_path = "";
        swap = false;
    }

    std::string photoSorter::getFileName(const std::filesystem::path& fileName) {
        return fileName.filename().string();

    }
}

