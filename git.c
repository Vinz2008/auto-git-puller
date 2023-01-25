#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <git2/global.h>
#include <git2/remote.h>
#include <git2/repository.h>
#include <git2/annotated_commit.h>


bool is_git_repo(const char* path){
    struct dirent *de;
    DIR *dr = opendir(path);
    while ((de = readdir(dr)) != NULL){
        if (strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0 ){
            if (strcmp(".git", de->d_name) == 0){
                closedir(dr);
                return true;
            }
        }
    }
    closedir(dr);
    return false;
}

int run_command(const char* command){
    FILE* cmdf = popen(command, "r");
    char c;
    while ((c = fgetc(cmdf)) != EOF){
        putc(c, stdout);
    }
    pclose(cmdf);
    return 0;
}

int git_pull(const char* path){
    char* cmd = malloc((strlen("cd ") + strlen(path) + strlen(" && git pull") + 1) * sizeof(char));
    strcpy(cmd, "cd ");
    strncat(cmd, path, strlen(path));
    strncat(cmd, " && git pull", strlen(" && git pull") + 1);
    run_command(cmd);
    free(cmd);
    return 0;
}