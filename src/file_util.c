#include <sys/stat.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void append_char(char c, char* str){
    str[strlen(str)] = c;
}

bool is_dir(const char* path){
#ifdef _WIN32
    struct _stat statbuf;
    if (_stat(path, &statbuf) != 0)
#else
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
#endif
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

const char* get_filename(const char* path){
    char* ret = malloc(strlen(path) * sizeof(char));
    for (int i = 0; i < strlen(path); i++){
        if (path[i] == '/'){
            memset(ret, 0, strlen(path) * sizeof(char));
        } else {
            append_char(path[i], ret);
        }
    }
    return ret;
}