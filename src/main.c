#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "./cJSON.h"
#include "cJSON.h"

#define CONFIG_PATH "./.prog/c.json"

void create_project_structure(int use_template) {
    system("mkdir -p src build assets .prog");
    if (use_template) {
        system("cp /opt/Progstart/c.json .prog/");
    }
    system("git init && git add . && git commit -m 'the start'");
}

cJSON *load_config(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Failed to open file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);

    cJSON *json = cJSON_Parse(data);
    free(data);

    if (!json) {
        printf("Error parsing JSON\n");
        exit(EXIT_FAILURE);
    }

    return json;
}

void compile_c_project(cJSON *config) {
    cJSON *comp = cJSON_GetObjectItem(config, "comp");
    cJSON *srcFiles = cJSON_GetObjectItem(comp, "srcFiles");
    cJSON *compiler = cJSON_GetObjectItem(comp, "compiler");
    cJSON *buildPath = cJSON_GetObjectItem(comp, "buildPath");
    cJSON *compilerArgs = cJSON_GetObjectItem(comp, "Compileratgs");
    cJSON *srcBasePath = cJSON_GetObjectItem(comp, "srcBassPath");

    char command[1024] = {0};
    strcat(command, compiler->valuestring);
    strcat(command, " -o ");
    strcat(command, buildPath->valuestring);

    int srcFilesCount = cJSON_GetArraySize(srcFiles);
    for (int i = 0; i < srcFilesCount; ++i) {
        strcat(command, " ");
        strcat(command, srcBasePath->valuestring);
        strcat(command, cJSON_GetArrayItem(srcFiles, i)->valuestring);
    }

    strcat(command, " ");
    strcat(command, compilerArgs->valuestring);

    printf("Compiling with command: %s\n", command);
    system(command);
}

void run_or_test_c_project(cJSON *config) {
    cJSON *comp = cJSON_GetObjectItem(config, "comp");
    cJSON *test = cJSON_GetObjectItem(config, "Test");
    cJSON *buildPath = cJSON_GetObjectItem(comp, "buildPath");

    if (access(buildPath->valuestring, F_OK) == -1) {
        compile_c_project(config);
    }

    cJSON *args = cJSON_GetObjectItem(test, "args");
    cJSON *prefix = cJSON_GetObjectItem(test, "prefix");

    char command[1024] = {0};
    strcat(command, prefix->valuestring);
    strcat(command, " ");
    strcat(command, buildPath->valuestring);

    int argsCount = cJSON_GetArraySize(args);
    for (int i = 0; i < argsCount; ++i) {
        strcat(command, " ");
        strcat(command, cJSON_GetArrayItem(args, i)->valuestring);
    }

    printf("Running with command: %s\n", command);
    system(command);
}
void install(cJSON *config){
  cJSON *Istaller = cJSON_GetObjectItem(config, "Installer");
  cJSON *command = cJSON_GetObjectItem(Istaller, "command");
  char *installcammand = cJSON_GetStringValue(command);

  if (!system(installcammand)) {
    fprintf(stderr, "you must run this as root or with sudo");
    exit(1);
  }
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./prog -r|-n|-T [options]\n");
        return 1;
    }

    if (strcmp(argv[1], "-n") == 0) {
        if (argc >= 3 && strcmp(argv[2], "-t") == 0) {
            create_project_structure(1);
        } else {
            create_project_structure(0);
        }
    } else if (strcmp(argv[1], "-r") == 0) {
        cJSON *config = load_config(CONFIG_PATH);
        compile_c_project(config);
        cJSON_Delete(config);
    } else if (strcmp(argv[1], "-T") == 0) {
        cJSON *config = load_config(CONFIG_PATH);
        run_or_test_c_project(config);
        cJSON_Delete(config);
    } else if(strcmp(argv[1], "-I" == 0){
        cJSON *config = load_config(CONFIG_PATH);
        install(conifg);
    }

    return 0;
}
