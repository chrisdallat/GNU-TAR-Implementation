#include "../include/list.h"

int open_archive(t_opts *opts)
{
    int fd, flags = 0;
    const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    if (opts->c)
        flags = O_CREAT | O_WRONLY | O_TRUNC;
    else if (opts->x || opts->t)
        flags = O_RDONLY;
    else if (opts->r)
        flags = O_WRONLY | O_APPEND;
    else if (opts->u)
        flags = O_RDWR;

    fd = open(opts->archive_name, flags, mode);
    return fd;
}

void end_archive(int fd)
{
    char end[BLOCKSIZE * 2];
    my_memset(end, '\0', BLOCKSIZE * 2);

    write(fd, end, BLOCKSIZE * 2);
}

void create_archive(int fd, t_opts *opts)
{
    t_files *current = opts->files;
    while (current != NULL)
    {
        write_to_tar(current->filename, fd);
        current = current->next;
    }
    end_archive(fd);
}

// create the header and write them to the tar
int write_to_tar(char *filename, int fd)
{
    int ret = 0;
    header *header = malloc(BLOCKSIZE);

    strncpy(header->name, filename, 100);

    struct stat file_info;
    stat(filename, &file_info);
    my_itoa(header->mode, file_info.st_mode & 0777, 8, OCTAL_BASE);
    my_itoa(header->uid, file_info.st_uid, 8, OCTAL_BASE);
    my_itoa(header->gid, file_info.st_gid, 8, OCTAL_BASE);
    my_itoa(header->size, file_info.st_size, 12, OCTAL_BASE);
    my_itoa(header->mtime, file_info.st_mtime, 12, OCTAL_BASE);

    header->typeflag = get_typeflag(file_info.st_mode);

    strncpy(header->name, filename, 100);
    if (header->typeflag == '5')
    {
        int i = strlen(header->name);
        header->name[i] = '/';
        header->name[i + 1] = '\0';
        strncpy(header->size, "00000000000", 12);
    }

    int n_read = readlink(filename, header->linkname, 100);
    if (strlen(header->linkname) != 0)
    {
        header->typeflag = SYMTYPE;
    }

    if (n_read == -1)
    {
        my_memset(header->linkname, '\0', 100);
        n_read = 0;
    }
    else
    {
        header->linkname[n_read] = '\0';
    }

    strncpy(header->magic, TMAGIC, 6);
    //strncpy(header->version, TVERSION, 2);
    header->version[0] = ' ';
    header->version[1] = ' ';

    struct passwd *pwd;
    pwd = getpwuid(file_info.st_uid);
    char *user_name = pwd->pw_name;
    strcpy(header->uname, user_name);

    struct group *grp;
    grp = getgrgid(file_info.st_gid);
    char *group_name = grp->gr_name;
    strcpy(header->gname, group_name);

    //int dev_major = (int)major(file_info.st_rdev);
    //int dev_minor = (int)minor(file_info.st_rdev);
    //my_itoa(header->devmajor, dev_major, 8, OCTAL_BASE);
    //my_itoa(header->devminor, dev_minor, 8, OCTAL_BASE);
    header->devmajor[0] = '\0';
    header->devminor[0] = '\0';
    header->prefix[0] = '\0';

    my_memset(header->chksum, ' ', 8); //set checksum to blanks before calculating it
    my_itoa(header->chksum, (get_checksum(header)), 7, OCTAL_BASE);
    header->chksum[7] = ' ';

    if (header != NULL)
    {
        write(fd, header, BLOCKSIZE);
        if (header->typeflag == REGTYPE)
        {
            write_file(fd, filename);
        }
        else if (header->typeflag == DIRTYPE)
        {
            write_directory(fd, filename);
        }
        free(header);
    }
    else
    {
        ret++;
    }
    return ret;
}

char *read_tar_content(int fd, int size)
{
    char *content = malloc(size * sizeof(char) + 1);
    read(fd, content, size);
    return content;
}

void write_file(int fd, char *filename)
{

    int fd_file = open(filename, O_RDONLY);
    int file_size = get_file_size(fd_file);
    int new_size = file_size + BLOCKSIZE - (file_size % BLOCKSIZE);

    char content[new_size];
    my_memset(content, '\0', new_size);

    read(fd_file, content, file_size);
    write(fd, content, new_size);

    close(fd_file);
}

void write_directory(int fd, char *dirname)
{
    DIR *dir = opendir(dirname);
    struct dirent *sd;
    char *path;
    while ((sd = readdir(dir)) != NULL)
    {
        if (strcmp(sd->d_name, ".") != 0 && strcmp(sd->d_name, "..") != 0)
        {
            path = get_path(dirname, sd->d_name);
            write_to_tar(path, fd);
            free(path);
        }
    }
    closedir(dir);
}

char *get_path(char *directory, char *content)
{
    char *path = malloc(strlen(directory) + strlen(content) + 1);
    strcpy(path, directory);
    strcat(path, "/");
    strcat(path, content);
    return path;
}

int get_file_size(int fd)
{
    int size = 0;
    int n_read = 0;
    char buffer[BLOCKSIZE];

    while ((n_read = read(fd, buffer, BLOCKSIZE)) > 0)
    {
        size += n_read;
    }

    if (n_read % BLOCKSIZE != 0)
    {
        size += BLOCKSIZE - (size % BLOCKSIZE);
    }

    lseek(fd, 0, SEEK_SET);
    return size;
}
