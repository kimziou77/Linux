/*
 * Student ID: 2017726001
 * Name: 홍길동
 */
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PATH_LEN 1024

void myError(const char *msg);
void fileType(const struct stat *fileInfo);
void fileMode(const struct stat *fileInfo);

int main(int argc, char *argv[]) {
    char cwd[MAX_PATH_LEN + 1] = {
        '\0',
    };

    DIR *dirp;
    struct dirent *dirInfo;

    struct stat fileInfo;
    struct passwd *userInfo;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [pathname]\n", argv[0]);
        exit(-1);
    }

    if (stat(argv[1], &fileInfo) == -1) {
        myError("stat() error!");
    }

    printf("Directory to search: %s\n", argv[1]);

    // 작업 디렉토리를 입력된 디렉토리의 경로로 변경
    chdir(argv[1]);

    // 디렉토리 내의 하위 파일 또는 하위 디렉토리를 탐색
    dirp = opendir(".");
    while ((dirInfo = readdir(dirp)) != NULL) {
        if (lstat(dirInfo->d_name, &fileInfo) == -1) {
            myError("lstat() error!");
        }

        userInfo = getpwuid(fileInfo.st_uid);

        fileType(&fileInfo);
        fileMode(&fileInfo);
        printf("  %8s", userInfo->pw_name);
        printf("  %8lld", fileInfo.st_size);
        printf("  %ld", fileInfo.st_mtime);
        printf("  %s\n", dirInfo->d_name);
    }

    closedir(dirp);

    return 0;
}

void myError(const char *msg) {
    perror(msg);
    exit(-1);
}

void fileType(const struct stat *fileInfo) {
    if (S_ISREG(fileInfo->st_mode)) {
        printf("-");
    } else if (S_ISDIR(fileInfo->st_mode)) {
        printf("d");
    } else if (S_ISLNK(fileInfo->st_mode)) {
        printf("l");
    } else if (S_ISSOCK(fileInfo->st_mode)) {
        printf("s");
    } else if (S_ISFIFO(fileInfo->st_mode)) {
        printf("p");
    } else if (S_ISCHR(fileInfo->st_mode)) {
        printf("c");
    } else if (S_ISBLK(fileInfo->st_mode)) {
        printf("b");
    } else {
        printf("?");
    }
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
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXOTH & fileInfo->st_mode) {
        printf("x");
    } else {
        printf("-");
    }
}
