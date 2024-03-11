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

void file83::print_infos()
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

    cout << "total size in bytes " << this->size << endl;

    cout << "first cluster at " << this->first_cluster_low << endl;
}

void file83::read_file(FILE* img, int fat_in_bytes, int data_in_bytes, int bytes_per_sector, int sectors_per_cluster)
{
    if (this->file_name[0] == 229)
        return;
    
    int size_of_a_cluster = bytes_per_sector*sectors_per_cluster;
    double clusters_to_read = static_cast<double>(this->size)/size_of_a_cluster;

    unsigned short cluster[int(ceil(clusters_to_read))];
    cluster[0] = this->first_cluster_low;
    int i = 0;
    if (clusters_to_read > 1)
    {
        do
        {
            fseek(img, fat_in_bytes+(2 * cluster[i]), SEEK_SET);
            i++;
            fread(&cluster[i], sizeof(short), 1, img);
        } while (i < ceil(clusters_to_read));
    }
    
    char content[size_of_a_cluster];
    i = 0;

    cout << "file " << this->file_name << endl;
    cout << "Content: " << endl << endl;
    while (clusters_to_read >= 1)
    {
        if(fseek(img, data_in_bytes + size_of_a_cluster * (cluster[i] - 2), SEEK_SET))
        {
            cerr << endl << endl << "Error ocurred when looking for content on data sector" << endl;
            return;
        }
        fread(&content, size_of_a_cluster, 1, img);
        clusters_to_read -= 1;
        cout << content;
        i++;
    }
    if (clusters_to_read > 0)
    {
        fread(&content, size_of_a_cluster*clusters_to_read, 1, img);
        int end_of_file = (size_of_a_cluster*clusters_to_read);
        content[end_of_file] = '\0';
        cout << content;
    }

    this->print_infos();
}

int file83::get_data_type()
{
    return static_cast<int>(this->file_atributte);
}

int file83::get_first_byte()
{
    return static_cast<int>(this->file_name[0]);
}

int root_data::search_data(FILE* root)
{
    file83 newFile;
    fread(&newFile, 32, 1, root);

    if (newFile.get_first_byte() == 0)
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
        cout << "This is a hidden file";

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
        if (this->standard_8_3_file.get_first_byte() == 229)
        {
            cout << "this is an unused file" << endl;
            return NULL;
        }
        cout << "This is a archive" << endl;
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
    cout << endl << "reading root dir" << endl;
    while(new_file.search_data(fat_root))
    {
        this->files.push_back(new_file);
    }
    cout << endl;
    return 1;
}

void root_dir::read_files(FILE* img, int fat_in_bytes, int data_in_sector, int bytes_per_sector, int sectors_per_cluster)
{
    for (int i = 0; i < this->files.size(); i++)
    {
        file83* archive = this->files[i].get_file();

        if (archive)
        {
            archive->read_file(img, fat_in_bytes, (data_in_sector*bytes_per_sector), bytes_per_sector, sectors_per_cluster);
            cout << endl << endl;
            return;
        }
        
    }
}