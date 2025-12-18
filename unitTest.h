//Unit tests for photoSort
#pragma once
#include "photoSort.h"

class unitTest {
    public:
        unitTest() {
            PSS_unitTest_0();


        }

	    //photoSort settings constructor test
        int PSS_unitTest_0() {
        	//TODO: test creating settings.toml and test toml++ parsing
		    
            //pt. 1: create photoSort_settings object when file is missing.
            photo::photoSort_settings createSet;

            return 0;
        }

        int PSS_unitTest_1() {
            //pt. 2: load settings from existing settings.toml, output some values to verify
            photo::photoSort_settings loadSet;

            toml::table configTable;
            try {
                configTable = toml::parse_file(loadSet.cfgName);
            } catch (const toml::parse_error& err) {
                std::cerr << "Parsing error in unit test: " << err.description() << "\n";
                std::cerr << "At " << err.source().begin << "\n";
                return -1;
            }

            return 0;
        }

        //photoSort constructor test
        int PS_unitTest_0() {
            photo::photoSort setOne;
            
            
            return 0;
        }
};
