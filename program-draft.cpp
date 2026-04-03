#include <iostream>
#include <thread>
#include "photoReset.h"

/* Step by step draft of procedure:
 * 1) on first run, initiate photosettings constructor
 * 2) verify settings.toml is created before proceeding to run command
 * 3) take argv list
 * */

/* CLI command list
 * -b -> moveToDestination, takes 2 string path values. 
 * -a -> autoMove.
 * -m -> selectMove. takes multiple arguments from argv
 * -sd -> setDestination, takes 1 string path.
 * -sc -> setCWD, takes 1 string path.
 * */

int draftmain(int __argc, char** __argv) {
    
}
