#include "room_debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "path.h"
#include "room_vars.h"
#include "utils.h"

void print_room(struct room *room, int nb)
{
    printf("struct room room%d = { \"%s\", \"%s\", { ", nb, room->name, room->id);
    if (room->items[0])
    {
        size_t i = 0;
        printf("0x%016llX", room->items[i]);
        i++;
        while (i < 15 && room->items[i])
        {
            printf(", 0x%016llX", room->items[i]);
            i++;
        }
    }

    printf(" } };\n");
}

void print_every_room()
{
    for (size_t i = 0; i < ROOMS_IDS_STR_NB; i++)
    {
        char *id = rooms_ids_str[i];
        set_path_room_str(id);
        struct room *room = parse_room();
        if (!room)
        {
            char buf_log[512];
            sprintf(buf_log, "Couldn't alloc room with id %s", id);
            log(ERROR, buf_log);
            return;
        }
        print_room(room, i);
        free(room);
    }
}

void print_struct_rooms_ids()
{
    printf("struct room *rooms_generated[] =\n{\n");
    for (size_t i = 0; i < ROOMS_IDS_STR_NB; i++)
    {
        printf("\t&room%d\n", i);
    }
    printf("};");
}

void print_enum_rooms_ids()
{
    printf("enum room_id\n{\n");
    char id[5];
    strncpy(id, rooms_ids_str[0], 5);
    string_toupper(id);
    printf("\t%s = 0,\n", id);
    for (size_t i = 1; i < ROOMS_IDS_STR_NB; i++)
    {
        strncpy(id, rooms_ids_str[i], 5);
        string_toupper(id);
        printf("\t%s,\n", id);
    }
    printf("};");
}

void print_enum_rooms_array()
{
    //enum room_id rooms_ids[] = { B000, B008, E103 };
    printf("enum room_id rooms_ids[] =\n{\n");
    char id[5];
    for (size_t i = 1; i < ROOMS_IDS_STR_NB; i++)
    {
        strncpy(id, rooms_ids_str[i], 5);
        string_toupper(id);
        printf("\t%s,\n", id);
    }
    printf("};");
}
