#include <stdio.h>


int main(){
    
    char path[250];
    printf("Enter the directory path to scan: ");
    scanf("%s",&path);


    scan_directory(path);
    return 0;
}