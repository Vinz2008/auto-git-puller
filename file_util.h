#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

int is_dir(const char *path);
char* get_filename(const char* path);