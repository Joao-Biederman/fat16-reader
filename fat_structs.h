#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "boot.h"
#include "data.h"

using namespace std;

class FAT16
{
private:
    FILE* img;
    Boot_sector bs;
    root_dir root;
    vector<int> fat_in_sector;
    int root_dir_in_sector;
    int data_in_sector;

    void read_root();
public:
    FAT16();
    ~FAT16();

    void reed_FAT();
    void read_files();
    void print_img_infos();
};