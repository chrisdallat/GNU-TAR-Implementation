#include "../include/list.h"

//check if filename is in linked list
bool in_args(char *filename, t_files **files)
{
    t_files *current = *files;
    if (current->filename == NULL)
        return false;

    while (current && current->filename != NULL)
    {
        if (strcmp(current->filename, filename) == 0)
            return true;
        current = current->next;
    }

    return false;
}

int update_archive(int fd, t_opts *opts)
{
    int ret = 0;
    t_files *current = opts->files;
    int size = get_archive_size(opts->archive_name);
    char *content = get_archive_content(fd, size);
    int index = 0;
    unsigned long mtime_in_tar;
    unsigned long mtime_update_file;
    header *hdr;
    while (current)
    {
        while (index < size)
        {
            hdr = (header *)&content[index];
            if (strcmp(hdr->name, current->filename) == 0)
            {
                mtime_in_tar = (unsigned long)my_oct_to_dec(my_atoi(hdr->mtime, 12));
                struct stat file_info;
                lstat(current->filename, &file_info);
                mtime_update_file = (unsigned long)file_info.st_mtim.tv_sec;

                if (mtime_in_tar < mtime_update_file)
                    ret += write_to_tar(current->filename, fd);
            }
            index += BLOCKSIZE;
        }
        index = 0;
        current = current->next;
    }
    end_archive(fd);
    return ret;
}
