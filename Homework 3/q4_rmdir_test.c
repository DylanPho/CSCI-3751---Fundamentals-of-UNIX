#include "apue.h"
#include <dirent.h>
#include <errno.h>

#define DIR_PERM 0777
#define DIR_NAME "/csegrid/home/phouttdy/csci3751/hw3/test"

void try_opendir(const char *label, const char *path) {
    DIR *dp = opendir(path);
    if (dp == NULL) {
        printf("opendir(%s) failed: %s\n", label, strerror(errno));
    } else {
        printf("opendir(%s) succeeded\n", label);
        closedir(dp);
    }
}

int main() {
    if (mkdir(DIR_NAME, DIR_PERM) < 0)
        err_sys("mkdir failed");

    if (chdir(DIR_NAME) < 0)
        err_sys("chdir failed");

    if (rmdir(DIR_NAME) < 0)
        err_sys("rmdir failed");

    printf("Directory was deleted while we are still inside it.\n");

    try_opendir(".", ".");
    try_opendir("../test", "../test");
    try_opendir("DIR_NAME", DIR_NAME);

    return 0;
}
