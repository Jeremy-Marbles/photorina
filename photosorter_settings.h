/* This file controls the settings for the photo sorter application
    TLDR: reads and writes settings to a configuration file
    TBD: 1) txt or cfg file format
    2) settings to be read and written
*/
#pragma once

#include "photo_sort.h"
#include <fstream>

class photosorter_settings : public photo::photoSorter
{
private:
    /* data */
public:
    photosorter_settings(/* args */);
    ~photosorter_settings();
};

