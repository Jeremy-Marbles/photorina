#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <system_error>
#include <stdexcept>
#include <thread>
#include <mutex>

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
			
			std::string switchCWD(const std::filesystem::path& workingDirectory); //main function to get folder/directory of photos that are stored
			std::string switchRoot(const std::string directorySwitch); 	//TODO: figure out how linux does root directory on a different drive that is not main.
							//EX: drive letter are used for root on Windows (C:, E:, etc)
			uint32_t setNumFiles();	//set number of files based off current_working_directory_
			int setCurrentListedFile(std::string fileName);

			//TODO: functions to actually start creating folders and moving files
			//Note: use void? or find some better way?
			void createDirectory(std::string dirName);	//dirName is meant to specify a sort type, i.e format, shot date, etc
			void movePhoto(/*add variables? or pull from private variables established*/);

		private:
			std::filesystem::path root_directory_;
			std::filesystem::path current_working_directory_;
			std::filesystem::path file_path_;

			std::string file_name;	//
			std::atomic<std::uint32_t> num_files;	//number of files in a folder structure, directories also count as files
			int *file_position;	//thread specific file position
			

			std::atomic<bool> swap;
			std::mutex mtx;
			std::thread *thread_pool;

			//potential additions for filesystem
			//array of bools to represent a current directory structure that reflects changes
			//private function to print out directory structure for CLI. will be changed if GUI application is made
		};
	};

