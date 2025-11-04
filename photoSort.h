//Merge settings with photosort
//for filesystem metadata: either filesystem library or https://exiv2.org/

/*Photorina rewrite
 * - One of the problems with the original codebase was the generated try catch blocks for error handling,
 *   which responded with unformatted error logs. This rewrite seeks to streamline that, along with
 *   removing redundant class items to compact the code and allowing more sophisticated systems.*/

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
#include <vector>
#include <queue>
#include <functional>
#include <cassert>
#include <cstdlib>
#include <fstream>
//include https://github.com/benhoyt/inih for ini parsing

//TODO: global error handling
//ie for missing file vs bad file
//or bad directory (which shouldn't be possible)

namespace photo {
	class photoSort_settings {

			//std::map<std::string, std::string> settings;
			//considerations for settings:
			//https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
			//- set default working directory
			//- set destination folder
            //- set file format to be sorted (e.g., .jpg, .png, .raw)
			//- set raw formats to be sorted (e.g., .cr2, .nef, .arw)
			//- set camera model filters (Sony, Canon, Nikon, etc)
			//- set date range filters
		
		public:			
			std::string cfgName;
			std::ofstream cfgFile;
			std::filesystem::path cfgPath = "settings.ini";

			photoSort_settings();
			~photoSort_settings();

			int loadSettings(int command);
			int modifySettings(int command);

			int makeNewCfg(std::string name);
			int populateCfg(int command);
	};

	class photoSort {
		private:
			std::filesystem::path root_; //not const: assumes multi drive set ups
			std::filesystem::path CWD_;

			std::vector<std::filesystem::path> directory_CWD; 
			
		public:
			photoSort();
			~photoSort();

			void printFileList() const {
				std::cout << "Files in Directory " << CWD_.string() << ":" << std::endl;

				for (const auto& file : directory_CWD) {
					std::cout << file.string() << std::endl;
				}
			}

			int populateCWD_Vector();
			void singleMove(std::filesystem::path des, int vec_pos);
			//TODO: set up singleMove for file name in vector
			//in the future 
			//void singleMove(std::filesystem::path des, std::string fileName);
			
			void multiMove(std::filesystem::path toMove, int command);
			
			std::filesystem::path setFolder(std::filesystem::path newPath);
			std::filesystem::path setCWD_(std::string folder);
			std::filesystem::path createDir(std::string name);

		protected:
			photoSort_settings sortSettings_ = photoSort_settings();
	};	
}
