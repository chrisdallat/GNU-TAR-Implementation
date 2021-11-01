#include "../include/list.h"

int my_memset(char *buffer, char c, int size)
{
    int index = 0;
    while (index < size)
    {
        buffer[index] = c;
        index += 1;
    }
    return index;
}

char *my_itoa(char *dest, unsigned int number, int size, int base)
{
    const char symbols[] = "0123456789abcdef";
    my_memset(dest, '0', size - 1);
    dest[size - 1] = '\0';
    size -= 2;
    while (number)
    {
        dest[size] = symbols[number % base];
        number /= base;
        size--;
    }
    return dest;
}

void reset_list(t_files **files)
{
    t_files *current = *files;
    if (current->filename == NULL)
    {
        free(current);
        return;
    }
    t_files *prev;
    while (current && current->filename != NULL)
    {
        free(current->filename);
        prev = current;
        current = current->next;
        free(prev);
    }
}

void reset_opts(t_opts *opts)
{
    if (opts->archive_name != NULL)
    {
        free(opts->archive_name);
    }
    reset_list(&(opts)->files);
    free(opts);
}

t_files *initialize_files()
{
    t_files *head = (t_files *)malloc(sizeof(t_files));
    head->filename = NULL;
    head->next = NULL;

    return head;
}

long my_atoi(char *number_string, int size)
{
    int index = 0;
    long res = 0;
    while (index < size && number_string[index])
    {
        if (number_string[index] >= '0' && number_string[index] <= '9')
        {
            res *= 10;
            res += number_string[index] - '0';
        }
        index++;
    }
    return res;
}

long my_oct_to_dec(long octal_number)
{
    int decimal = 0;
    int index = 0;
    while (octal_number > 0)
    {
        decimal += (octal_number % 10) * my_iterative_pow(8, index);
        index++;
        octal_number /= 10;
    }
    return decimal;
}

int my_iterative_pow(int num, int pow)
{
    int result = 1;
    if (pow < 0)
        return 0;
    for (int i = 0; i < pow; i++)
        result *= num;
    return result;
}

unsigned long get_file_mtime(char *filename)
{
    struct stat *file_info = NULL;
    stat(filename, file_info);
    printf("get file mtime function: %ld: \n", file_info->st_mtim.tv_sec); //linux uses st_mtimespec.tv_sec
    return (unsigned long)file_info->st_mtim.tv_sec; //linux uses st_mtimespec.tv_sec
}

unsigned int get_checksum(header *header)
{
    int sum = 0;
    char *header_size = (char *)header;
    for (int i = 0; i < BLOCKSIZE; i++)
    {
        sum += header_size[i];
    }
    return sum;
}

char get_typeflag(int mode)
{
    switch (mode & S_IFMT)
    {
    case S_IFREG:
        return REGTYPE;
    case S_IFLNK:
        return SYMTYPE;
    case S_IFCHR:
        return CHRTYPE;
    case S_IFBLK:
        return BLKTYPE;
    case S_IFDIR:
        return DIRTYPE;
    case S_IFIFO:
        return FIFOTYPE;
    default:
        return REGTYPE;
    }
}

unsigned int find_chksum(header *hdr)
{
    unsigned int sum = 0;
    char hdr_chksum[8];
    strcpy(hdr_chksum, hdr->chksum);
    my_memset(hdr->chksum, ' ', 8);
    char *headerBytes = (char *)hdr;
    for (int i = 0; i < BLOCKSIZE; i++)
    {
        sum += headerBytes[i];
    }
    strcpy(hdr->chksum, hdr_chksum);
    return sum;
}
