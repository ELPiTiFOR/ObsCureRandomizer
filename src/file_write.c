#include "file_write.h"

void write_at_offset(FILE *file, size_t offset, char *buf, size_t nb)
{
    fseek(file, offset, SEEK_SET);
    fwrite(buf, 1, nb, file);
}
