/*
### "Photorina": Photo-sorter project by Jeremy Wong
# This project is a photo sorting application that organizes photos into folders based on their metadata.
# It uses C++17 and the C++ filesystem library for file handling and threading.
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
 * 
 * TODO: fstream library integration for txt settings/config file
 * */

#include "photo_sort.h"

#include <iostream>
#include <string>
#include <atomic>
#include <cstdint>
#include <thread>
#include <mutex>
#include <filesystem> //https://en.cppreference.com/w/cpp/filesystem

int unitTest_constructor() //test 1 passed
{
	const std::filesystem::path userDirectory = "C:\\";
	photo::photoSorter newOrganizer ( userDirectory );
	//newOrganizer.switchRoot(userDirectory);

	const std::filesystem::path photoFolder = "C:\\users\\marbl\\pictures\\";
	newOrganizer.switchCWD(photoFolder);

	std::string printCWD = newOrganizer.getCWD();
	printf("CWD: %s \n", printCWD.c_str());

	//std::string fileName = newOrganizer.getFileName();
	newOrganizer.setNumFiles();
	std::atomic<uint32_t> fileNumber = newOrganizer.getNumFiles();
	std::cout << " # files in directory: " << fileNumber.load() << std::endl;

	std::string path = newOrganizer.getCWD();
	for (const auto & entry : std::filesystem::directory_iterator(path)) {
		std::cout << entry.path() << std::endl;

	}

	return 0;
}

int unitTest_getters() //test 2 complete
{
	const std::filesystem::path mainDirectory = "C:\\";
	photo::photoSorter newOrganizer2 ( mainDirectory );

	const std::filesystem::path photoFolder = "C:\\users\\marbl\\pictures\\";
	newOrganizer2.switchCWD(photoFolder);
	//std::cout << "File path: " << newOrganizer2.getFilePath() << std::endl;
	
	newOrganizer2.setNumFiles();
	std::atomic<uint32_t> fileNumber = newOrganizer2.getNumFiles();
	
	std::cout << "# files: " << fileNumber.load() << std::endl;
	std::cout << "CWD: " << newOrganizer2.getCWD() << std::endl;
	
	// Test getFileName 
	newOrganizer2.setCurrentListedFile("DSC01803.JPG");	//FIXME:
	std::cout << "Current listed file path: " << newOrganizer2.getFilePath() << std::endl;

	// Test setCurrentListedFile error
	//newOrganizer2.setCurrentListedFile("nonexistent_file.jpeg");
	//std::cout << "File path: " << newOrganizer2.getFilePath() << std::endl;
	
	return 0;
}

int unitTest_operators() //test 3
{
	const std::filesystem::path mainDirectory = "C:\\";
	photo::photoSorter newOrganizer3 ( mainDirectory );
	const std::filesystem::path testLibrary = "C:\\users\\marbl\\desktop\\photorina test folder\\";
	newOrganizer3.switchCWD(testLibrary);
	std::cout << "1. " << newOrganizer3.getCWD() << std::endl;
	newOrganizer3.setNumFiles();

  	newOrganizer3.createDirectory("genTest");
    
  	//Add a print out for all files in the directory
	return 0;
}

int main(int argc, char *argv[])
{
	/*
	const std::filesystem::path userDirectory = "C:\\";
	photo::photoSorter newOrganizer ( userDirectory );
	//newOrganizer.switchRoot(userDirectory);

	const std::filesystem::path photoFolder = "C:\\users\\marbl\\pictures\\";
	newOrganizer.switchCWD(photoFolder);

	std::string printCWD = newOrganizer.getCWD();
	printf("CWD: %s \n", printCWD.c_str());

	//std::string fileName = newOrganizer.getFileName();
	newOrganizer.setNumFiles();
	std::atomic<uint32_t> fileNumber = newOrganizer.getNumFiles();
	std::cout << " # files in directory: " << fileNumber.load() << std::endl;
	*/

	//unitTest_constructor();
	//unitTest_getters();
	unitTest_operators();

	return 0;
}


