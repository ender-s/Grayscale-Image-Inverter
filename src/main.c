#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "complement.h"
#include "constants.h"
#include "utils.h"

int check_before_operation(int, char**, char**, char**, int*, int*);
void print_usage(const char*);

int main(int argc, char* argv[])
{
    int result;

    char* output_path_pgm;
    char* output_path_bmp;
    int create_pgm_file;
    int create_bmp_file;

    result = check_before_operation(argc, argv, &output_path_pgm, &output_path_bmp,
                                        &create_pgm_file, &create_bmp_file);
    if (result != 0)
        return result;

    result = complement_and_write_image(argv[1], output_path_pgm, output_path_bmp, 
                                           create_pgm_file, create_bmp_file);
    
    if (result != 0)
    {
        free(output_path_pgm);
        free(output_path_bmp);
        return result;
    }

    free(output_path_pgm);
    free(output_path_bmp);

    return 0;
}

int check_before_operation(int argc, char** argv, char** output_path_pgm, char** output_path_bmp,
                            int* create_pgm_file, int* create_bmp_file)
{
    if(argc != 3)
    {
        print_usage(argv[0]);
        return WRONG_CMD_ARGS_FORMAT;
    }

    // if the given input path does not exist
    if (!check_if_file_exists(argv[1]))
    {
        printf("The given input path does not exist!: %s\n", argv[1]);
        return INPUT_PATH_DOES_NOT_EXIST;
    }

    int output_image_name_length = strlen(argv[2]);
    int pgm_bmp_name_size = output_image_name_length + 4 + 1; // + 1 for the null char '\0'

    *output_path_pgm = (char*) malloc(pgm_bmp_name_size * sizeof(char));
    *output_path_bmp = (char*) malloc(pgm_bmp_name_size * sizeof(char));

    strcpy(*output_path_pgm, argv[2]);
    strcat(*output_path_pgm, ".pgm");

    strcpy(*output_path_bmp, argv[2]);
    strcat(*output_path_bmp, ".bmp");

    int pgm_file_exists = check_if_file_exists(*output_path_pgm);
    int bmp_file_exists = check_if_file_exists(*output_path_bmp);

    if (pgm_file_exists && bmp_file_exists)
    {
        printf("Both of the following paths exist!:\n");
        printf("%s\n", *output_path_pgm);
        printf("%s\n", *output_path_bmp);
        return PGM_BMP_OUTPUT_PATHS_EXIST;
    }

    *create_pgm_file = 1;
    *create_bmp_file = 1;

    if (pgm_file_exists)
    {
        printf("[WARNING] The following path already exists!: %s\n", *output_path_pgm);
        printf("[WARNING] pgm output will not be created!");
        *create_pgm_file = 0;
    }

    if (bmp_file_exists)
    {
        printf("[WARNING] The following path already exists!: %s\n", *output_path_bmp);
        printf("[WARNING] bmp output will not be created!");
        *create_bmp_file = 0;
    }

    return 0;
}

void print_usage(const char* program_path)
{
    printf("Usage: %s <input_image_path> <output_image_name>\n", program_path);
}