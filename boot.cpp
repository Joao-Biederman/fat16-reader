#include "boot.h"

Boot_sector::Boot_sector(FILE* img)
{
    this->read_boot_sector(img);
}

Boot_sector::Boot_sector()
{
}

Boot_sector::~Boot_sector()
{
}

void Boot_sector::print_bytes_per_sector()
{
    cout << "bytes per sector: " << this->bytes_per_sector << endl;
    return;
}

void Boot_sector::print_reserved_sectors()
{
    cout << "reserved sectors: " << this->reserved_cluster_count << endl;
    return;
}

void Boot_sector::print_sectors_per_cluster()
{
    cout << "sectors per cluster: " << static_cast<int>(this->sectors_per_cluster) << endl;
    return;
}

void Boot_sector::print_num_fat()
{
    cout << "table count: " << static_cast<int>(this->table_count) << endl;
    return;
}

void Boot_sector::print_sectors_per_fat()
{
    cout << "sectors per fat: " << this->table_size_16 << endl;
    return;
}

void Boot_sector::print_root_entry_count()
{
    cout << "root entry count: " << static_cast<int>(this->root_entry_count) << endl;
    return;
}

void Boot_sector::read_boot_sector(FILE* img)
{
    FILE* boot = img;

    fseek(boot, 0, SEEK_SET);
    fread(this, sizeof(Boot_sector), 1, img);
}

void Boot_sector::print_core_infos()
{
    this->print_bytes_per_sector();
    this->print_reserved_sectors();
    this->print_sectors_per_cluster();
    this->print_num_fat();
    this->print_sectors_per_fat();
    this->print_root_entry_count();
}

int Boot_sector::get_bytes_per_sector()
{
    return static_cast<int>(this->bytes_per_sector);
}

int Boot_sector::get_cluster_per_fat()
{
    return static_cast<int>(this->table_size_16);
}

int Boot_sector::get_reserved_cluster_count()
{
    return static_cast<int>(this->reserved_cluster_count);
}

int Boot_sector::get_table_count()
{
    return static_cast<int>(this->table_count);
}

int Boot_sector::get_root_entry_count()
{
    return static_cast<int>(this->root_entry_count);
}

int Boot_sector::get_sector_per_cluster()
{
    return static_cast<int>(this->sectors_per_cluster);
}