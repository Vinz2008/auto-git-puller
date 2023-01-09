#include "config_file.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int removeCharFromString(char charToRemove, char *str){
	int i,j;
	i = 0;
	while(i<strlen(str))
	{
    	if (str[i]==charToRemove)
    	{
        for (j=i; j<strlen(str); j++) {
            str[j]=str[j+1];
        }
    	}
    	else i++;
	}
	return 0;
}

int find_parameter_pos(const char* name, struct config_file* config){
    int pos = -1;
    for (int i = 0; i < config->used; i++){
        if (strcmp(name, config->parameters[i].name) == 0){
            pos = i;
            break;
        }
    }
    if (pos == -1){
        printf("ERROR : parameter %s could not be found\n", name);
        exit(1);
    }

    return pos;
}

void init_config_list(struct config_file* config, size_t initialSize){
    config->parameters = malloc(initialSize * sizeof(struct parameter));
    config->used = 0;
    config->size = initialSize;
}

void append_parameter_config_list(struct parameter param, struct config_file* config){
    if (config->used == config->size){
        config->size *= 2;
        config->parameters = realloc(config->parameters, config->size * sizeof(struct parameter));
    }
    config->parameters[config->used++] = param;
}

void empty_config_list(struct config_file* config){
    free(config->parameters);
    config->parameters = NULL;
    config->used = config->size = 0;
}

struct parameter* parse_config_line(char* line){
    removeCharFromString('\n', line);
    struct parameter* parameter_ptr = malloc(sizeof(struct parameter));
    bool equalFound = false;
    char* name = malloc(strlen(line) * sizeof(char));
    char* value = malloc(strlen(line) * sizeof(char));;
    //char* value = malloc(20 * sizeof(char));
    memset(name, 0, sizeof(name));
    memset(value, 0, sizeof(value));
    for (int i = 0; i < strlen(line); i++){
        if (line[i] == '='){
            equalFound = true;
        } else {
            if (equalFound){
               strncat(value, line + i, 1);
            } else {
                strncat(name, line + i, 1);
            }
        }
    }
    parameter_ptr->name = name;
    parameter_ptr->value_str = value;
    return parameter_ptr;
}

struct config_file* parse_config_file(char* path){
    if (path == NULL){
        struct config_file* config_file_ptr_empty = malloc(sizeof(struct config_file));
        init_config_list(config_file_ptr_empty, 1);
        struct parameter* temp_param = malloc(sizeof(struct parameter));
        temp_param->name = "search_folder_starting_with_point";
        temp_param->value_str = "false";
        append_parameter_config_list(*temp_param, config_file_ptr_empty);
        free(temp_param);
        return config_file_ptr_empty;
    }
    struct config_file* config_file_ptr = malloc(sizeof(struct config_file));
    init_config_list(config_file_ptr, 1);
    config_file_ptr->path = path;
    char* line = malloc(sizeof(char)*100);
    FILE* f = fopen(path, "r");
    while (fgets(line, 100, f) != NULL){
        struct parameter* temp_param = parse_config_line(line);
        append_parameter_config_list(*temp_param, config_file_ptr);
        free(temp_param);
    }
    fclose(f);
    free(line);
    return config_file_ptr;
}

config_t* create_config(struct config_file* f){
    config_t* config = malloc(sizeof(config_t));
    char* bool_search_folder_point_str = f->parameters[find_parameter_pos("search_folder_starting_with_point", f)].value_str;
    if (strcmp("true", bool_search_folder_point_str) == 0){
        config->search_folder_starting_point = true;
    } else if (strcmp("false", bool_search_folder_point_str) == 0){
        config->search_folder_starting_point = false;
    } else {
        printf("parameter for search_folder_starting_with_point isn't a boolean : %s\n", bool_search_folder_point_str);
    }
    return config;
}