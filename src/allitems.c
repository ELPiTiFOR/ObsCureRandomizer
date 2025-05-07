#include "allitems.h"

#include <stdlib.h>

#include "config.h"
#include "file_io.h"
#include "file_read.h"
#include "file_write.h"
#include "logger.h"
#include "path.h"
#include "utils.h"

size_t allitems_nb = 0;
enum item_id allitems[256];
enum item_id weapons[] = { BBAT, MBAR, ABAT, OPIS, APIS, FGUN, REVO, LGUN, SHOT, DBAR };
enum item_id guns[] = { OPIS, APIS, FGUN, REVO, LGUN, SHOT, DBAR };
enum item_id ressource_items[] = { ENER, DISC, FAID, TAPE };

enum item_id get_random_item_with_prob()
{
    if (get_random_percentage() >= general_item_spawn_prob[difficulty_mode])
    {
        return NONE;
    }

    if (get_random_percentage() < weapon_spawn_prob[difficulty_mode])
    {
        return weapons[rand() % WEAPONS_NB];
    }

    return ressource_items[rand() % RESSOURCES_NB];
}

enum item_id get_random_item(enum item_id *group, size_t group_len)
{
    return group[rand() % group_len];
}

void groupscat()
{
    for (size_t i = 0; i < 256; i++)
    {
        allitems[i] = NONE;
    }

    size_t i = 0;

    for (size_t j = 0; j < WEAPONS_NB; j++)
    {
        allitems[i] = weapons[j];
        i++;
    }

    for (size_t j = 0; j < RESSOURCES_NB; j++)
    {
        allitems[i] = ressource_items[j];
        i++;
    }

    allitems_nb = WEAPONS_NB + RESSOURCES_NB;
}

int restore_allitems()
{
    if (!is_path_backups_set())
    {
        log(ERROR, "You must specify a backup path before restoring allitems.it\n");
        return 1;
    }
    if (!is_path_backups_items_set())
    {
        log(ERROR, "You must specify a backup name before restoring allitems.it\n");
        return 1;
    }

    copy_file(path_backups_items, path_items);

    return 0;
}

// in allitems.it, replace the item with location `loc`
// with the id `id`
// before: 0x???????? $(loc)
// after:  $(id) $(loc)
// TODO: put FILE* as argument and set it in caller function?
void replace_item_allitems(uint32_t loc, uint32_t id)
{
    FILE *items_file = fopen(path_items, "r+b");
    if (items_file == NULL)
    {
        char buf_log[1024];
        sprintf(buf_log, "Couldn't open items_file: %s\n", path_items);
        log(ERROR, buf_log);
        return;
    }

    char buf4_loc[4];
    char buf4_item[4];

    itoa_hex(loc, buf4_loc);
    itoa_hex(id, buf4_item);

    // items file
    ssize_t offset_items = search_pattern(items_file, buf4_loc, 4);
    write_at_offset(items_file, offset_items - 4, buf4_item, 4);

    fclose(items_file);
}

