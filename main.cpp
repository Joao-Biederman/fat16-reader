#include "fat_structs.h"
#include <stdio.h>

using namespace std;

int main(int argc, char const *argv[])
{
    string filename = "fat16_1sectorpercluster.img";
    FAT16 fat(filename);

    return 0;
}
