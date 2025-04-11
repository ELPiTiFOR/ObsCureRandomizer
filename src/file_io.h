#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stddef.h>

void print_hex(unsigned char *buf, size_t n);
void print_hex_file(FILE *file);
void write_at_offset(FILE *file, size_t offset, char *buf, size_t nb);
int compare_arrays(unsigned char *a1, unsigned char *a2, size_t len);
int check_pattern(FILE *file, unsigned char *pat, size_t pat_len);
ssize_t search_pattern(FILE* file, unsigned char *pat, size_t pat_len);

#endif // FILE_IO_H
