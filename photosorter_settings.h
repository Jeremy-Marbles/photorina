/* This file controls the settings for the photo sorter application
    TLDR: reads and writes settings to a configuration file
    TBD: 1) ~~txt~~ or cfg file format
    2) settings to be read and written
*/
#pragma once

#include "photo_sort.h"
#include <map>
#include <fstream>

namespace std {
  class photosorter_settings : public photo::photoSorter
  {
    private:
        string cfgName;
        map<string, string> settings;

        void loadSettings();
    public:
        photosorter_settings();
        ~photosorter_settings();

        int makeNewCFG(string name) {               //file name must include cfg  
            //Check if cfg file exists already
            filesystem::path currentFolder = filesystem::current_path() / name;

            if (filesystem::exists(currentFolder)) {
                cerr << "Error: config file already exists: " << currentFolder.string() << endl; 
                return -1;
            }
           
            ofstream cfgFile(name);
            if (!cfgFile) {
                cerr << "Error creating config file: " << name << endl;
                return -1; 
            }

            return 1;
        }

        int populateCfg();
        
    };  
}
