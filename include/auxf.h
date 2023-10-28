#ifndef AUXF_H
#define AUXF_H 1

#include <sys/stat.h>

#include <errno.h>
#include <stdio.h>

#include <dirent.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ARGC_MAX 10
#define TIME_STRING_MAX 25

void parseArgs(int argc, char **argv);

int showDirInfo(const char *dir_path);
int showFileInfo(const char *file_path, int pr_fpath);

void extractLastName(const char *path, char *buf);

#endif
