#include <stdio.h>
#include <windows.h>
#include <sys/stat.h>
#include <time.h>
#include "scanner.h"


long long get_folder_size(const char *path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[1024];
    long long totalSize = 0;

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) return 0;

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            totalSize += get_folder_size(fullPath); 
        } else {
            struct stat fileStat;
            if (stat(fullPath, &fileStat) == 0)
                totalSize += fileStat.st_size;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return totalSize;
}

void scan_directory(const char *path, int option) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = NULL;
    char searchPath[1024];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);

    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: Unable to open directory %s\n", path);
        return;
    }

    printf("\n%-30s %-15s %-20s %-20s\n", "Name", "Size(B)", "Type", "Last Modified");
    printf("-----------------------------------------------------------------------------\n");

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, findFileData.cFileName);

        struct stat fileStat;
        if (stat(fullPath, &fileStat) == -1) {
            perror("stat");
            continue;
        }

        int isFolder = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;

        
        if ((option == 1 && !isFolder) ||      
            (option == 2 && isFolder)) {      
            continue; 
        }

        
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_mtime));

        if (isFolder) {
            long long folderSize = get_folder_size(fullPath); 
            printf("[Folder] %-23s %-15lld %-20s %-20s\n",
                   findFileData.cFileName, folderSize, "Directory", timebuf);
        } else {
            char *ext = strrchr(findFileData.cFileName, '.');
            printf("%-30s %-15lld %-20s %-20s\n",
                   findFileData.cFileName,
                   (long long)fileStat.st_size,
                   ext ? ext + 1 : "Unknown",
                   timebuf);
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}
