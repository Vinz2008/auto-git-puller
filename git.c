#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <git2/global.h>
#include <git2/remote.h>
#include <git2/repository.h>
#include <git2/annotated_commit.h>

int git_init(){
    git_libgit2_init();
    return 0;
}

bool is_git_repo(const char* path){
    return git_repository_open_ext(NULL, path, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) == 0;
}

int git_pull_libgit2(const char* path){
    git_repository *repo = NULL;
    git_remote *remote;
    git_repository_open(&repo, path);
    git_remote_lookup( &remote, repo, "origin" );
    git_fetch_options options = GIT_FETCH_OPTIONS_INIT;
    git_remote_fetch(remote, NULL, &options, "pull");
    git_annotated_commit* annotated_commit = NULL;
    git_annotated_commit_from_fetchhead(&annotated_commit, repo, "origin", NULL, NULL);
    return 0;
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
    char* cmd = malloc((strlen("cd ") + strlen(path) + strlen("&& git pull")) * sizeof(char));
    strcpy(cmd, "cd ");
    strncat(cmd, path, strlen(path));
    strncat(cmd, "&& git pull", strlen("&& git pull") + 1);
    run_command(cmd);
    free(cmd);
    return 0;
}