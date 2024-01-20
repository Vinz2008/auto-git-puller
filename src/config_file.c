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
    memset(name, 0, strlen(line));
    memset(value, 0, strlen(line));
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

void append_parameter_config_file_empty(struct config_file* config_f, const char* name, const char* value){
    struct parameter* temp_param = malloc(sizeof(struct parameter));
    temp_param->name = strdup(name);
    temp_param->value_str = strdup(value);
    append_parameter_config_list(*temp_param, config_f);
    free(temp_param);
}

struct config_file* parse_config_file(char* path){
    if (path == NULL){
        struct config_file* config_file_ptr_empty = malloc(sizeof(struct config_file));
        init_config_list(config_file_ptr_empty, 1);
        append_parameter_config_file_empty(config_file_ptr_empty, "search_folder_starting_with_point", "false");
        append_parameter_config_file_empty(config_file_ptr_empty, "run_every_directory", "false");
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

void create_config_struct_member_bool(struct config_file* f, const char* parameter_name, bool* bool_to_set){
    char* temp_str = f->parameters[find_parameter_pos(parameter_name, f)].value_str;
    if (strcmp("true", temp_str) == 0){
        *bool_to_set = true;
    } else if (strcmp("false", temp_str) == 0){
        *bool_to_set = false;
    } else {
        printf("parameter for %s isn't a boolean : %s\n",parameter_name, temp_str);
    }

}

config_t* create_config(struct config_file* f){
    config_t* config = malloc(sizeof(config_t));
    create_config_struct_member_bool(f, "search_folder_starting_with_point", &config->search_folder_starting_point);
    create_config_struct_member_bool(f, "run_every_directory", &config->run_every_directory);
    return config;
}