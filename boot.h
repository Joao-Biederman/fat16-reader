#include <iostream>
#include <vector>

using namespace std;

class Boot_sector
{
private:
    unsigned char  bootjmp[3];
	unsigned char  oem_name[8];
	unsigned short bytes_per_sector;
	unsigned char  sectors_per_cluster;
	unsigned short reserved_cluster_count;
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
    int get_cluster_per_fat();
    int get_reserved_cluster_count();
    int get_table_count();
    int get_root_entry_count();
    int get_sector_per_cluster();

}__attribute__((packed));