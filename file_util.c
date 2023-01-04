#include <sys/stat.h>
#include <stdbool.h>

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