#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <thread>
#include <mutex>

namespace photo {
	class photoSorter {
		public:
			photoSorter();
			explicit photoSorter(const std::filesystem::path& root_directory);
			~photoSorter(); 

			//operating system constructor needed?

			std::string getFileName(const std::filesystem::path& fileName);
			std::string getFilePath(const std::filesystem::path& pathName);

			std::uint32_t getNumFiles();
			
			
		private:
			std::filesystem::path root_directory_;
			std::string file_name;
			std::atomic<std::uint32_t> num_files;	//number of files in a folder structure, directories also count as files
			int *file_position;	//thread specific file position
			
			std::string file_path;

			std::atomic<bool> swap;
			std::mutex mtx;
			std::thread *thread_pool;

			/*struct operating_system_info {
				bool is_windows = false;
				bool is_linux = false;
				bool is_mac = false;

				std::string linux_root;
				std::string windows_folder;

				//TODO: include macOS folder

			};*/
	};
}
