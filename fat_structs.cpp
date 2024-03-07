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

file83::file83()
{
}

file83::~file83()
{
}

void file83::print_time(short time)
{
    unsigned char hour = (time >> 11) & 0x1F;      // First 5 bits for hour
    unsigned char minute = ((time >> 5) & 0x3F);    // Next 6 bits for minute
    unsigned char second = time & 0x1F;             // Last 5 bits for second

    cout << static_cast<int>(hour) << ":" << static_cast<int>(minute) << ":" << static_cast<int>(second);
}

void file83::print_date(short date)
{
    unsigned char year = (date >> 11) & 0x1F;      // First 5 bits for hour
    unsigned char month = ((date >> 5) & 0x3F);    // Next 6 bits for minute
    unsigned char day = date & 0x1F;             // Last 5 bits for second

    cout << static_cast<int>(day) << "/" << static_cast<int>(month) << "/" << static_cast<int>(year);
}

void file83::read_file()
{
    cout << "file " << this->file_name << endl;

    cout << "created in ";
    print_date(date_of_creation);
    cout << " at ";
    print_time(time_of_creation);
    cout << endl;

    cout << "modified in ";
    print_date(last_mod_date);
    cout << " at ";
    print_time(last_mod_time);
    cout << endl;

    cout << "last access at " << this->last_access << endl;
    cout << "first cluster at " << this->first_cluster_low << endl; 
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

int root_data::search_data(FILE* img)
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

file83* root_data::get_file()
{
    if (this->data_type == 2)
        cout << "This is a Hidden file";

    if (this->data_type == 4)
        cout << "You found a system file";
    
    if (this->data_type == 8)
        cout << "you found your volume ID";

    if (this->data_type == 15)
        cout << "This is a long file name type of file";

    if (this->data_type == 16)
        cout << "This is a directory";

    if (this->data_type == 32)
    {
        cout << "This is an archive";
        return &standard_8_3_file;
    }

    cout << endl << endl;
    return NULL;
}

root_dir::root_dir()
{
}

root_dir::~root_dir()
{
}

int root_dir::add_files(FILE* img, int root_dir_start)
{
    FILE* fat_root = img;
    root_data new_file;

    fseek(fat_root, root_dir_start, SEEK_CUR);
    
    while(new_file.search_data(fat_root))
    {
        this->files.push_back(new_file);
    }
    return 1;
}

void root_dir::read_files(int fat_sector)
{
    for (int i = 0; i < this->files.size(); i++)
    {
        file83* archive = this->files[i].get_file();

        if (archive)
        {
            archive->read_file();
        }
        
    }
}

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
    
}