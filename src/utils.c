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
    char *mark = my_strchrnul(str, c);
    if (!mark)
    {
        char buf_log[512];
        sprintf(buf_log, "Character %c not found in %s\n", c, str);
        log(ERROR, buf_log);
        return NULL;
    }
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
    couple[0] = key;

    index++;
    char *value = duplicate_string_until(str + index, '\n', &index);
    if (!value)
    {
        free_argv(couple);
        return NULL;
    }
    couple[1] = value;
    *index_ptr += index + 1;

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

        char ***config2 = realloc(config, sizeof(char **) * (nb_couples + 1));
        if (!config2)
        {
            free_triple_char_pointer(config);
            log(ERROR, "Couldn't realloc\n");
            return NULL;
        }
        config = config2;
        config[nb_couples] = NULL;

        char **couple = equal_from_line(str + index, &index);
        if (!couple)
        {
            char buf_log[512];
            sprintf(buf_log, "Couldn't equal_from_line(%s)\n", str);
            log(ERROR, buf_log);
            free_triple_char_pointer(config);
            return NULL;
        }

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

    char *p = str + offset;
    char *p2 = str + offset + len_section;

    size_t i = 0;
    size_t len = len_str - len_section - offset;
    while (i < len)
    {
        *p = *p2;
        i++;
        p++;
        p2++;
    }

    str[offset + i] = 0;
}

int get_value_from_char_base(char c, int base)
{
    char buf_log[64];
    sprintf(buf_log, "Parsing digit <%c> in base %d\n", c, base);
    log(LOG_VERY_MINOR, buf_log);

    int res = 0;

    if (c >= '0' && c <= '9')
    {
        res = c - '0';
    }
    else if (c >= 'a' && c <= 'z')
    {
        res = c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        res = c - 'A' + 10;
    }
    else
    {
        return -1;
    }

    if (res >= base)
    {
        char buf_log[64];
        sprintf(buf_log, "%c is an nvalid digit in %d base\n", c, base);
        log(ERROR, buf_log);
        return -1;
    }

    sprintf(buf_log, "Result: %lX\n", res);
    log(LOG_VERY_MINOR, buf_log);
    return res;
}

// ONLY POSITIVE NUMBERS, NO SPACES
uint32_t my_atoi_base(char *str, int base)
{
    char buf_log[64];
    sprintf(buf_log, "Parsing <%s> in base %d\n", str, base);
    log(LOG_MINOR, buf_log);
    uint32_t res = 0;
    size_t i = 0;
    char c = 0;
    while ((c = str[i]))
    {
        res *= base;
        int value = get_value_from_char_base(c, base);
        if (value < 0)
        {
            log(ERROR, "The given string is not a valid\n");
            return 0;
        }

        res += value;
        i++;
    }

    sprintf(buf_log, "Result: %lX\n", res);
    log(LOG_VERY_MINOR, buf_log);

    return res;
}

// returns 0 if str represents False, 1 if it represents True, -1 otherwise
int bool_str(char *str)
{
    string_toupper(str);
    if (strcmp(str, "TRUE") == 0)
    {
        return 1;
    }
    else if (strcmp(str, "1") == 0)
    {
        return 1;
    }
    if (strcmp(str, "FALSE") == 0)
    {
        return 0;
    }
    else if (strcmp(str, "0") == 0)
    {
        return 0;
    }

    return -1;
}









