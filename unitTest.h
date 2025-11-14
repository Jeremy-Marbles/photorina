//Unit tests for photoSort
#pragma once
#include "photoSort.h"

class unitTest {
    public:
        unitTest(int test) {
            if (test == 0) {

            }
        }

	    //photoSort settings constructor test
        int PSS_unitTest_1() {
        	//TODO: test creating settings.toml and test toml++ parsing
		    photo::photoSort_settings createSet;
            

            return 0;
        }

        //photoSort constructor test
        int PS_unitTest_1() {
            photo::photoSort setOne;

            return 0;
        }
};
