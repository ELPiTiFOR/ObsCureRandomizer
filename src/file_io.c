#include "file_io.h"

#include "utils.h"

void print_hex(unsigned char *buf, size_t n)
{
    size_t i = 0;
    while (i < n)
    {
        printf("%2X ", buf[i]);
        if (i != 0 && (i + 1) % 16 == 0)
        {
            printf("s\n");
        }
        i++;
    }
}

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

void write_at_offset(FILE *file, size_t offset, char *buf, size_t nb)
{
    fseek(file, offset, SEEK_SET);
    fwrite(buf, 1, nb, file);
}

// We assume a1 and a2 are at least len bytes long
int compare_arrays(unsigned char *a1, unsigned char *a2, size_t len)
{
    size_t i = 0;
    while (i < len && a1[i] == a2[i])
    {
        i++;
    }

    return i == len;
}

// We assume pat_len < 64
int check_pattern(FILE *file, unsigned char *pat, size_t pat_len)
{
    unsigned char buf[64];
    size_t _read = fread(buf, 1, pat_len, file);
    fseek(file, -_read + 1, SEEK_CUR);
    return compare_arrays(buf, pat, pat_len);
}

ssize_t search_pattern(FILE* file, unsigned char *pat, size_t pat_len)
{
    //print_array(pat, pat_len);
    unsigned char first = pat[0];
    fseek(file, 0, SEEK_SET);
    size_t _read;
    unsigned char buf[1];
    while ((_read = fread(buf, 1, 1, file)) > 0)
    {
        if (buf[0] == first)
        {
            fseek(file, -1, SEEK_CUR);
            size_t offset = ftell(file);
            if (check_pattern(file, pat, pat_len))
            {
                // success
                return offset;
            }
        }
    }

    return -1;
}
