#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <fstream>        //TODO: read txt file for settings
#include <system_error>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <vector>

namespace photo {
	class photoSorter {
		public:
			photoSorter();
			explicit photoSorter(const std::filesystem::path& root_directory);
			~photoSorter(); 

			//operating system constructor needed?
			//Not needed: std::filesystem:path handles path formats 

			std::string getFileName();
			std::string getFilePath();
			std::uint32_t getNumFiles();
			std::string getCWD();

			std::string getFilesInDirectory(std::filesystem::path& directory); //https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
			
			std::string switchCWD(const std::filesystem::path& workingDirectory); //main function to get folder/directory of photos that are stored
			std::string switchRoot(const std::string directorySwitch); 	//TODO: figure out how linux does root directory on a different drive that is not main.
							//EX: drive letter are used for root on Windows (C:, E:, etc)
			uint32_t setNumFiles();	//set number of files based off current_working_directory_
			int setCurrentListedFile(std::string fileName);
			int setCurrentListedFilePATH(std::filesystem::path directFilePath);

			int setFileList(); // sets the list of files in the current working directory

			//TODO: functions to actually start creating folders and moving files
			//Note: use void? or find some better way?
			void createDirectory(std::string dirName);	//dirName is meant to specify a sort type, i.e format, shot date, etc
			void movePhoto(std::filesystem::path& destination);
      		void movePhoto(std::filesystem::path& destination, std::string name);
			
			void printFileList() const {
				std::cout << "Files in directory: " << current_working_directory_.string() << std::endl;
				for (const auto& file : file_list) {
					std::cout << file.string() << std::endl;
				}
			}
			
		private:
			std::filesystem::path root_directory_;
			std::filesystem::path current_working_directory_;
			std::filesystem::path file_path_;
            std::filesystem::path folder_path_;             //when creating a new folder using createDirectory and needed to redirect to it

			std::string file_name;	
			std::atomic<std::uint32_t> num_files;	//number of files in a folder structure, directories also count as files
			int *file_position;	//thread specific file position
			
			std::atomic<bool> swap;
			std::mutex mtx;
			std::thread *thread_pool;

			//potential additions for filesystem
			//array of bools to represent a current directory structure that reflects changes
			//array of strings to represent list of files in a directory
			//private function to print out directory structure for CLI. will be changed if GUI application is made

			std::vector<std::filesystem::path> file_list;

		};
	};

