#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <sys/stat.h>
#include "tracker.h"


#define MAX_FILES 1000

typedef struct {
    char name[512];
    long long size;
    time_t modified;
} FileInfo;


int scan_directory_for_tracking(const char *path, FileInfo files[]) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[1024];
    int count = 0;

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) return 0;

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, findFileData.cFileName);

        struct stat st;
        if (stat(fullPath, &st) == 0) {
            strcpy(files[count].name, findFileData.cFileName);
            files[count].size = st.st_size;
            files[count].modified = st.st_mtime;
            count++;
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return count;
}


void track_changes(const char *path) {
    FileInfo oldFiles[MAX_FILES];
    FileInfo newFiles[MAX_FILES];
    int oldCount = 0, newCount = 0;

    
    FILE *fp = fopen("scan_log.txt", "r");
    if (fp) {
        while (fscanf(fp, "%s %lld %ld", oldFiles[oldCount].name, &oldFiles[oldCount].size, &oldFiles[oldCount].modified) == 3) {
            oldCount++;
        }
        fclose(fp);
    }

    
    newCount = scan_directory_for_tracking(path, newFiles);

    
    printf("\n==== Track Changes ====\n");
    for (int i = 0; i < newCount; i++) {
        int found = 0;
        for (int j = 0; j < oldCount; j++) {
            if (strcmp(newFiles[i].name, oldFiles[j].name) == 0) {
                found = 1;
                if (newFiles[i].size != oldFiles[j].size || newFiles[i].modified != oldFiles[j].modified) {
                    printf("* File modified: %s\n", newFiles[i].name);
                }
                break;
            }
        }
        if (!found) {
            printf("+ New file added: %s\n", newFiles[i].name);
        }
    }

    for (int i = 0; i < oldCount; i++) {
        int found = 0;
        for (int j = 0; j < newCount; j++) {
            if (strcmp(oldFiles[i].name, newFiles[j].name) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("- File removed: %s\n", oldFiles[i].name);
        }
    }

    
    fp = fopen("scan_log.txt", "w");
    for (int i = 0; i < newCount; i++) {
        fprintf(fp, "%s %lld %ld\n", newFiles[i].name, newFiles[i].size, newFiles[i].modified);
    }
    fclose(fp);

    printf("\n Tracking complete!\n");
}
