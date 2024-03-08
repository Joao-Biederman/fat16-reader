#include "fat_structs.h"

FAT16::FAT16()
{
}

FAT16::~FAT16()
{
    cout << "Closing img" << endl;
    fclose(img);
}

void FAT16::reed_FAT()
{
    img = fopen("./fat16_1sectorpercluster.img", "rb");

    if (img == nullptr)
    {
        cerr << "error opening file" << endl;
        exit(1);
    }

    bs.read_boot_sector(img);

    bs.print_core_infos();
    
    fat_in_sector.push_back(bs.get_reserved_cluster_count());
    
    cout << "fat in sectors: " << fat_in_sector[0] << endl;

    for (int i = 1; i < bs.get_table_count(); i++)
    {
        fat_in_sector.push_back(fat_in_sector[i-1] + (bs.get_cluster_per_fat() * bs.get_sector_per_cluster()));
    }

    root_dir_in_sector = fat_in_sector[0] + (bs.get_table_count() * bs.get_cluster_per_fat());

    data_in_sector = root_dir_in_sector + ((bs.get_root_entry_count() * 32) / bs.get_bytes_per_sector());
}

void FAT16::read_files()
{
    root.add_files(img, (root_dir_in_sector * bs.get_bytes_per_sector()));
    root.read_files(this->fat_in_sector[0]);
}