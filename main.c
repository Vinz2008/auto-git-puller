#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "file_util.h"
#include "config_file.h"
#include "git.h"

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif


int main(int argc, char **argv){
    git_init();
    struct config_file* config_f = NULL;
    if (access("/etc/auto-git-pull/auto-git-pull.conf", F_OK) == 0) {
    config_f = parse_config_file("/etc/auto-git-pull/auto-git-pull.conf");
    } else if (access("./auto-git-pull.conf", F_OK) == 0){
    config_f = parse_config_file("./auto-git-pull.conf");
    } else {
    config_f = parse_config_file(NULL);
    }
    config_t* config =  create_config(config_f);
    empty_config_list(config_f);
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
            printf("TEST\n");
            char* folder_name = get_filename(path_name);
            if (folder_name[0] != '.' || config->search_folder_starting_point == true){
            if (is_dir(path_name) /*!= 0*/ == 16385){
                //printf("%s is_directory\n", path_name);
                printf("TEST3\n");
                if (is_git_repo(path_name)){
                    printf("TEST4\n");
                    printf("%s is git repo\n", path_name);
                    git_pull(path_name);
                }
            }
            }
            free(folder_name);
            printf("TEST2\n");
            free(path_name); 
        }
    }
    closedir(dr);
}