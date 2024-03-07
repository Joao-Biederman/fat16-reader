#include "fat_structs.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    FAT16 fat;    

    fat.reed_FAT();
    fat.read_files();

    return 0;
}
