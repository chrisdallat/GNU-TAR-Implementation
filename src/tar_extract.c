#include "../include/list.h"

int extract_file(header *hdr, char *archive_content)
{
    char *f_content = archive_content + BLOCKSIZE;
    int f_size = get_header_size(hdr);
    int mode = get_header_mode(hdr);
    int fd = open(hdr->name, O_WRONLY | O_CREAT, mode);
    write(fd, f_content, f_size);
    close(fd);
    set_mtime(hdr);
    int offset = BLOCKSIZE - (f_size % 512);
    return BLOCKSIZE + f_size + offset;
}

int extract_directory(header *hdr)
{
    int mode = get_header_mode(hdr);
    mkdir(hdr->name, mode);
    set_mtime(hdr);
    return BLOCKSIZE;
}

int get_header_size(header *header)
{
    return my_oct_to_dec(my_atoi(header->size, strlen(header->size)));
}

int set_mtime(header *header)
{
    unsigned long m_time_of_file = my_atoi(header->mtime, 12);
    struct utimbuf time_buf;
    time_buf.modtime = my_oct_to_dec(m_time_of_file);
    time_buf.actime = time(NULL);
    return utime(header->name, &time_buf);
}

char *get_archive_content(int fd, int size)
{
    char *content = malloc(size * sizeof(char) + 1);
    read(fd, content, size);
    return content;
}

int get_archive_size(char *filename)
{
    struct stat info;
    if (stat(filename, &info) < 0)
        return -1;
    return info.st_size;
}

bool is_header(header *hdr)
{
    unsigned int header_checksum = get_header_chksum(hdr);
    unsigned int my_checksum = find_chksum(hdr);

    if (my_checksum == 0 || header_checksum != my_checksum)
    {
        return false;
    }
    if (my_checksum == header_checksum)
    {
        return true;
    }
    return false;
}

int get_header_chksum(header *hdr)
{
    return my_oct_to_dec(my_atoi(hdr->chksum, 8));
}

unsigned long get_header_mtime(header *hdr)
{
    return (unsigned long)my_oct_to_dec(my_atoi(hdr->mtime, 12));
}

int get_header_mode(header *hdr)
{
    return my_oct_to_dec(my_atoi(hdr->mode, sizeof(hdr->mode)));
}

int extract_archive(int fd, t_opts *opts)
{
    int ret = 0;
    int archive_size = get_archive_size(opts->archive_name);
    char *archive_content = get_archive_content(fd, archive_size);
    header *hdr;
    int index = 0;

    while (index < archive_size)
    {
        hdr = (header *)&archive_content[index];
        if (is_header(hdr))
        {
            if (hdr->typeflag == REGTYPE)
            {
                extract_file(hdr, &archive_content[index]);
            }
            else if (hdr->typeflag == DIRTYPE)
            {
                extract_directory(hdr);
            }
            index += BLOCKSIZE;
        }
        else
        {
            index += BLOCKSIZE;
        }
    }

    return ret + index;
}