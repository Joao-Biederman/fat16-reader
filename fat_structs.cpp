#include "fat_structs.h"

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

void Boot_sector::print_bytes_per_sector(){
    cout << "bytes per sector: " << this->bytes_per_sector << endl;
    return;
}

void Boot_sector::print_reserved_sectors(){
    cout << "reserved sectors: " << this->reserved_sector_count << endl;
    return;
}

void Boot_sector::print_sectors_per_cluster(){
    cout << "sectors per cluster: " << static_cast<int>(this->sectors_per_cluster) << endl;
    return;
}

void Boot_sector::print_num_fat(){
    cout << "table count: " << static_cast<int>(this->table_count) << endl;
    return;
}

void Boot_sector::print_sectors_per_fat(){
    cout << "sectors per fat: " << this->table_size_16 << endl;
    return;
}

void Boot_sector::print_root_entry_count(){
    cout << "root entry count: " << static_cast<int>(this->root_entry_count) << endl;
    return;
}

void Boot_sector::read_boot_sector(FILE* img){
    FILE* boot = img;

    fseek(boot, 0, SEEK_SET);
    fread(this, sizeof(Boot_sector), 1, img);
}

void Boot_sector::print_core_infos(){
    this->print_bytes_per_sector();
    this->print_reserved_sectors();
    this->print_sectors_per_cluster();
    this->print_num_fat();
    this->print_sectors_per_fat();
    this->print_root_entry_count();
}

int Boot_sector::get_bytes_per_sector(){
    return static_cast<int>(this->bytes_per_sector);
}

int Boot_sector::get_sector_per_fat(){
    return static_cast<int>(this->table_size_16);
}

int Boot_sector::get_reserved_sector_count(){
    return static_cast<int>(this->reserved_sector_count);
}

int Boot_sector::get_table_count(){
    return static_cast<int>(this->table_count);
}

file83::file83()
{
}

file83::~file83()
{
}

long_file::long_file(/* args */)
{
}

long_file::~long_file()
{
}

root_data::root_data()
{
}

root_data::~root_data()
{
}

int root_data::get_data(FILE* img)
{
    FILE* type = img;
    if (fseek(type, 11, SEEK_CUR) != 0){
        cerr << "Error looking for type" << endl;
        fclose(type);
        return 0;
    }

    char data_type;
    fread(&data_type, 1, 1, type);
    fclose(type);

    if (static_cast<int>(data_type) == 0)
    {
        cout << "end of root dir data" << endl;
        return 0;
    }

    this->data_type = static_cast<int>(data_type);    

    if (data_type != 15)
    {
        fread(&this->standard_8_3_file, sizeof(file83), 1, img);
    }
    else
    {
        fread(&this->long_file_name, sizeof(long_file), 1, img);
    }
    
    return 1;
}

void root_data::read_file()
{
    if (this->data_type == 2)
        cout << "This is a Hidden file";

    if (this->data_type == 4)
        cout << "You found a system file";
    
    if (this->data_type == 8)
        cout << "you found your volume ID";

    if (this->data_type == 15)
        cout << "This is a long file name type of file";

    

    cout << endl << endl;
}

root_dir::root_dir()
{
}

root_dir::~root_dir()
{
}

int root_dir::add_file(FILE* img)
{
    root_data new_file;
    while(new_file.get_data(img))
    {
        this->files.push_back(new_file);
    }
}

void root_dir::read_files()
{
    for (int i = 0; i < this->files.size(); i++)
    {
        this->files[i].read_file();
    }
}

FAT16::FAT16()
{
    img = fopen("./fat16_1sectorpercluster.img", "rb");

    if (img == nullptr)
    {
        cerr << "error opening file" << endl;
        exit(1);
    }

    bs.read_boot_sector(img);

    bs.print_core_infos();
    
    fat_in_sector.push_back(bs.get_reserved_sector_count());

    for (int i = 1; i < bs.get_table_count(); i++)
    {
        fat_in_sector.push_back(fat_in_sector[i-1] + bs.get_sector_per_fat());
    }
}

FAT16::~FAT16()
{
    fclose(img);
}