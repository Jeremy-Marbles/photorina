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

			std::string getFileName(const std::filesystem::path& fileName);
			std::string getFilePath(const std::filesystem::path& pathName);

			std::uint32_t getNumFiles();

			//TODO: add mutator functions to set the root directory, current working directory
			std::string switchCWD(const std::filesystem::path& workingDirectory); //main function to get folder/directory of photos that are stored
			//std::string switchRoot(); 	//TODO: figure out how linux does root directory on a different drive that is not main.
							//EX: drive letter are used for root on Windows (C:, E:, etc)

			//TODO: functions to actually start creating folders and moving files
			//Note: use void? or find some better way?
			void createDirectory();
			void movePhoto(/*add variables? or pull from private variables established*/);

		private:
			std::filesystem::path root_directory_;
			std::filesystem::path current_working_directory_;
			std::string file_name;
			std::atomic<std::uint32_t> num_files;	//number of files in a folder structure, directories also count as files
			int *file_position;	//thread specific file position
			
			std::string file_path;

			std::atomic<bool> swap;
			std::mutex mtx;
			std::thread *thread_pool;
		};
	};
}
