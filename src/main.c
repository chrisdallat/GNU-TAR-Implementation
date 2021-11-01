#include "../include/list.h"

int main(int argc, char **argv)
{
    t_opts *opts = set_opts(argc, argv);
    if (opts == NULL)
    {
        return 1;
    }

    int fd = open_archive(opts);

    if (opts->c && opts->f)
        create_archive(fd, opts);
    else if (opts->x)
        extract_archive(fd, opts);
    else if (opts->t)
        list_archive(fd, opts);
    else if (opts->r)
        append_archive(fd, opts);
    else if (opts->u)
        update_archive(fd, opts);
    return 0;
}
