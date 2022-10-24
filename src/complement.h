#ifndef COMPLEMENT_H_INCLUDED
#define COMPLEMENT_H_INCLUDED

#include <stdint.h>

#define BUFFER_SIZE 65536
int complement_and_write_image(const char*, const char*, const char*, int, int);
long calculate_value(uint8_t[], uint8_t, uint8_t);
void read_header(FILE*, uint8_t[], long, long);
long count_bytes(FILE*, int);
void read_pixels_into_matrix(FILE*, uint8_t**, int, int, long, long, int);
void complement_matrix(uint8_t**, int, int, uint8_t);
void print_matrix(uint8_t**, int, int);
void create_image(FILE*, uint8_t**, int, int, long, int, const char*, long, int);

#endif // COMPLEMENT_H_INCLUDED
