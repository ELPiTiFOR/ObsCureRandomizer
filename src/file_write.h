#ifndef FILE_WRITE_H
#define FILE_WRITE_H

#include <stddef.h>
#include <stdio.h>

void write_at_offset(FILE *file, size_t offset, char *buf, size_t nb);

#endif // FILE_WRITE_H
