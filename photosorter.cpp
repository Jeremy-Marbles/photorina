/*
### Project PhotoSorter by Jeremy Wong
# This project is a photo sorting application that organizes photos into folders based on their metadata.
# It uses C++17 and the C++ standard library for file handling and threading.
# How to use:
# 1. Specify the directory containing the photos to be sorted.
# 2. The program will scan the directory and its subdirectories for image files.
# 3. It will create folders based on the metadata of the images that is specified by the user (e.g., date taken, image format, or camera model).
# 4. The images will be moved into the corresponding folders.
#
*/
#include "photo_sort.h"

#include <iostream>
#include <string>
#include <atomic>
#include <cstdint>
#include <thread>
#include <mutex>
#include <filesystem> //https://en.cppreference.com/w/cpp/filesystem

int main(int argc, char *argv[])
{
	return 0;
}
