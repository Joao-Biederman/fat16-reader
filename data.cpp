#include "data.h"

void file83::print_time(short time)
{
    unsigned short hour = (time >> 11) & 0x1F;      // First 5 bits for hour
    unsigned short minute = ((time >> 5) & 0x3F);    // Next 6 bits for minute
    unsigned short second = time & 0x1F;             // Last 5 bits for second

    cout << hour << ":" << minute << ":" << second*2;
}

void file83::print_date(short date)
{
    unsigned short year = (date >> 12) & 0x7F;      // First 7 bits for year
    unsigned short month = ((date >> 8) & 0x0F);    // Next 4 bits for month
    unsigned short day = date & 0x1F;               // Last 5 bits for second

    cout << day << "/" << month << "/" << year;
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

    cout << "last access at ";
    print_date(this->last_access);
    cout << endl;
    
    cout << "first cluster at " << this->first_cluster_low << endl; 

    cout << "data type " << static_cast<int>(this->file_atributte) << endl;
}

int file83::get_data_type()
{
    return static_cast<int>(this->file_atributte);
}

int root_data::search_data(FILE* root)
{
    file83 newFile;
    fread(&newFile, 32, 1, root);

    if (newFile.get_data_type() == 0)
    {
        cout << "end of root dir data" << endl;
        return 0;
    }

    this->data_type = newFile.get_data_type();

    if (this->data_type != 15)
    {
        this->standard_8_3_file = newFile;
    }
    else
    {
        fseek(root, -sizeof(file83), SEEK_CUR);
        fread(&this->long_file_name, sizeof(long_file), 1, root);
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
        cout << "This is an archive" << endl;
        return &standard_8_3_file;
    }

    cout << endl << endl;
    return NULL;
}

int root_dir::add_files(FILE* img, int root_dir_start)
{
    FILE* fat_root = img;
    root_data new_file;

    fseek(fat_root, root_dir_start, SEEK_SET);
    
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
            cout << endl;
        }
        
    }
}