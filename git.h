#include <stdbool.h>

int git_init();
int git_pull(const char* path);
bool is_git_repo(const char* path);