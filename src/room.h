#ifndef ROOM_H
#define ROOM_H

#include <stddef.h>

#include "allitems.h"

#define ROOMS_NB 3
#define ROOMS_IDS_STR_NB 100

struct room
{
    char name[30];
    char id[8];
    long long items[15];
};

/*
enum room_id
{
    B000 = 0,
    B008,
    E103,
    NOROOM
};
*/

enum room_id
{
        A000 = 0,
        A001,
        A002,
        A003,
        A004,
        B000,
        B002,
        B004,
        B005,
        B006,
        B007,
        B008,
        B009,
        B010,
        B100,
        B102,
        B103,
        B104,
        B106,
        C000,
        C003,
        C004,
        C006,
        C009,
        C010,
        C011,
        C100,
        C101,
        C104,
        C105,
        C109,
        D000,
        D001,
        D002,
        D003,
        D004,
        D005,
        D006,
        D009,
        D010,
        D100,
        D101,
        D102,
        D103,
        D104,
        D105,
        D106,
        E000,
        E001,
        E002,
        E003,
        E100,
        E101,
        E102,
        E103,
        F000,
        F001,
        F002,
        F003,
        F101,
        F102,
        G000,
        G001,
        G002,
        G003,
        G004,
        G005,
        G006,
        G007,
        G008,
        G009,
        G010,
        G012,
        G013,
        G014,
        G015,
        G016,
        G100,
        G103,
        G104,
        G105,
        G106,
        G107,
        I000,
        I001,
        I002,
        I004,
        I007,
        I100,
        I101,
        I102,
        I103,
        I107,
        J000,
        J001,
        M000,
        M001,
        M002,
        M003,
        M100,
        NOROOM
};

void restore_room(struct room* room);
int restore_room2(enum room_id room);
void replace_item(enum room_id room, enum item_loc loc, enum item_id new_item);
void randomize_item(enum room_id room, enum item_loc loc, enum item_id *group, size_t group_len);
void randomize_room(enum room_id room);
void randomize_room2(struct room *room);
enum room_id get_e_room_from_id(char *id);
struct room *parse_room();


#endif // ROOM_H
