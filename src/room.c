#include "room.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "file_io.h"
#include "allitems.h"
#include "utils.h"

struct room cafeteria_r = {"Cafeteria", "b008", { 0x0000025A00020401, 0x000000CA00020402, 0x0000026400020403} };
struct room infirmary_r = {"Infirmary", "e103", { 0x0000025A00053383, 0x0000025F00053386, 0x0000025F00053385, 0x0000025F00053384 } };
struct room *rooms[] = { &cafeteria_r, &infirmary_r };
enum room_id rooms_ids[] = { B008, E103 };

// we assume path_room has at least the content of path_game, maybe more
void set_path_room(struct room *my_room)
{
    size_t path_game_len = strlen(path_game);
    path_room[path_game_len] = 0;
    strcat(path_room, "\\data\\_levels\\b\\b008\\b008_n.tm");
    path_room[path_game_len + 14] = my_room->id[0];
    strncpy(path_room + path_game_len + 16, my_room->id, 4);
    strncpy(path_room + path_game_len + 21, my_room->id, 4);
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

void restore_room(struct room* room)
{
    size_t i = 0;
    long long item;

    //FILE *room_file = fopen("C:\\SteamLibrary\\steamapps\\common\\Obscure\\data\\_levels\\b\\b008\\b008_n.tm", "r+b");
    FILE *room_file = fopen(path_room, "r+b");
    if (room_file == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't open room_file: %s\n", path_room);
        return;
    }

    //FILE *items_file = fopen("C:\\SteamLibrary\\steamapps\\common\\Obscure\\data\\_common\\allitems.it", "r+b");
    FILE *items_file = fopen(path_items, "r+b");
    if (items_file == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't open items_file\n");
        fclose(room_file);
        return;
    }

    while ((item = room->items[i]) != 0)
    {
        // we take the last 4 bytes of item
        int lsb = lsb_from_long(item);

        // we take the first 4 bytes of item
        int msb = msb_from_long(item);

        char buf4[4];
        char buf8[8];

        itoa_hex(lsb, buf4);
        lltoa_hex(item, buf8);

        ssize_t offset_room = search_pattern(room_file, buf4, 4);
        write_at_offset(room_file, offset_room - 4, buf8, 8);

        // items file
        ssize_t offset_items = search_pattern(items_file, buf4, 4);
        write_at_offset(items_file, offset_items - 4, buf8, 8);
        i++;
    }
    fclose(room_file);
    fclose(items_file);
}

void replace_item(enum room_id room, enum item_loc loc, enum item_id new_item)
{
    struct room *my_room = rooms[room];

    FILE *room_file = fopen(path_room, "r+b");
    if (room_file == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't open room_file %s\n", path_room);
        return;
    }

    FILE *items_file = fopen(path_items, "r+b");
    if (items_file == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't open items_file: %s\n", path_items);
        fclose(room_file);
        return;
    }

    char buf4_loc[4];
    char buf4_item[4];

    itoa_hex(loc, buf4_loc);
    itoa_hex(new_item, buf4_item);

    // room file
    ssize_t offset_room = search_pattern(room_file, buf4_loc, 4);
    write_at_offset(room_file, offset_room - 4, buf4_item, 4);

    // items file
    ssize_t offset_items = search_pattern(items_file, buf4_loc, 4);
    write_at_offset(items_file, offset_items - 4, buf4_item, 4);

    fclose(room_file);
    fclose(items_file);
}

void randomize_item(enum room_id room, enum item_loc loc, enum item_id *group, size_t group_len)
{
    struct room *my_room = rooms[room];

    FILE *room_file = fopen(path_room, "r+b");
    if (room_file == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't open room_file %s\n", path_room);
        return;
    }

    FILE *items_file = fopen(path_items, "r+b");
    if (items_file == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't open items_file: %s\n", path_items);
        fclose(room_file);
        return;
    }

    char buf4_loc[4];
    char buf4_item[4];

    enum item_id new_item = get_random_item(group, group_len);

    itoa_hex(loc, buf4_loc);
    itoa_hex(new_item, buf4_item);

    // room file
    ssize_t offset_room = search_pattern(room_file, buf4_loc, 4);
    write_at_offset(room_file, offset_room - 4, buf4_item, 4);

    // items file
    ssize_t offset_items = search_pattern(items_file, buf4_loc, 4);
    write_at_offset(items_file, offset_items - 4, buf4_item, 4);

    fclose(room_file);
    fclose(items_file);
}

void randomize_room(enum room_id room)
{
    switch(room)
    {
    case B008:
        printf("LOG: Randomizing room B008\n");
        randomize_item(room, CAFE_BBAT, weapons, WEAPONS_NB);
        randomize_item(room, CAFE_DISC, allitems, allitems_nb);
        randomize_item(room, CAFE_ENER, allitems, allitems_nb);
        break;
    case E103:
        printf("LOG: Randomizing room E103\n");
        randomize_item(room, INFI_DISC, allitems, allitems_nb);
        randomize_item(room, INFI_FAID1, allitems, allitems_nb);
        randomize_item(room, INFI_FAID2, allitems, allitems_nb);
        randomize_item(room, INFI_FAID3, allitems, allitems_nb);
        break;
    default:
        printf("Unknown room");
        break;
    }
}
