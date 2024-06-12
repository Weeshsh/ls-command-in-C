#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <getopt.h>

#define RESET "\033[0m"
#define DIR_COLOR "\033[1;34m"
#define EXEC_COLOR "\033[1;32m"
#define FILE_COLOR "\033[0m"

int compare_mtime(const struct dirent** a, const struct dirent** b)
{
    struct stat statA, statB;
    char pathA[1024], pathB[1024];

    snprintf(pathA, sizeof(pathA), "%s/%s", ".", (*a)->d_name);
    snprintf(pathB, sizeof(pathB), "%s/%s", ".", (*b)->d_name);

    stat(pathA, &statA);
    stat(pathB, &statB);

    if (statA.st_mtime > statB.st_mtime)
        return -1;
    if (statA.st_mtime < statB.st_mtime)
        return 1;

    return 0;
}

void print_full_file_info(const char* path, const char* fileName)
{
    struct stat fileInfo;
    char fullPath[1024];

    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, fileName);

    if (stat(fullPath, &fileInfo) < 0)
    {
        perror("stat");
        return;
    }

    printf((S_ISDIR(fileInfo.st_mode)) ? "d" : "-");
    printf((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileInfo.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileInfo.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileInfo.st_mode & S_IROTH) ? "r" : "-");
    printf((fileInfo.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileInfo.st_mode & S_IXOTH) ? "x" : "-");
    printf(" %lu", fileInfo.st_nlink);
    printf(" %lu", fileInfo.st_uid);
    printf(" %lu", fileInfo.st_gid);
    printf(" %5ld", fileInfo.st_size);

    char time[128];
    struct tm lt;
    localtime_r(&fileInfo.st_mtime, &lt);
    strftime(time, sizeof(time), "%b %d %H:%M", &lt);
    printf(" %s", time);

    if (S_ISDIR(fileInfo.st_mode))
    {
        printf(" " DIR_COLOR "%s" RESET "\n", fileName);
    }
    else if (fileInfo.st_mode & S_IXUSR)
    {
        printf(" " EXEC_COLOR "%s" RESET "\n", fileName);
    }
    else
    {
        printf(" " FILE_COLOR "%s" RESET "\n", fileName);
    }
}

void print_output(const char* path, bool all, bool recursive, bool details, bool sort)
{
    struct dirent** namelist;
    struct stat fileInfo;
    char fullPath[1024];
    long long total_blocks = 0;

    int length = scandir(path, &namelist, NULL, sort ? compare_mtime : alphasort);

    if (details)
    {
        for (int j = 0; j < length; j++)
        {
            snprintf(fullPath, sizeof(fullPath), "%s/%s", path, namelist[j]->d_name);

            if (stat(fullPath, &fileInfo) == 0)
            {
                total_blocks += fileInfo.st_blocks;
            }
        }
        printf("total %lld\n", total_blocks / 2);
    }

    for (int i = 0; i < length; i++)
    {
        if (details)
        {
            print_full_file_info(path, namelist[i]->d_name);
        }
        else
        {
            snprintf(fullPath, sizeof(fullPath), "%s/%s", path, namelist[i]->d_name);

            if (stat(fullPath, &fileInfo) < 0)
            {
                free(namelist[i]);
                continue;
            }

            if (S_ISDIR(fileInfo.st_mode))
            {
                printf(DIR_COLOR "%s" RESET "\n", namelist[i]->d_name);
            }
            else if (fileInfo.st_mode & S_IXUSR)
            {
                printf(EXEC_COLOR "%s" RESET "\n", namelist[i]->d_name);
            }
            else
            {
                printf(FILE_COLOR "%s" RESET "\n", namelist[i]->d_name);
            }
        }

        if (recursive && namelist[i]->d_type == DT_DIR)
        {
            if (namelist[i]->d_name[0] != '.')
            {
                char newPath[1024];
                snprintf(newPath, sizeof(newPath), "%s/%s", path, namelist[i]->d_name);
                printf("\n%s:\n", newPath);

                print_output(newPath, all, recursive, details, sort);
            }
        }
        free(namelist[i]);
    }

    free(namelist);
}

void print_help()
{
    FILE* file = fopen("./help.txt", "r");
    if (file == NULL)
    {
        perror("Error opening help.txt");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    fclose(file);
}

int main(int argc, char* argv[])
{
    int options;
    bool all = false, recursive = false, details = false, sort = false;
    const char* path = ".";

    static struct option long_options[] = {
        {"all", no_argument, 0, 'a'},
        {"long", no_argument, 0, 'l'},
        {"recursive", no_argument, 0, 'R'},
        {"time", no_argument, 0, 't'},
        {"help", no_argument, 0, 0},
        {0, 0, 0, 0} };

    while ((options = getopt_long(argc, argv, "alRth", long_options, NULL)) != -1)
    {
        switch (options)
        {
        case 'a':
            all = true;
            break;
        case 'R':
            recursive = true;
            break;
        case 'l':
            details = true;
            break;
        case 't':
            sort = true;
            break;
        case 0:
            print_help();
            exit(0);
        default:
            exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
        path = argv[optind];

    print_output(path, all, recursive, details, sort);
    return 0;
}