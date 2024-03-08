#include <iostream>
#include <vector>

using namespace std;

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

    void print_time(short time);
    void print_date(short date);
public:
    void read_file();
    int get_data_type();

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

}__attribute__((packed));

class root_data
{
private:
    int data_type;
    file83 standard_8_3_file;
    long_file long_file_name;

public:
    int search_data(FILE* img);
    int get_data_type();

    file83* get_file();
};

class root_dir
{
private:
    vector<root_data> files;
public:
    int add_files(FILE* img, int root_dir_start);
    void read_files(int fat_sector);
    
};