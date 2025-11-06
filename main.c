#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "organizer.h"
#include "tracker.h"
#include "search.h"
#include "report.h"

void main_menu();
void scan_menu();

int main() {
    main_menu();
    return 0;
}

void main_menu() {
    int choice;

    while (1) {
        printf("\n==== Smart File Organizer and Tracker ====\n");
        printf("1. Scan Directory\n");
        printf("2. Organize Files\n");
        printf("3. Track Changes\n");
        printf("4. Search Files\n");
        printf("5. Generate Report\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                scan_menu();
                break;
            case 2:
                {
                    char path[512];
                    printf("Enter directory path to organize: ");
                    scanf("%s", path);
                    organize_files(path);
                    break;
                }
                
            case 3:
                {
                    char path[512];
                    printf("Enter directory path to track changes: ");
                    scanf("%s", path);
                    track_changes(path);
                    break;
                }
            case 4:
                {
                    char path[512], keyword[128];
                    printf("Enter directory path to search: ");
                    scanf("%s", path);
                    printf("Enter file name keyword to search: ");
                    scanf("%s", keyword);
                    printf("\nSearching for \"%s\" in %s ...\n", keyword, path);
                    search_files(path, keyword);
                    break;
                }
            case 5:
                {
                    char path[512];
                    printf("Enter directory path to generate report: ");
                    scanf("%s", path);
                    generate_report(path);
                    break;
                }
            case 6:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}

void scan_menu() {
    int choice;
    char path[512];

    printf("\nEnter directory path to scan: ");
    scanf("%s", path);

    while (1) {
        printf("\n==== Scan Menu ====\n");
        printf("1. Show only Folders\n");
        printf("2. Show only Files\n");
        printf("3. Show Both (Folders + Files)\n");
        printf("4. Back to Main Menu\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                scan_directory(path, 1);
                break;
            case 2:
                scan_directory(path, 2); 
                break;
            case 3:
                scan_directory(path, 3);
                break;
            case 4:
                return; 
            case 5:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}
