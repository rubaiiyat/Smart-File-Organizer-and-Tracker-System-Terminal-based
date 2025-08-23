#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

void organize_files();
void track_files();
void search_files();
void generate_report();

void show_menu() {
    printf("\n===== Smart File Organizer and Tracker System =====\n");
    printf("1. Scan Directory\n");
    printf("2. Organize Files\n");
    printf("3. Track File Changes\n");
    printf("4. Search Files\n");
    printf("5. Generate Report\n");
    printf("6. Exit\n");
    printf("==================================================\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    char path[512];

    while (1) {
        show_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter directory path to scan: ");
                scanf("%s", path);
                scan_directory(path);
                break;

            case 2:
                organize_files();
                break;

            case 3:
                track_files();
                break;

            case 4:
                search_files();
                break;

            case 5:
                generate_report();
                break;

            case 6:
                printf("Exiting... Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void organize_files() {
    printf("[Organizer Module is under development]\n");
}
void track_files() {
    printf("[Tracker Module is under development]\n");
}
void search_files() {
    printf("[Search Module is under development]\n");
}
void generate_report() {
    printf("[Report Generator Module is under development]\n");
}
