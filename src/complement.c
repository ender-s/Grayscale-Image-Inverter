#include <stdio.h>
#include <stdlib.h>
#include "bmp_info.h"
#include "complement.h"
#include "dynamic.h"
#include "arraytoimage.h"
#include "constants.h"

int complement_and_write_image(const char* input_filename, const char* output_pgm_filename, const char* output_bmp_filename,
                                int create_pgm_file, int create_bmp_file)
{
    printf("Opening input file named %s in rb mode... ", input_filename);
    FILE* f = fopen(input_filename, "rb");
    if (f == NULL)
    {
        printf("ERROR!: Could not open the file in rb mode.\n");
        return COULD_NOT_OPEN_FILE;
    }
    else
        printf("DONE!\n");

    uint8_t width_header[WIDTH_INFO_LENGTH];
    uint8_t height_header[HEIGHT_INFO_LENGTH];
    uint8_t offset_header[OFFSET_INFO_LENGTH];
    uint8_t depth_header[DEPTH_INFO_LENGTH];

    read_header(f, width_header, WIDTH_START_POS, WIDTH_END_POS);
    long width = calculate_value(width_header, WIDTH_INFO_LENGTH, PIXEL_MAX_VAL);

    read_header(f, height_header, HEIGHT_START_POS, HEIGHT_END_POS);
    long height = calculate_value(height_header, HEIGHT_INFO_LENGTH, PIXEL_MAX_VAL);

    read_header(f, offset_header, OFFSET_START_POS, OFFSET_END_POS);
    long offset_byte_index = calculate_value(offset_header, OFFSET_INFO_LENGTH, PIXEL_MAX_VAL);

    read_header(f, depth_header, DEPTH_START_POS, DEPTH_END_POS);
    long bit_depth = calculate_value(depth_header, DEPTH_INFO_LENGTH, PIXEL_MAX_VAL);

    printf("Information Regarding The Image %s:\n", input_filename);
    printf("Width of the image is %ld\n", width);
    printf("Height of the image is %ld\n", height);
    printf("Bit depth of the image is %ld\n", bit_depth);

    int cont;

    if (bit_depth == 8)
        cont = 1;
    else
    {
        cont = 0;
        printf("[ERROR] The images only with bit-depth 8 are allowed! Halting...\n");
    }

    if (cont == 0)
        return UNSUPPORTED_BIT_DEPTH;

    long n = count_bytes(f, BUFFER_SIZE);

    int padded_width = width;
    while (padded_width % PADDING_FACTOR != 0)
        padded_width++;

    int pad_per_row = padded_width - width;

    printf("Pad per row is %d\n", pad_per_row);

    uint8_t** matrix = allocate_memory_dynamically(width, height);

    printf("Reading pixels in the image into a %ldx%ld 2D matrix...", width, height);
    read_pixels_into_matrix(f, matrix, width, height, n, offset_byte_index, pad_per_row);
    printf(" DONE!\n");

    printf("Complementing the pixels...");
    complement_matrix(matrix, height, width, PIXEL_MAX_VAL);
    printf(" DONE!\n");

    if (create_pgm_file)
    {
        printf("Writing the complemented version of the pixels into the file %s in pgm format...", output_pgm_filename);
        array_to_image((uint8_t*) matrix, width, height, output_pgm_filename);
        printf(" DONE!\n");
    }

    if (create_bmp_file)
    {
        printf("Writing the complemented version of the image into the file %s in bmp format...", output_bmp_filename);
        create_image(f, matrix, height, width, n, BUFFER_SIZE, output_bmp_filename, offset_byte_index, pad_per_row);
        printf(" DONE!\n");
    }

    int i;
    for(i = 0; i < height; i++)
        free(matrix[i]);
        
    free(matrix);

    return 0;
}


/**
 * Used to convert information found in the bmp header file 
 * to integer values.
*/
long calculate_value(uint8_t info[], uint8_t size, uint8_t max_limit)
{
    int base = max_limit + 1;
    long dim = 0;
    int i, j;
    for(i = 0; i < size; i++)
    {
        int current_value = (int)info[i];
        if (current_value != 0)
        {
            int power = 1;
            for (j = 0; j < i; j++)
                power *= base;

            dim += current_value * power;
        }
    }
    return dim;
}

void read_header(FILE* f, uint8_t output[], long start_pos, long end_pos)
{
    long len = (end_pos - start_pos) + 1;
    fseek(f, start_pos, SEEK_SET);
    fread(output, sizeof(uint8_t), len, f);
}

long count_bytes(FILE* f, int buffer_size)
{
    if (buffer_size <= 0)
        buffer_size = 1;

    fseek(f, 0, SEEK_SET);
    long counter = 0;
    uint8_t* bytes = (uint8_t*) malloc(buffer_size * sizeof(buffer_size));

    int result;

    do
    {
        result = fread(bytes, sizeof(uint8_t), buffer_size, f);
        counter += result;
    } while (result == buffer_size);

    free(bytes);

    return counter;
}

void read_pixels_into_matrix(FILE* f, uint8_t** matrix, int width, int height, long size, long pixel_offset, int pad_per_row)
{
    int padded_width = width + pad_per_row;
    fseek(f, pixel_offset, SEEK_SET);

    uint8_t* null_val = (uint8_t*) malloc(pad_per_row * sizeof(uint8_t));

    int i;
    for (i = height - 1; i >= 0; i--)
    {
        fread(matrix[i], sizeof(uint8_t), width, f);
        fread(null_val, sizeof(uint8_t), pad_per_row, f);
    }

    free(null_val);

}

void complement_matrix(uint8_t** matrix, int rows, int columns, uint8_t max_pixel_val)
{
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            matrix[i][j] = max_pixel_val - matrix[i][j];
}

void create_image(FILE* input, uint8_t** matrix, int height, int width, long size,
                   int buffer_size, const char* filename, long palette_offset, int pad_per_row)
{
    int padded_width = width + pad_per_row;
    FILE* output = fopen(filename, "wb");
    if (output == NULL)
    {
        printf("FATAL ERROR!");
        return;
    }

    uint8_t* buffer = (uint8_t*) malloc(buffer_size * sizeof(uint8_t));
    fseek(input, 0, SEEK_SET);

    int result;
    do
    {
        result = fread(buffer, sizeof(uint8_t), buffer_size, input);
        fwrite(buffer, sizeof(uint8_t), result, output);
    } while (result == buffer_size);

    free(buffer);

    uint8_t* null_val = (uint8_t*) malloc(pad_per_row * sizeof(uint8_t));
    int i;
    for (i = 0; i < pad_per_row; i++)
        null_val[i] = 0;


    fseek(output, palette_offset, SEEK_SET);
    for (i = height - 1; i >= 0; i--)
    {
        fwrite(matrix[i], sizeof(uint8_t), width, output);
        fwrite(null_val, sizeof(uint8_t), pad_per_row, output);
    }
    free(null_val);
    fclose(output);
}
