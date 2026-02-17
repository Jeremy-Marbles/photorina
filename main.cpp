#include <iostream>
#include "photoReset.h"

int main() {

    photo::photoSettings newSort;

    toml::table settingsTable = toml::parse_file("settings.toml");

    std::cout << settingsTable << std::endl;

    //TODO: test setMetadata function
    

    return 0;
}


