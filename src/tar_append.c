#include "../include/list.h"

int append_archive(int fd, t_opts *opts)
{
    int res = 0;
    t_files *current = opts->files;
    while (current)
    {
        res += write_to_tar(current->filename, fd);
        current = current->next;
    }
    end_archive(fd);
    return res;
}