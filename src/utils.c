#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "logger.h"


void arfillzeros(char *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        arr[i] = 0;
    }
}

void print_array(unsigned char *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%02X |", arr[i]);
    }
}

long buf_to_long(unsigned char *buf)
{
    long res = 0;
    for (size_t i = 0; i < 4; i++)
    {
        res = res << 8;
        res |= buf[i];
        char buf_log[512];
        sprintf(buf_log, "Buf_to_long iter: %zu | long: %x\n", i, res);
        //log(LOG_VERY_MINOR, buf_log);
    }

    return res;
}

uint64_t buf_to_long_long(unsigned char *buf)
{
    uint64_t res = 0;
    for (size_t i = 0; i < 8; i++)
    {
        res = res << 8;
        res |= buf[i];
        char buf_log[512];
        sprintf(buf_log, "Buf_to_long iter: %zu | buf[i]: %02X | long: %016llX\n", i, buf[i], res);
        //log(LOG_VERY_MINOR, buf_log);
    }

    return res;
}

void string_toupper(char *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }
}

void print_hex(unsigned char *buf, size_t n)
{
    printf("Printing...\n");
    size_t i = 0;
    while (i < n)
    {
        printf("%02X ", buf[i]);
        if (i != 0 && (i + 1) % 16 == 0)
        {
            printf("\n");
        }
        i++;
    }
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

// we assume the path has at least one directory before
// the filename (can't have 'somefilename')
size_t remove_file_from_path(char *path)
{
    size_t i = 0;
    while (path[i] != 0)
    {
        i++;
    }

    while(path[i] != '\\')
    {
        i--;
    }

    path[i] = 0;

    return i;
}

int lsb_from_long(long long l)
{
    long long *p = &l;
    void *p_v = p;
    int *lsb = p_v;
    return *lsb;
}

int msb_from_long(long long l)
{
    long long *p = &l;
    void *p_v = p;
    int *lsb = p_v;
    return *(lsb + 1);
}

// we assume buf is 4 bytes long
void itoa_hex(int i, char *buf)
{
    int *p = &i;
    void *p_v = p;
    char *p_c = p_v;

    for (size_t j = 0; j < 4; j++)
    {
        buf[j] = p_c[3 - j];
    }
}

// we assume buf is 8 bytes long
void lltoa_hex(long long i, char *buf)
{
    long long *p = &i;
    void *p_v = p;
    char *p_c = p_v;

    for (size_t j = 0; j < 8; j++)
    {
        buf[j] = p_c[7 - j];
    }
}

void print_argv(int argc, char **argv)
{
    for (size_t i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
}

// returns a random integer between 0 and 100
int get_random_percentage()
{
    return rand() % 101;
}

// returns a new heap allocated string equal in content to `str`
char *duplicate_string(char *str)
{
    size_t len = strlen(str);
    char *res = calloc(len + 1, 1);
    if (!res)
    {
        char buf_log[512];
        sprintf(buf_log, "Couldn't calloc() %s\n", str);
        log(ERROR, buf_log);
        return NULL;
    }

    strcpy(res, str);
    return res;
}

char *duplicate_string_until(char *str, int c, size_t *index)
{
    //printf("DUPLICATE_STRING_UNTIL: %s\n", str);
    char *mark = my_strchrnul(str, c);
    if (!mark)
    {
        char buf_log[512];
        sprintf(buf_log, "Character %c not found in %s\n", c, str);
        log(ERROR, buf_log);
        return NULL;
    }
    //printf("MARK: %s\n", mark);
    if (*mark == '\n' && *(mark - 1) == '\r')
    {
        *(mark - 1) = 0;
    }

    *mark = 0;
    char *key = duplicate_string(str);
    if (!key)
    {
        *mark = c;
        return NULL;
    }

    if (index)
    {
        *index += mark - str;
    }

    if (c == '\n')
    {
        *(mark - 1) = '\r';
    }
    *mark = c;
    return key;
}

// returns an array of strings consisting of two strings
// str must have this pattern: `[^=\n]*=[^=\n]*\n.*`
char **equal_from_line(char *str, size_t *index_ptr)
{
    char **couple = calloc(3, sizeof(char *));
    if (!couple)
    {
        log(ERROR, "Couldn't calloc() the couple\n");
        return NULL;
    }
    couple[2] = NULL;

    size_t index = 0;
    char *key = duplicate_string_until(str, '=', &index);
    if (!key)
    {
        free_argv(couple);
        return NULL;
    }
    //printf("new index: %c", str[index]);
    couple[0] = key;

    index++;
    //printf("value / str + %d: %c", index, str[index]);
    char *value = duplicate_string_until(str + index, '\n', &index);
    if (!value)
    {
        free_argv(couple);
        return NULL;
    }
    couple[1] = value;
    *index_ptr += index + 1;
    //printf("new index: %c", str[index]);
    //print_argv(2, couple);

    return couple;
}

void free_triple_char_pointer(char ***config)
{
    for (size_t i = 0; config[i]; i++)
    {
        free_argv(config[i]);
    }

    free(config);
}

char ***config_from_str(char *str)
{
    size_t nb_couples = 0;
    size_t index = 0;
    char ***config = calloc(1, sizeof(char **));
    if (!config)
    {
        log(ERROR, "Couldn't calloc() the config\n");
        return NULL;
    }
    config[0] = NULL;

    do
    {
        nb_couples += 1;
        char buf_log[512];
        sprintf(buf_log, "config_from_str nb_couples = %zu\n", nb_couples);
        log(LOG_VERY_MINOR, buf_log);

        char ***config2 = realloc(config, sizeof(char **) * (nb_couples + 1));
        if (!config2)
        {
            free_triple_char_pointer(config);
            log(ERROR, "Couldn't realloc\n");
            return NULL;
        }
        config = config2;
        config[nb_couples] = NULL;

        /*
        if (str[index] == '\n')
        {
            index++;
        }
        */

        char **couple = equal_from_line(str + index, &index);
        if (!couple)
        {
            char buf_log[512];
            sprintf(buf_log, "Couldn't equal_from_line(%s)\n", str);
            log(ERROR, buf_log);
            free_triple_char_pointer(config);
            return NULL;
        }

        //printf("str[index] = %s", str + index);
        config[nb_couples - 1] = couple;
    } while (str[index] && nb_couples < 10);

    return config;
}

char *my_strchrnul(char *str, int c)
{
   while(*str && *str != c)
   {
      str++;
   }

   return str;
}

void print_config(char ***config)
{
    for (size_t i = 0; config[i]; i++)
    {
        print_argv(2, config[i]);
    }
}

void eliminate_range_from_string(char *str, size_t offset, size_t len_section, size_t len_str)
{
    /*
    printf("The old string: len: %zu\n", len_section);
    print_hex(str + offset, 16);
    print_hex(str + offset + len_section, 16);
    puts("");
    */

    char *p = str + offset;
    char *p2 = str + offset + len_section;
    //printf("*p = %02X\n", *p);
    //printf("*p2 = %02X\n", *p2);

    size_t i = 0;
    size_t len = len_str - len_section - offset;
    //printf("len = %zu | len_str = %zu | len_section = %zu | offset = %zu |\n", len, len_str, len_section, offset);
    //printf("len = %llX | len_str = %llX | len_section = %llX | offset = %llX |\n", len, len_str, len_section, offset);
    while (i < len)
    {
        /*

        printf("bueno\n");

        printf("*p = %hhX\n", *p);
        printf("*p2 = %hhX\n", *p2);
        */
        *p = *p2;
        i++;
        p++;
        p2++;
    }

    str[offset + i] = 0;
    /*
    printf("The new string:\n");
    print_hex(str + offset, 16);
    puts("");
    */
}













