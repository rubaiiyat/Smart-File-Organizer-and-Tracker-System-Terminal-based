#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <direct.h> 
#include "organizer.h"


void create_folder_if_not_exists(const char *folderName) {
    DWORD attrib = GetFileAttributes(folderName);
    if (attrib == INVALID_FILE_ATTRIBUTES || !(attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        _mkdir(folderName);
    }
}


void move_file(const char *source, const char *destinationFolder) {
    char destPath[1024];
    snprintf(destPath, sizeof(destPath), "%s\\%s", destinationFolder, strrchr(source, '\\') + 1);

    if (!MoveFile(source, destPath)) {
        printf("❌ Failed to move: %s\n", source);
    }
}


void organize_files(const char *inputPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = NULL;
    char searchPath[1024];
    char path[1024];

   
    strcpy(path, inputPath);
    int len = strlen(path);
    if (len == 2 && path[1] == ':') strcat(path, "\\");

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: Unable to open directory %s\n", path);
        return;
    }

    printf("\nOrganizing files in directory: %s\n\n", path);

    int fileCount = 0;
    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

      
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, findFileData.cFileName);

       
        char *ext = strrchr(findFileData.cFileName, '.');
        if (!ext) ext = "";

        char category[256];
        if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".png") == 0 || strcmp(ext, ".jpeg") == 0)
            strcpy(category, "Images");
        else if (strcmp(ext, ".mp3") == 0 || strcmp(ext, ".wav") == 0)
            strcpy(category, "Music");
        else if (strcmp(ext, ".mp4") == 0 || strcmp(ext, ".mkv") == 0)
            strcpy(category, "Videos");
        else if (strcmp(ext, ".pdf") == 0 || strcmp(ext, ".doc") == 0 || strcmp(ext, ".docx") == 0)
            strcpy(category, "Documents");
        else if (strcmp(ext, ".txt") == 0)
            strcpy(category, "Text");
        else if (strcmp(ext, ".c") == 0 || strcmp(ext, ".cpp") == 0 || strcmp(ext, ".py") == 0)
            strcpy(category, "Code");
        else
            strcpy(category, "Others");

      
        char newFolderPath[1024];
        snprintf(newFolderPath, sizeof(newFolderPath), "%s\\%s", path, category);
        create_folder_if_not_exists(newFolderPath);

       
        move_file(fullPath, newFolderPath);
        printf("Moved: %-30s -> %s\n", findFileData.cFileName, category);

        fileCount++;

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (fileCount == 0)
        printf("No files found to organize.\n");
    else
        printf("\n[Ok] Organization complete! %d files moved.\n\n", fileCount);
}
