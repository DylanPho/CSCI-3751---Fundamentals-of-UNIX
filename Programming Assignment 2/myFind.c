#include "apue.h"
#include <dirent.h>
#include <limits.h>
#include <string.h>

typedef int Myfunc(const char *, const struct stat *, int);
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);

static char *fullpath;
static size_t pathlen;

int total_files = 0;
off_t total_bytes = 0;
char *pattern;  // from argv[2]

int main(int argc, char *argv[]) {
    int ret;

    if (argc != 3)
        err_quit("usage: myFind <starting-path> <pattern>");

    pattern = argv[2];

    ret = myftw(argv[1], myfunc);

    printf("Total number of files: %d\n", total_files);
    printf("Total number of bytes: %lld\n", (long long)total_bytes);
    exit(ret);
}

#define FTW_F   1
#define FTW_D   2
#define FTW_DNR 3
#define FTW_NS  4

static int myftw(char *pathname, Myfunc *func) {
    pathlen = PATH_MAX;
    if ((fullpath = malloc(pathlen)) == NULL)
        err_sys("malloc failed");

    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");
    }

    strcpy(fullpath, pathname);
    return dopath(func);
}

static int dopath(Myfunc *func) {
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;

    if (lstat(fullpath, &statbuf) < 0)
        return func(fullpath, &statbuf, FTW_NS);

    if (S_ISDIR(statbuf.st_mode) == 0)
        return func(fullpath, &statbuf, FTW_F);

    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return ret;

    n = strlen(fullpath);
    fullpath[n++] = '/';
    fullpath[n] = 0;

    if ((dp = opendir(fullpath)) == NULL)
        return func(fullpath, &statbuf, FTW_DNR);

    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(&fullpath[n], dirp->d_name);
        if ((ret = dopath(func)) != 0)
            break;
    }

    fullpath[n-1] = 0;
    if (closedir(dp) < 0)
        err_ret("can't close directory %s", fullpath);

    return ret;
}

static int myfunc(const char *pathname, const struct stat *statptr, int type) {
    if (type != FTW_F)
        return 0;

    const char *filename = strrchr(pathname, '/');
    filename = (filename != NULL) ? filename + 1 : pathname;

    if (strstr(filename, pattern) != NULL) {
        printf("%s %lld\n", pathname, (long long)statptr->st_size);
        total_files++;
        total_bytes += statptr->st_size;
    }

    return 0;
}
