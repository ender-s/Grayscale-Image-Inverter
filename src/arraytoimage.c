#include <stdio.h>
#include <stdint.h>

#include "arraytoimage.h"

int array_to_image(uint8_t* arr, int width, int height, const char* filename)
{

    FILE* f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("ERROR!: Could not open %s in write mode!\n", filename);
        return 0;
    }

    const char* magic_number = "P2";
    uint8_t max_val = 255;
    fprintf(f, "%s\n", magic_number);
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "%u\n", max_val);

    uint8_t** p = (uint8_t**) arr;
    write_pixels(p, width, height, f);

    fclose(f);
    return 0;
}

void write_pixels(uint8_t** arr, int width, int height, FILE* f)
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
            fprintf(f, "%u ", arr[i][j]);
        fprintf(f, "\n");
    }

}
