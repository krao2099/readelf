#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include "debug.h"

#define NUM_MAN_ARGS 1
#define NUM_OPT_ARGS 0
#define NUM_TOTAL_ARGS NUM_MAN_ARGS + NUM_OPT_ARGS

int check_elf_header(char * buf, off_t size) {
    if (size < sizeof(Elf64_Ehdr)) {
        return -1;
    }
    if (buf[0] == EI_MAG0 && buf[1]==EI_MAG1 && buf[2] == EI_MAG2 && buf[3]==EI_MAG3) {
        return 0;
    }
    return 1;
}


int main(int argc, char **argv) {
    if (argc - 1 < NUM_MAN_ARGS || argc - 1 > NUM_TOTAL_ARGS) {
        fprintf(stderr, "Incorrect number of args\n");
        exit(1);
    }
    char* file_name = argv[argc - 1];
    char file_path[FILENAME_MAX];
    if (getcwd(file_path, FILENAME_MAX) == NULL) {
        perror("PWD Failure");
        exit(0);
    }
    int path_len = strlen(file_path);

    //Unsafe copy, cannot break buf due to kernel path max
    file_path[path_len] = '/';
    strcpy(&file_path[path_len+1], file_name);

    debug_printf("Target file: %s\n", file_path);

    if (access(file_path,F_OK) != 0) {
        printf("Unable to find %s\n", file_path);
        exit(1);
    }
    if (access(file_path,R_OK) != 0) {
        printf("No read permissions for %s\n", file_path);
        exit(1);
    }
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }
    struct stat statbuf;
    if (fstat(fd, &statbuf) == -1) {
        perror("Error with fstat:");
        exit(1);
    }
    char * file_buf = malloc(statbuf.st_size);
    if (file_buf == NULL) {
        printf("OOM\n");
        exit(1);
    }
    if (read(fd, file_buf, statbuf.st_size) != statbuf.st_size) {
        printf("Failed to read file into memory\n");
        exit(0);
    }

    if (check_elf_header(file_buf, statbuf.st_size) == -1) {
        printf("Not an Elf file\n");
        exit(1);
    }
    debug_printf("%s is an Elf File!!!\n", file_name);

    free(file_buf);
    close(fd);
}