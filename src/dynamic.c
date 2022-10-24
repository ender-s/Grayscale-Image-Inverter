#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dynamic.h"

uint8_t** allocate_memory_dynamically(int width, int height)
{

    uint8_t** matrix = (uint8_t **) calloc(height, sizeof(uint8_t*));

    if (matrix != NULL) // if the first allocation is successful
    {
        int i;
        for (i = 0; i < height; i++)
        {
            matrix[i] = (uint8_t*) calloc(width, sizeof(uint8_t));
            if (matrix[i] == NULL) // fail to allocate, abort process
            {
                matrix = NULL;
                break;
            }
        }
    }

    return matrix;
}


