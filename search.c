#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void search_files_recursive(const char *path, const char *filename, int *foundCount) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);
    hFind = FindFirstFile(searchPath, &findFileData);

    
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do {
        const char *name = findFileData.cFileName;

        
        if (findFileData.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
            continue;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        char fullPath[MAX_PATH];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, name);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            
            search_files_recursive(fullPath, filename, foundCount);
        } else {
            if (strstr(name, filename) != NULL) {
                printf("Found: %s\n", fullPath);
                (*foundCount)++;
            }
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void search_files(const char *path, const char *filename) {
    int foundCount = 0;
    printf("\n===== Searching for \"%s\" in \"%s\" =====\n\n", filename, path);
    search_files_recursive(path, filename, &foundCount);

    if (foundCount == 0)
        printf("No files found matching \"%s\".\n", filename);
    else
        printf("\nTotal matches found: %d\n", foundCount);
}
