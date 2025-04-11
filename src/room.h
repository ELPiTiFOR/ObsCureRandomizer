#ifndef ROOM_H
#define ROOM_H

#include <stddef.h>

#include "allitems.h"

#define ROOMS_NB 2

struct room
{
    char name[30];
    char id[8];
    long long items[10];
};

enum room_id
{
    B008 = 0,
    E103,
    NOROOM
};

void restore_room(struct room* room);
void replace_item(enum room_id room, enum item_loc loc, enum item_id new_item);
void randomize_item(enum room_id room, enum item_loc loc, enum item_id *group, size_t group_len);
void randomize_room(enum room_id room);
void set_path_room(struct room *my_room);

extern struct room cafeteria_r;
extern struct room *rooms[];
extern enum room_id rooms_ids[];

#endif // ROOM_H
