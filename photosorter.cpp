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

/* Notes:
 * -filesystem::path takes either 1) file or 2) directory, 
 * so generally no need for a file name variable that is only string based. 
 * -path operator = takes either a path format object or a basic string object towards a path name 
 * -file_type detects whether a file or directory is given by a path object.
 *
 * TODO: figure out if filesystem library can create directories and if it uses a distinct command
 * Also TODO: Sketch logic for inserting a new directory into the CWD for photos
 * */

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
	//TODO: Make test functions for use on main
	namespace photo {
		const std::filesystem::path userDirectory = "C:\";
		photoSorter newOrganizer;
		newOrganizer.switchRoot(userDirectory);
		
		const std::filesystem::path photoFolder = "C:\users\marbl\photos";
		newOrganizer.switchCWD(photoFolder);

		std::string printCWD = newOrganizer.getCWD();
		printf("%s", printCWD);

		//std::string fileName = newOrganizer.getFileName();
		std::atomic<uint32_t> fileNumber = newOrganizer.getNumFiles();
	}
	return 0;
}


