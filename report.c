#include <stdio.h>
#include <windows.h>
#include <sys/stat.h>
#include <time.h>

long long get_folder_size_report(const char *path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];
    long long totalSize = 0;

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) return 0;

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        char fullPath[MAX_PATH];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            totalSize += get_folder_size_report(fullPath);
        } else {
            struct stat st;
            if (stat(fullPath, &st) == 0)
                totalSize += st.st_size;
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return totalSize;
}

void count_files_and_folders(const char *path, int *fileCount, int *folderCount) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        char fullPath[MAX_PATH];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            (*folderCount)++;
            count_files_and_folders(fullPath, fileCount, folderCount);
        } else {
            (*fileCount)++;
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void generate_report(const char *path) {
    int totalFiles = 0, totalFolders = 0;
    long long totalSize = 0;

    count_files_and_folders(path, &totalFiles, &totalFolders);
    totalSize = get_folder_size_report(path);

    time_t now = time(NULL);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    FILE *fp = fopen("report.txt", "w");
    if (!fp) {
        printf("Failed to create report.txt\n");
        return;
    }

    printf("\n==== Directory Report for: %s ====\n", path);
    printf("Total Folders : %d\n", totalFolders);
    printf("Total Files   : %d\n", totalFiles);
    printf("Total Size    : %.2f MB\n", totalSize / (1024.0 * 1024.0));
    printf("Report Time   : %s\n", timeStr);

    fprintf(fp, "==== Directory Report for: %s ====\n", path);
    fprintf(fp, "Total Folders : %d\n", totalFolders);
    fprintf(fp, "Total Files   : %d\n", totalFiles);
    fprintf(fp, "Total Size    : %.2f MB\n", totalSize / (1024.0 * 1024.0));
    fprintf(fp, "Report Time   : %s\n", timeStr);

    fclose(fp);
    printf("\nReport saved as report.txt\n");
}
