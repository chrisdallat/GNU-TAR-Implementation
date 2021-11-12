<!-- GETTING STARTED -->
## TAR (GNU) Implementation
### Task Description

my_tar is a replication of the GNU Tar command, only works for GNU format Tar headers.

The first option to tar is a mode indicator from the following list:

* -c Create a new archive containing the specified items.
* -r Like -c, but new entries are appended to the archive. The -f option is required.
* -t List archive contents to stdout.
* -u Like -r, but new entries are added only if they have a modification date newer than the corresponding entry in the archive. The -f option is required.
* -x Extract to disk from the archive. If a file with the same name appears more than once in the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies.
* In -c, -r, or -u mode, each specified file or directory is added to the archive in the order specified on the command line. By default, the contents of each directory are also archived.

Unless specifically stated otherwise, options are applicable in all operating modes:

* -f file Read the archive from or write the archive to the specified file. The filename can be standard input or standard output

## Authorized function(s)
* chmod
* close(2)
* free(3)
* fstat(2)
* getpwuid
* getgrgid
* getxattr
* listxattr
* lseek(2)
* major
* malloc(3)
* minor
* mkdir(2)
* open(2)
* opendir
* read(2)
* readdir
* readlink
* stat(2)
* lstat(2)
* symlink
* unlink(2)
* time
* ctime
* utime
* write(2)

### Program Usage

1. From the terminal move to "my_tar" directory and type "make" to compile the program.

   ```sh
   make
   ```
2. Type ./my_tar -c/x/r/u/f archive.tar file1 file2 ...
    * -t and -c cannot both be used at once.
    * -c and -x cannot both be used at once.
    * -c and -r cannot both be used at once.
    * -f must always be used (except with -t)
    * error handling will notify user 

   ```sh
   e.g './my_tar -c archive.tar file1.c file2.c'
   ```
3. The program will work as regular tar command, and will work with tarballs created by real GNU tar, and vice versa. 

4. You can clean up '.o' files by typing the following command after usage
    ```sh
   'make clean'
   ```
