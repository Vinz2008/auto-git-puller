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


// TODO : add silent mode to not print the output of git pull
int run_command(const char* command){
    FILE* cmdf = popen(command, "r");
    char c;
    while ((c = fgetc(cmdf)) != EOF){
        putchar(c);
    }
    pclose(cmdf);
    return 0;
}

int git_pull(const char* path){
    // TODO : add quotes to the path to accept characters like ()
    char* cmd = malloc((strlen("cd ") + strlen(path) + strlen(" && git pull") + 1) * sizeof(char));
    strcpy(cmd, "cd ");
    strncat(cmd, path, strlen(path));
    strncat(cmd, " && git pull", strlen(" && git pull") + 1);
    run_command(cmd);
    free(cmd);
    return 0;
}