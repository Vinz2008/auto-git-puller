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

    struct dirent *de;  // Pointer for directory entry
 
    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(directory);

    if (dr == NULL){
       printf("Could not open current directory" );
       exit(1);
    }
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL){
        if (strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0 ){
            //printf("%s\n", de->d_name);
            //printf("size : %ld\n",(strlen(directory) + strlen(de->d_name) + 3) * sizeof(char));
            char* path_name = malloc((strlen(directory) + strlen(de->d_name) + 3) * sizeof(char));
            memset(path_name, 0, strlen(path_name));
            strncpy(path_name, directory, strlen(directory));
            strncat(path_name, SEPARATOR, 2);
            strncat(path_name, de->d_name, strlen(de->d_name));
            //printf("is_dir %s %d\n", path_name, is_dir(path_name));
            if (is_dir(path_name) /*!= 0*/ == 16385){
                //printf("%s is_directory\n", path_name);
                if (is_git_repo(path_name)){
                printf("%s is git repo\n", path_name);
                git_pull(path_name);
                }
                /*DIR *dir_temp = opendir(path_name);
                struct dirent *de_temp;
                while ((de_temp = readdir(dir_temp)) != NULL){
                    if (strcmp(".", de_temp->d_name) != 0 && strcmp("..", de_temp->d_name) != 0){
                        printf("%s/%s\n", path_name, de_temp->d_name);
                        if (strcmp(".git", de_temp->d_name) == 0){
                            char* git_pull_path = malloc((strlen(path_name) + strlen(de_temp->d_name) + 1) * sizeof(char));
                            strncpy(git_pull_path, path_name, strlen(path_name));
                            strncat(git_pull_path, SEPARATOR, 2);
                            strncat(git_pull_path, de_temp->d_name, strlen(de_temp->d_name));
                            git_pull(git_pull_path);
                            printf("FREE GIT pull path\n");
                            free(git_pull_path);
                        }
                    }
                }
                closedir(dir_temp);*/
            }
            //printf("FREE PATH_NAME path\n");
            free(path_name); 
        }
    }
    closedir(dr);
}