#include "file_read.h"

#include "utils.h"

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
