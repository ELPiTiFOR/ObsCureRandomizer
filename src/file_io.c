#include "file_io.h"

#include <direct.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "file_debug.h"
#include "logger.h"
#include "utils.h"

void copy_file(char *src, char *dst)
{
    FILE *src_file = fopen(src, "r+b");
    if (!src_file)
    {
        // TODO: format logs
        //log(ERROR, "Couldn't open src_file\n")
        fprintf(stderr, "ERROR: Couldn't open src_file: %s\n", src);
        return;
    }

    size_t slash_index = remove_file_from_path(dst);
    mkdir_p(dst);
    dst[slash_index] = '\\';
    FILE *dst_file = fopen(dst, "wb");
    if (!dst_file)
    {
        fprintf(stderr, "ERROR: Couldn't open dst_file: %s\n", dst);
        fclose(src_file);
        return;
    }

    fseek(src_file, 0, SEEK_SET);
    char buf[4096];
    arfillzeros(buf, 4096);

    size_t _read = fread(buf, 1, 4096, src_file);
    size_t total_read = _read;
    while (_read != 0)
    {
        /*
        printf("Read: %zu\nContent: ", _read);
        print_array(buf, _read);
        puts("");
        */
        size_t _written = fwrite(buf, 1, _read, dst_file);
        if (_written != _read)
        {
            fprintf(stderr, "WARN: Quantity of bytes read and written not equal\n");
        }
        _read = fread(buf, 1, 4096, src_file);
        total_read += _read;
    }

    //printf("LOG: Total read bytes: %zu\n", total_read);
    fclose(src_file);
    fclose(dst_file);

    char buf_log[2048];
    sprintf(buf_log, "Copied file %s to %s\n", src, dst);
    log(LOG_MINOR, buf_log);
}

size_t next_step(char **path)
{
    size_t i = 0;
    while (path[0][i] != 0 && path[0][i] != '\\')
    {
        i++;
    }

    if (path[0][i] == 0)
    {
        // *path keeps its value
        return 0;
    }

    i++;
    char *new_start = *path + i;
    i = 0;
    *path = new_start;
    while (new_start[i] != 0 && new_start[i] != '\\')
    {
        i++;
    }

    if (new_start[i] == 0)
    {
        return 0;
    }

    new_start[i] = 0;

    //printf("next_step: %s\n", new_start);

    return i;
}

// make as many directories as necessary to have the whole path
// (like linux command `mkdir -p`)
// we assume path is absolute and thus starts with 'X:\\'
void mkdir_p(char *path)
{
    //printf("mkdir -p: %s\n", path);
    size_t i = 0;
    char *p = path;
    char *p2 = p;

    while ((i = next_step(&p)) != 0 && p != p2)
    {
        if(_mkdir(path) != 0)
        {
            /*
            if (errno == ENOENT)
            {
                fprintf(stderr, "ERROR: _mkdir didn't work on %s, NOENT\n", p);
            }
            else if (errno == EEXIST)
            {
                fprintf(stderr, "ERROR: _mkdir didn't work on %s, EEXIST\n", p);
            }
            else
            {
                fprintf(stderr, "ERROR: _mkdir didn't work on %s, WTF\n", p);
            }
            */
        }
        p[i] = '\\';
        p2 = p;
    }

    if (i == 0)
    {
        if(_mkdir(path) != 0)
        {
            /*
            if (errno == ENOENT)
            {
                fprintf(stderr, "ERROR: _mkdir didn't work on %s, NOENT\n", p);
            }
            else if (errno == EEXIST)
            {
                fprintf(stderr, "ERROR: _mkdir didn't work on %s, EEXIST\n", p);
            }
            else
            {
                fprintf(stderr, "ERROR: _mkdir didn't work on %s, WTF\n", p);
            }
            */
        }
    }
}

char *str_from_file(char *src, size_t *total_written)
{
    // DON'T DELETE THE "b" IN THE MODE IN fopen() !!!!!!!
    FILE *file = fopen(src, "r+b");
    if (!file)
    {
        fprintf(stderr, "ERROR: Couldn't fopen %s\n", src);
        return NULL;
    }


    //print_hex_file(file);
    fseek(file, 0, SEEK_SET);

    char *res = calloc(128, 1);
    if (!res)
    {
        fprintf(stderr, "ERROR: Couldn't alloc res");
        fclose(file);
        return NULL;
    }



    size_t _read;
    size_t i = 0;
    while ((_read = fread(res + i, 1, 128 - 1, file)) != 0)
    {
        //printf("i = %zu | _read = %zu | i + _read = %zu\n", i, _read, i + _read);
        i += _read;
        if (_read == 127)
        {
            char *res2 = realloc(res, 128 + i + 1);
            if (!res2)
            {
                fprintf(stderr, "ERROR: Couldn't alloc res2");
                fclose(file);
                return NULL;
            }

            res = res2;
        }
    }

    char buf_log[4096];
    sprintf(buf_log, "Content of the file: <%s>\n", res);
    //log(LOG_MINOR, buf_log);

    /*
    printf("CONTENT OF THE FILE ONCE AGAIN:\n");
    print_array(res, i);
    puts("");
    */

    if (total_written)
    {
        //printf("i = %zu\n", i);
        *total_written = i;
        //printf("*total_written = %zu\n", *total_written);
    }
    res[i] = 0;
    fclose(file);
    return res;
}

// we assume argv ends with a NULL pointer
void free_argv(char **argv)
{
    size_t i = 0;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }

    free(argv);
}

char **str_to_argv(char *str, int *argc)
{
    char buf_log2[1024];
    sprintf(buf_log2, "str_to_argv from %s\n", str);
    log(LOG_VERY_MINOR, buf_log2);

    *argc = 0;
    char *delim = ";\r\n";
    char **res = calloc(1, sizeof(char *));
    if (!res)
    {
        fprintf(stderr, "ERROR: Couldn't alloc res**");
        return NULL;
    }

    res[0] = NULL;
    size_t i = 0;

    char *tok = strtok(str, delim);
    while (tok)
    {
        char buf_log[512];
        sprintf(buf_log, "Next token: %s\n", tok);
        log(LOG_VERY_MINOR, buf_log);

        size_t len = strlen(tok);
        char *tmp = calloc(len + 1, sizeof(char));
        if (tmp == NULL)
        {
            fprintf(stderr, "ERROR: Couldn't alloc res[i]\n");
            free_argv(res);
            return NULL;
        }

        char **res2 = realloc(res, sizeof(char *) * (i + 2));
        if (!res2)
        {
            fprintf(stderr, "ERROR: Couldn't alloc res2\n");
            free_argv(res);
            return NULL;
        }
        res = res2;
        res[i + 1] = NULL;
        strcpy(tmp, tok);
        res[i] = tmp;
        i++;
        tok = strtok(NULL, delim);
    }

    *argc = i;
    return res;
}

void initialize_tm_file(FILE *file)
{
    fseek(file, 4, SEEK_SET);
}

int get_buf_from_file(FILE *file, size_t from, size_t to, unsigned char *buf)
{
    fseek(file, from, SEEK_CUR);
    size_t read = fread(buf, 1, to - from + 1, file);
    fseek(file, -read - from, SEEK_CUR);
    return read != to - from + 1;
}

int file_from_string(char *dst, char *content, size_t len)
{
    FILE *file = fopen(dst, "wb");
    if (!file)
    {
        char buf_log[512];
        sprintf(buf_log, "Couldn't fopen() <%s>\n", dst);
        log(ERROR, buf_log);
        return 1;
    }

    //printf("len (from file_from_string) = %llX\n", len);
    size_t written = 0;

    size_t w;
    while ((w = fwrite(content + written, 1, len - written, file)))
    {
        written += w;
    }

    fclose(file);
    return 0;
}












