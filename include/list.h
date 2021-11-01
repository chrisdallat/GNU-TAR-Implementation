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

#ifndef TAR_OPTIONS_C
#define TAR_OPTIONS_C

bool check_options(t_opts*);
t_opts *set_opts(int, char**);
int add_file_list(t_files**, char*);

#endif

// -------------------

#ifndef TAR_ARCHIVE_C
#define TAR_ARCHIVE_C

int open_archive(t_opts*);
void end_archive(int);
void create_archive(int, t_opts*);
char *read_tar_content(int, int);
int write_to_tar(char*, int);
void write_file(int, char*);
void write_directory(int, char*);
int get_file_size(int);
char *get_path(char*, char*);

#endif

// -------------------

#ifndef TAR_UTIL_C
#define TAR_UTIL_C

int my_memset(char*, char, int);
char *my_itoa(char*, unsigned int, int, int);
void reset_list(t_files**);
void reset_opts(t_opts*);
t_files *initialize_files();
long my_atoi(char*, int);
long my_oct_to_dec(long);
int my_iterative_pow(int, int);
unsigned long get_file_mtime(char*);
unsigned int get_checksum(header*);
char get_typeflag(int);
header *create_header(char*);
unsigned int find_chksum(header*);

#endif

// -------------------

#ifndef TAR_EXTRACT_C
#define TAR_EXTRACT_C

int extract_file(header*, char*);
int extract_directory(header*);
int get_header_size(header*);
int set_mtime(header*);
char *get_archive_content(int, int);
int get_archive_size(char*);
bool is_header(header*);
int get_header_chksum(header*);
int extract_archive(int, t_opts*);
unsigned long get_header_mtime(header*);
int get_header_mode(header*);

#endif

// -------------------

#ifndef TAR_LIST_C
#define TAR_LIST_C

int list_archive(int, t_opts*);
void remove_duplicates(t_files*);
void print_list(t_files**);

#endif

// -------------------

#ifndef TAR_APPEND_C
#define TAR_APPEND_C

int append_archive(int, t_opts*);

#endif

// --------------------

#ifndef TAR_UPDATE_C
#define TAR_UPDATE_C

bool in_args(char*, t_files**);
int update_archive(int, t_opts*);

#endif