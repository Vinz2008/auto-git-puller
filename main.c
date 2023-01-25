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

#define CONFIG_PATH_LIST_LENGTH 2

char* config_path_list[CONFIG_PATH_LIST_LENGTH] = {
    "/etc/auto-git-pull/auto-git-pull.conf",
    "./auto-git-pull.conf"
};

int main(int argc, char **argv){
    struct config_file* config_f = NULL;
    for (int i = 0; i < CONFIG_PATH_LIST_LENGTH; i++){
        if (access(config_path_list[i], F_OK) == 0){
            config_f = parse_config_file(config_path_list[i]);
            printf("Config file : %s\n", config_path_list[i]);
        }
    }
    if (config_f == NULL){
        printf("No config file found. Using defaults\n");
        config_f = parse_config_file(NULL);
    }
    /*if (access("/etc/auto-git-pull/auto-git-pull.conf", F_OK) == 0) {
    config_f = parse_config_file("/etc/auto-git-pull/auto-git-pull.conf");
    } else if (access("./auto-git-pull.conf", F_OK) == 0){
    config_f = parse_config_file("./auto-git-pull.conf");
    } else {
    config_f = parse_config_file(NULL);
    }*/
    config_t* config =  create_config(config_f);
    empty_config_list(config_f);
    char* directory = ".";
    for (int i = 1; i < argc; i++) {
        if (strcmp("--all-directories", argv[i]) == 0){
            config->run_every_directory = true;
        } else if (strcmp("--with-point-starting-folder", argv[i]) == 0) {
            config->search_folder_starting_point = true;
        } else {
            directory = argv[i];
        }

    }

    struct dirent *de;
    DIR *dr = opendir(directory);
    if (dr == NULL){
       printf("Could not open current directory : %s\n", directory);
       exit(1);
    }
    while ((de = readdir(dr)) != NULL){
        if (strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0 ){
            char* path_name = malloc((strlen(directory) + strlen(de->d_name) + 3) * sizeof(char));
            memset(path_name, 0, strlen(path_name));
            strncpy(path_name, directory, strlen(directory));
            strncat(path_name, SEPARATOR, 2);
            strncat(path_name, de->d_name, strlen(de->d_name));
            char* folder_name = get_filename(path_name);
            if (folder_name[0] != '.' || config->search_folder_starting_point == true){
            if (is_dir(path_name) /*!= 0*/ == 16385){
                //printf("%s is_directory\n", path_name);
                if (is_git_repo(path_name) || config->run_every_directory == true){
                    printf("%s is git repo\n", path_name);
                    git_pull(path_name);
                }
            }
            }
            free(folder_name);
            free(path_name); 
        }
    }
    closedir(dr);
}