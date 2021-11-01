#include "../include/list.h"

bool check_options(t_opts *opts)
{
    if (opts->x && opts->c)
    {
        printf("tar: Can't specify both -x and -c\n");
        return false;
    }
    if (opts->u && opts->r)
    {
        printf("tar: Can't specify both -u and -r\n");
        return false;
    }
    if (!opts->c && !opts->x && !opts->r && !opts->u && !opts->t)
    {
        printf("tar: Must specify one of -c, -r, -t, -u, -x\n");
        return false;
    }
    if ((opts->r || opts->u) && !opts->f)
    {
        printf("tar: Cannot append to stdout.\n");
        return false;
    }
    if ((opts->t && !opts->f) || (opts->t && opts->archive_name == NULL))
    {
        printf("tar: no files or directories specified\n");
        return false;
    }
    if (opts->archive_name == NULL && opts->files->filename == NULL)
    {
        printf("tar: Option -f requires an argument\n");
        printf("Usage:\n");
        printf("  List:    tar -tf <archive-filename>\n");
        printf("  Extract: tar -xf <archive-filename>\n");
        printf("  Create:  tar -cf/-rf <archive-filename> [filenames...]\n");
        return false;
    }
    if (opts->archive_name == NULL && opts->files->filename != NULL)
    {
        printf("tar: no files or directories specified\n");
        return false;
    }
    return true;
}

t_opts *set_opts(int argc, char *argv[])
{
    int i = 1, j = 0;
    t_opts *opts = malloc(sizeof(t_opts));
    opts->c = false;
    opts->x = false;
    opts->r = false;
    opts->u = false;
    opts->t = false;
    opts->f = false;
    opts->archive_name = NULL;
    opts->files = initialize_files();
    while (i < argc)
    {
        j = 0;
        if (argv[i][j] == '-')
        {
            j++;
            while (argv[i][j] != '\0')
            {
                if (argv[i][j] == 'c')
                    opts->c = true;
                else if (argv[i][j] == 'x')
                    opts->x = true;
                else if (argv[i][j] == 'r')
                    opts->r = true;
                else if (argv[i][j] == 'u')
                    opts->u = true;
                else if (argv[i][j] == 't')
                    opts->t = true;
                else if (argv[i][j] == 'f')
                {
                    opts->f = true;

                    if (argv[i + 1])
                    {
                        if ((opts->c || opts->r) && !argv[i + 2])
                        {
                            printf("tar: Option -f requires an argument\n");
                            printf("Usage:\n");
                            printf("  List:    tar -tf <archive-filename>\n");
                            printf("  Extract: tar -xf <archive-filename>\n");
                            printf("  Create:  tar -cf/-rf <archive-filename> [filenames...]\n");
                            break;
                        }
                        else
                        {
                            opts->archive_name = strdup(argv[i + 1]);
                            i++;
                            break;
                        }
                    }
                }
                else
                    printf("tar: option -%c not available!\n", argv[i][j]);
                j++;
            }
        }
        else
        {
            if (add_file_list(&(opts)->files, argv[i]) != 0)
                return NULL;
        }
        i++;
    }

    if (check_options(opts) == false)
        return NULL;
    else
        return opts;
}

int add_file_list(t_files **files, char *filename)
{
    struct stat buffer;
    if (stat(filename, &buffer) != 0)
    {
        printf("File %s does not exist\n", filename);
        return 1;
    }

    t_files *temp;
    t_files *current = *files;

    if (current->filename == NULL)
    {
        current->filename = strdup(filename);
        return 0;
    }

    while (current)
    {
        temp = current;
        current = current->next;
    }

    t_files *new_node = (t_files *)malloc(sizeof(t_files));
    new_node->filename = strdup(filename);
    new_node->next = NULL;
    temp->next = new_node;
    return 0;
}
