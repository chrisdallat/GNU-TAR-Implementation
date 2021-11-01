#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <utime.h>
#include <dirent.h>

#define OCTAL_BASE 8
#define DECIMAL_BASE 10

/* https://www.gnu.org/software/tar/manual/html_node/Standard.html */
/*

  The name field is the file name of the file...
  with directory names (if any) preceding the file name, separated by slashes.

  The size field is the size of the file in bytes;
  linked files are archived with this field specified as zero.

  The mtime field represents the data modification time...
  of the file at the time it was archived.

 */

/* POSIX header.  */
typedef struct posix_header
{                       /* byte offset */
    char name[100];     /*   0 */
    char mode[8];       /* 100 */
    char uid[8];        /* 108 */
    char gid[8];        /* 116 */
    char size[12];      /* 124 */
    char mtime[12];     /* 136 */
    char chksum[8];     /* 148 */
    char typeflag;      /* 156 */
    char linkname[100]; /* 157 */
    char magic[6];      /* 257 */
    char version[2];    /* 263 */
    char uname[32];     /* 265 */
    char gname[32];     /* 297 */
    char devmajor[8];   /* 329 */
    char devminor[8];   /* 337 */
    char prefix[155];   /* 345 */
                        /* 500 */
} header;

#define TMAGIC "ustar" /* ustar and a null */
#define TVERSION "00"  /* 00 and no null */
#define TVERSLEN 2

/* Values used in typeflag field.  */
#define REGTYPE '0'   /* regular file */
#define AREGTYPE '\0' /* regular file */
#define LNKTYPE '1'   /* link */
#define SYMTYPE '2'   /* reserved */
#define CHRTYPE '3'   /* character special */
#define BLKTYPE '4'   /* block special */
#define DIRTYPE '5'   /* directory */
#define FIFOTYPE '6'  /* FIFO special */
#define CONTTYPE '7'  /* reserved */

/* lengths of various fields.  */
#define NAMELEN 100
#define MODELEN 8
#define UIDLEN 8
#define GIDLEN 8
#define SIZELEN 12
#define MTIMELEN 12
#define CHKSUMLEN 8
#define TYPELEN 1
#define LNGLEN 100
#define TMAGLEN 6
#define TVERSLEN 2
#define UNAMELEN 32
#define GNAMELEN 32
#define DEVLEN 8 /* devmajor + devminor */
#define PREFIXLEN 155
#define HEADERBYTE 12 /* end of header */
#define BLOCKSIZE 512 /* size of a tar block */
#define BLOCKSIZEOCT 0000200


#ifndef STRUCT_H
#define STRUCT_H

typedef struct s_files
{
    char *filename;
    struct s_files *next;
} t_files;

typedef struct s_opts
{
    bool c;
    bool x;
    bool r;
    bool u;
    bool t;
    bool f;
    char *archive_name;
    struct s_files *files;
} t_opts;

#endif



void print_header(header *header);

//tar_options.c
bool check_options(t_opts *opts);
t_opts *set_opts(int argc, char *argv[]);
int add_file_list(t_files **files, char *filename);

//tar_archive.c
int open_archive(t_opts *opts);
void end_archive(int fd);
void create_archive(int fd, t_opts *opts);
char *read_tar_content(int fd, int size);
int write_to_tar(char *filename, int fd);
void write_file(int fd, char *filename);
void write_directory(int fd, char *dirname);
int get_file_size(int fd);
char *get_path(char *directory, char *content);

//tar_util.c
int my_memset(char *buffer, char c, int size);
char *my_itoa(char *dest, unsigned int number, int size, int base);
void reset_list(t_files **files);
void reset_opts(t_opts *opts);
t_files *initialize_files();
long my_atoi(char *number_string, int size);
long my_oct_to_dec(long octal_number);
int my_iterative_pow(int num, int pow);
unsigned long get_file_mtime(char *filename);
unsigned int get_checksum(header *header);
char get_typeflag(int mode);
header *create_header(char *filename);
unsigned int find_chksum(header *hdr);

//tar_extract.c
int extract_file(header *hdr, char *archive_content);
int extract_directory(header *header);
int get_header_size(header *header);
int set_mtime(header *header);
char *get_archive_content(int fd, int size);
int get_archive_size(char *filename);
bool is_header(header *hdr);
int get_header_chksum(header *hdr);
int extract_archive(int fd, t_opts *opts);
unsigned long get_header_mtime(header *hdr);
int get_header_mode(header *hdr);

//tar_list.c
int list_archive(int fd, t_opts *opts);
void remove_duplicates(t_files *files);
void print_list(t_files **files);

//tar_append.c
int append_archive(int fd, t_opts *opts);

//tar_update.c
bool in_args(char *filename, t_files **args);
int update_archive(int fd, t_opts *opts);
