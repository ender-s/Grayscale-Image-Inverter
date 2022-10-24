#ifndef ARRAYTOIMAGE_H_INCLUDED
#define ARRAYTOIMAGE_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

int array_to_image(uint8_t*, int, int, const char*);
void write_pixels(uint8_t**, int, int, FILE*);


#endif // ARRAYTOIMAGE_H_INCLUDED
