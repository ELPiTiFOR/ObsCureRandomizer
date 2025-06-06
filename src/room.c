#include "room.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allitems.h"
#include "file_io.h"
#include "file_read.h"
#include "file_write.h"
#include "logger.h"
#include "path.h"
#include "room_debug.h"
#include "room_vars.h"
#include "tm_file.h"
#include "utils.h"

int restore_room2(enum room_id room)
{
    char buf_log[512];
    sprintf(buf_log, "Restoring room %d, %s\n", room, rooms[room]);
    log(LOG_MINOR, buf_log);
    if (!is_path_backups_set())
    {
        log(ERROR, "You must specify a backup path before restoring a room\n");
        return 1;
    }
    if (!is_path_backups_room_set())
    {
        log(ERROR, "You must specify a backup name before restoring a room\n");
        return 1;
    }

    struct room *my_room = rooms[room];
    set_path_room(my_room);
    copy_file(path_backups_room, path_room);

    // now we need to modify allitems.it
    for (size_t i = 0; my_room->items[i] && i < 15; i++)
    {
        replace_item_allitems(lsb_from_long(my_room->items[i]), msb_from_long(my_room->items[i]));
    }

    return 0;
}

void replace_item(FILE *room_file, FILE *items_file, enum item_loc loc, enum item_id new_item)
{
    char buf_log[64];
    sprintf(buf_log, "Replacing Item: %06lX. With: %04lX.\n", loc, new_item);
    log(LOG_MINOR, buf_log);

    //print_all_paths();

    char buf4_loc[4];
    char buf4_item[4];

    itoa_hex(loc, buf4_loc);
    itoa_hex(new_item, buf4_item);

    // room file
    ssize_t offset_room = search_pattern(room_file, buf4_loc, 4);
    /*if (offset_room == -1)
    {
        char buf_log[64];
        sprintf(buf_log, "Pattern %06lX not found in room_file.\n", loc);
        log(ERROR, buf_log);
        return;
    }*/
    write_at_offset(room_file, offset_room - 4, buf4_item, 4);

    // items file
    ssize_t offset_items = search_pattern(items_file, buf4_loc, 4);
    write_at_offset(items_file, offset_items - 4, buf4_item, 4);
}


void delete_item(uint64_t id_loc)
{
    size_t content_len = 0;
    char *room_file_content = str_from_file(path_room, &content_len);

    FILE *room_file = fopen(path_room, "rb");
    if (!room_file)
    {
        char buf_log[1024];
        sprintf(buf_log, "Couldn't fopen() path_room <%s>\n", path_room);
        log(ERROR, buf_log);
        return;
    }

    size_t item_offset = get_offset_of_item_tm_file(room_file, id_loc);
    if(!item_offset)
    {
        char buf_log[1024];
        sprintf(buf_log, "Couldn't find item %016llX\n", id_loc);
        log(ERROR, buf_log);
        return;
    }

    size_t section_len = get_len_of_section_tm_file(room_file);

    eliminate_range_from_string(room_file_content, item_offset, section_len + 4, content_len);
    file_from_string(path_room, room_file_content, content_len - section_len - 4);
    free(room_file_content);

    fclose(room_file);
}

void randomize_item2(FILE *room_file, FILE *items_file, uint64_t id_loc, enum items_group e_group)
{
    // TODO: format log
    log(LOG_VERY_MINOR, "Randomizing item\n");

    char buf4_loc[4];
    char buf4_item[4];

    enum item_id new_item;

    if (e_group == ALLITEMS)
    {
        new_item = get_random_item_with_prob();
    }
    else
    {
        new_item = get_random_item(items_groups[e_group], items_groups_lens[e_group]);
    }



    char buf_log[64];
    sprintf(buf_log, "New Item: %04lX.\n", new_item);
    log(LOG_MINOR, buf_log);

    if (new_item == NONE)
    {
        // we need to eliminate the item from the room file _n.tm
        fclose(room_file);

        char buf_log[512];
        sprintf(buf_log, "Eliminating item %016llX\n", id_loc);
        log(LOG_MINOR, buf_log);

        delete_item(id_loc);

        room_file = fopen(path_room, "r+b");
        if (!room_file)
        {
            char buf_log[512];
            sprintf(buf_log, "Couldn't reopen room_file <%s>", path_room);
            log(ERROR, buf_log);
        }

        return;
    }

    int loc = lsb_from_long(id_loc);

    replace_item(room_file, items_file, loc, new_item);
}

void randomize_room2(struct room *room)
{

    char buf_log[512];
    if (check_items_in_room(room))
    {
        // we restore the room before randomizing it
        char buf_log2[512];
        sprintf(buf_log2, "Room %s items check failed, restoring now.\n", room->id);
        log(WARN_LOW, buf_log2);
        restore_room2(get_e_room_from_id(room->id));
    }

    sprintf(buf_log, "Randomizing room %s\n", room->id);
    log(LOG_APP_CMD, buf_log);

    FILE *room_file = fopen(path_room, "r+b");
    if (!room_file)
    {
        char buf_log[1024];
        sprintf(buf_log, "Couldn't load FILE room_file: %s\n", path_room);
        log(ERROR, buf_log);
        return;
    }

    FILE *items_file = fopen(path_items, "r+b");
    if (!items_file)
    {
        char buf_log[1024];
        sprintf(buf_log, "Couldn't load FILE items_file: %s\n", path_items);
        log(ERROR, buf_log);
    }

    uint64_t item = 0;
    for (size_t i = 0; (item = room->items[i]); i++)
    {
        if (strcmp(room->id, "b008") == 0 && lsb_from_long(room->items[i]) == 0x020402)
        {
            randomize_item2(room_file, items_file, 0x020402, DAMAGE_WEAPONS);
            continue;
        }
        if (strcmp(room->id, "a004") == 0 && lsb_from_long(room->items[i]) == 0x010203)
        {
            continue;
        }
        if (strcmp(room->id, "a004") == 0 && lsb_from_long(room->items[i]) == 0x010201)
        {
            continue;
        }
        if (strcmp(room->id, "a004") == 0 && lsb_from_long(room->items[i]) == 0x010202)
        {
            continue;
        }

        randomize_item2(room_file, items_file, item, ALLITEMS);
    }
}

enum room_id get_e_room_from_id(char *id)
{
    size_t i = 0;
    while (i < ROOMS_IDS_STR_NB)
    {
        if (strcmp(rooms[i]->id, id) == 0)
        {
            return rooms_ids[i];
        }
        i++;
    }

    return NOROOM;
}

// this function expects path_room to be initialized and to have the id
// of a valid room
// it returns the parsed room in a new heap allocated struct room
struct room *parse_room()
{
    char buf_id[5];
    get_id_from_path_room(buf_id);
    struct room *new_room = malloc(sizeof(struct room));
    if (!new_room)
    {
        char buf[512];
        arfillzeros(buf, 512);
        sprintf(buf, "Couldn't malloc() new_room %s\n", buf_id);
        log(ERROR, buf);
        return NULL;
    }
    strncpy(new_room->id, buf_id, 5);
    strncpy(new_room->name, buf_id, 5);

    FILE *room_file = fopen(path_room, "rb");
    if (!room_file)
    {
        char buf[1024];
        arfillzeros(buf, 1024);
        sprintf(buf, "Couldn't fopen() room_file %s\n", path_room);
        log(ERROR, buf);
        free(new_room);
        return NULL;
    }

    // now wee need to fill the items array
    int eof = 0;
    initialize_tm_file(room_file);
    eof = goto_items_tm_file(room_file);
    size_t item_index = 0;

    for (size_t i = 0; i < 15; i++)
    {
        new_room->items[i] = 0;
    }

    do
    {
        uint64_t id_loc = get_item_id_and_location_tm_file(room_file);
        if (!id_loc)
        {
            break;
        }
        int lsb = msb_from_long(id_loc);
        size_t j = 0;
        while (j < allitems_nb)
        {
            if (lsb == allitems[j])
            {
                char buf_log[512];
                sprintf(buf_log, "Id & Loc: %016llX\n", id_loc);
                log(LOG_MINOR, buf_log);
                new_room->items[item_index] = id_loc;
                item_index++;
            }
            j++;
        }

        eof = goto_next_section_tm_file(room_file);
    } while (eof == 0);

    return new_room;
}








