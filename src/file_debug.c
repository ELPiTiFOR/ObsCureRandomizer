#include "file_debug.h"

#include "utils.h"

void print_hex_file(FILE *file)
{
    fseek(file, 0, SEEK_SET);
    unsigned char buf[1024];
    for (size_t i = 0; i < 1024; i++)
    {
        buf[i] = 'c';
    }

    size_t _read = fread(buf, 1, 1024, file);

    while (_read > 0)
    {
        //printf("(%zu)", _read);
        print_hex(buf, _read);
        _read = fread(buf, 1, 1024, file);
    }
    printf("\n");
}
