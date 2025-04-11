#include "utils.h"

#include <stdio.h>

void arfillzeros(char *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        arr[i] = 0;
    }
}

char path_items[512];
char path_room[512];
char path_game[512];

void print_array(char *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%X |", arr[i]);
    }
}
