#include "allitems.h"

#include <stdlib.h>

size_t allitems_nb = 0;
enum item_id allitems[256];
enum item_id weapons[] = { BBAT, MBAR, ABAT, OPIS, APIS, FGUN, REVO, LGUN, SHOT, DBAR };
enum item_id ressource_items[] = { ENER, DISC, FAID, TAPE };

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
