/*
 * Student ID: 2018203062
 * Name: KIMSUBIN
 */
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_PATH_LEN 1024

// function
void fileType(const struct stat *fileInfo);
void fileMode(const struct stat *fileInfo);

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Usage : %s [pathname]\n", argv[0]);
        exit(-1);
    }

    if (chdir(argv[1]) == -1) {
        perror("chdir() error!");
        exit(-1);
    }

    char cwd[MAX_PATH_LEN + 1] = {
        '\0',
    };
    if (getcwd(cwd, MAX_PATH_LEN) == NULL) {
        perror("getcwd() error!");
        exit(-1);
    }
    printf("Directory to serach : %s\n", argv[1]);

    DIR *dirp;
    struct stat fileInfo;
    struct dirent *dirInfo;
    struct passwd *userInfo;

    dirp = opendir(cwd); // open dir

    // print error message and exit

    while ((dirInfo = readdir(dirp)) != NULL) {
        if (stat(dirInfo->d_name, &fileInfo) == -1) {
            perror("stat() error!");
            exit(-1);
        }

        fileType(&fileInfo); // fileType
        fileMode(&fileInfo); // permission
        userInfo = getpwuid(fileInfo.st_uid);
        printf("       %s", userInfo->pw_name);   // owner
        printf("%8ld", fileInfo.st_size);         // fileSize
        printf("       %8ld", fileInfo.st_mtime); // time
        printf("       %s \n", dirInfo->d_name);  // name
    }

    closedir(dirp);

    return 0;
}

void fileMode(const struct stat *fileInfo) {

    // User
    if (S_IRUSR & fileInfo->st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWUSR & fileInfo->st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXUSR & fileInfo->st_mode) {
        printf("x");
    } else {
        printf("-");
    }

    // Group
    if (S_IRGRP & fileInfo->st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWGRP & fileInfo->st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXGRP & fileInfo->st_mode) {
        printf("x");
    } else {
        printf("-");
    }

    // Others
    if (S_IROTH & fileInfo->st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWOTH & fileInfo->st_mode) {
        printf("x");
    } else {
        printf("-");
    }
    if (S_IXOTH & fileInfo->st_mode) {
        printf("w");
    } else {
        printf("-");
    }
}

void fileType(const struct stat *fileInfo) {
    if (S_ISREG(fileInfo->st_mode)) {
        printf("-");
    } else if (S_ISDIR(fileInfo->st_mode)) {
        printf("d");
    } else if (S_ISLNK(fileInfo->st_mode)) {
        printf("l");
    } else if (S_ISBLK(fileInfo->st_mode)) {
        printf("b");
    } else if (S_ISCHR(fileInfo->st_mode)) {
        printf("c");
    }else if (S_ISSOCK(fileInfo->st_mode)){
	printf("s");
    }else {
        printf("?");
    }
}
