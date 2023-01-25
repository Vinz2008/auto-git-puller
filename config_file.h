#include <stdbool.h>
#include <stddef.h>

typedef struct {
    bool search_folder_starting_point;
    bool run_every_directory;
} config_t;

struct parameter {
    char* name;
    char* value_str;
};

struct config_file {
    char* path;
    struct parameter* parameters;
    size_t used;
    size_t size;
};

struct config_file* parse_config_file(char* path);
config_t* create_config(struct config_file* f);
void empty_config_list(struct config_file* config);