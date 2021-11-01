#include "../include/list.h"

void print_list(t_files **files)
{
    t_files *current = *files;

    while (current != NULL)
    {
        printf("%s\n", current->filename);
        current = current->next;
    }
}

//function to remove duplicates from t_files list
void remove_duplicates(t_files *files)
{
    t_files *ptr1, *ptr2;
    ptr1 = files;
    while (ptr1 != NULL)
    {
        ptr2 = ptr1;
        while (ptr2->next != NULL)
        {
            if (strcmp(ptr1->filename, ptr2->next->filename) == 0)
                ptr2->next = ptr2->next->next;
            else
                ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
}

int list_archive(int fd, t_opts *opts)
{
    int res = 0;
    int archive_size = get_archive_size(opts->archive_name);
    char *archive_content = get_archive_content(fd, archive_size);
    header *hdr;
    int index = 0;

    while (index < archive_size)
    {
        hdr = (header *)&archive_content[index];
        if (is_header(hdr))
        {
            add_file_list(&(opts)->files, hdr->name);
            index += BLOCKSIZE;
        }
        else
            index += BLOCKSIZE;
    }
    remove_duplicates(opts->files);
    print_list(&(opts)->files);
    return res + index;
}