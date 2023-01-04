#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "file_util.h"
#include "git.h"

int main(int argc, char **argv){
    git_init();
    char* directory = ".";
    for (int i = 1; i < argc; i++) {
        directory = argv[i];
    }

    struct dirent *de;
    DIR *dr = opendir(directory);
    if (dr == NULL){
       printf("Could not open current directory" );
       exit(1);
    }
    while ((de = readdir(dr)) != NULL){
        if (strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0 ){
            char* path_name = malloc((strlen(directory) + strlen(de->d_name) + 3) * sizeof(char));
            memset(path_name, 0, strlen(path_name));
            strncpy(path_name, directory, strlen(directory));
            strncat(path_name, SEPARATOR, 2);
            strncat(path_name, de->d_name, strlen(de->d_name));
            if (is_dir(path_name) /*!= 0*/ == 16385){
                //printf("%s is_directory\n", path_name);
                if (is_git_repo(path_name)){
                    printf("%s is git repo\n", path_name);
                    git_pull(path_name);
                }
            }
            free(path_name); 
        }
    }
    closedir(dr);
}