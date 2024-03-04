#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

class Boot_sector
{
private:
    unsigned char  bootjmp[3];
	unsigned char  oem_name[8];
	unsigned short bytes_per_sector;
	unsigned char  sectors_per_cluster;
	unsigned short reserved_sector_count;
	unsigned char  table_count;
	unsigned short root_entry_count;
	unsigned short total_sectors_16;
	unsigned char  media_type;
	unsigned short table_size_16;
	unsigned short sectors_per_track;
	unsigned short head_side_count;
	unsigned int   hidden_sector_count;
	unsigned int   total_sectors_32;
 
	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	unsigned char  extended_section[54];

    void print_bytes_per_sector(); 
    void print_reserved_sectors();
    void print_sectors_per_cluster();
    void print_num_fat();
    void print_sectors_per_fat();
    void print_root_entry_count();

public:
    Boot_sector();
    Boot_sector(FILE* img);
    ~Boot_sector();

    void read_boot_sector(FILE* img);
    void print_core_infos();

    int get_bytes_per_sector();
    int get_sector_per_fat();
    int get_reserved_sector_count();
    int get_table_count();

}__attribute__((packed));

class file83
{
private:
    unsigned char  file_name[11];
    unsigned char  file_atributte;
    unsigned char  winNT;
    unsigned char  creation_time;
    unsigned short time_of_creation;
    unsigned short date_of_creation;
    unsigned short last_access;
    unsigned short first_cluster_high;
    unsigned short last_mod_time;
    unsigned short last_mod_date;
    unsigned short first_cluster_low;
    unsigned int   size;

public:
    file83();
    ~file83();
}__attribute__((packed));

class long_file
{
private:
    unsigned char  order;
    unsigned short first_char[5];
    unsigned char  file_atributte;
    unsigned char  long_entry_type;
    unsigned char  checksum;
    unsigned short middle_char[6];
    unsigned short always_zero;
    unsigned short last_char[2];

public:
    long_file(/* args */);
    ~long_file();
}__attribute__((packed));

class root_data
{
private:
    int data_type;
    file83 standard_8_3_file;
    long_file long_file_name;

public:
    root_data();
    ~root_data();

    int get_data(FILE* img);
    int get_data_type();
    void read_file();
};

class root_dir
{
private:
    vector<root_data> files;
public:
    root_dir();
    ~root_dir();

    int add_file(FILE* img);
    void read_files();
};

class FAT16
{
private:
    FILE* img;
    Boot_sector bs;
    vector<int> fat_in_sector;
    int root_dir_in_sector;
    int data_in_sector;

public:
    FAT16();
    ~FAT16();

    void read_root();
    void read_file();
};