#include "auxf.h"

// Parse command line arguments and process each file/directory
void parseArgs(int argc, char **argv)
{
    size_t dir_count = 0, start_print = 0, dir_indexes[ARGC_MAX];
    struct stat stat_buf;

    for (int i = 1; i < argc; ++i)
    {
        if (lstat(argv[i], &stat_buf) < 0)
        {
            switch (errno)
            {
            case EACCES:
                fprintf(stderr, "cmd-ls-l: cannot access %s: Permission denied\n", argv[i]);
                break;
            case ENOENT:
                fprintf(stderr, "cmd-ls-l: cannot access %s: No such file or directory\n", argv[i]);
                break;
            default:
                fprintf(stderr, "unknown error");
            }
        }
        else
        {
            if (S_ISDIR(stat_buf.st_mode))
            {
                dir_indexes[dir_count++] = i;
                continue;
            }
            else
            {
                start_print = 1;
                showFileInfo(argv[i], 1);
            }
        }
    }

    if (dir_count > 0 && start_print)
        printf("\n");

    for (int i = 0; i < dir_count; ++i)
    {
        if (argc > 1)
        {
            printf("%s:\n", argv[dir_indexes[i]]);
            showDirInfo(argv[dir_indexes[i]]);
            if (i != dir_count - 1)
                printf("\n");
        }
        else
            showDirInfo(argv[dir_indexes[i]]);
    }
}

// Show directory information, including its files and subdirectories
int showDirInfo(const char *dir_path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat stat_buf;
    char file_path[PATH_MAX];
    size_t total = 0;

    if (NULL == (dir = opendir(dir_path)))
        return -1;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        else
        {
            if ('/' == dir_path[strlen(dir_path) - 1])
                snprintf(file_path, PATH_MAX, "%s%s", dir_path, entry->d_name);
            else
                snprintf(file_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);

            showFileInfo(file_path, 0);

            lstat(file_path, &stat_buf);
            total += stat_buf.st_blocks;
        }
    }
    printf("total: %d\n", total >> 1);

    closedir(dir);
    return 0;
}

// Show file information, including file type, permissions, owner, size, modification time, etc.
int showFileInfo(const char *file_path, int pr_fpath)
{
    struct stat stat_buf;
    char f_type, permissions[10], time_buf[TIME_STRING_MAX];

    lstat(file_path, &stat_buf);

    if (S_ISREG(stat_buf.st_mode))
        f_type = '-';
    else if (S_ISDIR(stat_buf.st_mode))
        f_type = 'd';
    else if (S_ISLNK(stat_buf.st_mode))
        f_type = 'l';
    else if (S_ISCHR(stat_buf.st_mode))
        f_type = 'c';
    else if (S_ISBLK(stat_buf.st_mode))
        f_type = 'b';
    else if (S_ISFIFO(stat_buf.st_mode))
        f_type = 'p';
    else
        f_type = '?';

    permissions[0] = (stat_buf.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (stat_buf.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (stat_buf.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (stat_buf.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (stat_buf.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (stat_buf.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (stat_buf.st_mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (stat_buf.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (stat_buf.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[9] = '\0';

    strftime(time_buf, TIME_STRING_MAX, "%b %d %H:%M", localtime(&stat_buf.st_mtime));

    printf("%c%s. %3d %s %s %5ld %s",
           f_type,
           permissions,
           stat_buf.st_nlink,
           getpwuid(stat_buf.st_uid)->pw_name,
           getgrgid(stat_buf.st_gid)->gr_name,
           stat_buf.st_size,
           time_buf);

    if (pr_fpath)
        printf(" %s", file_path);
    else
    {
        char file_name[PATH_MAX];
        extractLastName(file_path, file_name);
        printf(" %s", file_name);
    }

    if ('l' != f_type)
        printf("\n");
    else
    {
        char link_target[PATH_MAX] = {'\0'};

        if (readlink(file_path, link_target, PATH_MAX - 1) < 0)
            return -1;
        else
            printf(" -> %s\n", link_target);
    }

    return 0;
}

// Extract the file name from a file path
void extractLastName(const char *path, char *buf)
{
    if (NULL == path || NULL == buf)
        return;

    size_t len = strlen(path), pre_start, end = len - 1;

    if (0 == len)
        return;

    if ('/' == path[end])
        --end;

    for (pre_start = end; pre_start >= 0; --pre_start)
        if ('/' == path[pre_start])
            break;

    memcpy(buf, path + pre_start + 1, sizeof(char) * (end - pre_start));
    buf[end - pre_start] = '\0';
}
