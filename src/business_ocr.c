#include "business_ocr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "choose_items.h"
#include "hoe_file.h"
#include "item.h"
#include "it_file.h"
#include "logger.h"
#include "ObsCureFileParserHeader/business.h"
#include "ObsCureInfoHeader/obscure_paths.h"
#include "paths.h"
#include "random.h"
#include "room.h"
#include "tm_file.h"

char msg[1024] = {0};

int randomize_room(char *room_id_string)
{
    // room id
    room_id ri = ri_from_ristr(room_id_string);

    // Parsing the `_n.tm` file
    uint8_t *path_tm = get_level_file_path(game_path, ri, "_n.tm");
    if (!path_tm)
    {
        log_msg(ERRO, "Couldn't get path_tm");
        return BUSINESS_OCR_FAIL;
    }
    tm_file *tm = parse_tm_file(path_tm);
    if (!tm)
    {
        char msg[512] = {0};
        sprintf(msg, "Couldn't parse %s", path_tm);
        log_msg(ERRO, msg);
        return BUSINESS_OCR_FAIL;
    }

    // Parsing the `.hoe` file
    uint8_t *path_hoe = get_level_file_path(game_path, ri, ".hoe");
    if (!path_hoe)
    {
        log_msg(ERRO, "Couldn't get path_hoe");
        return BUSINESS_OCR_FAIL;
    }
    hoe_file *hoe = parse_hoe_file(path_hoe);
    if (!hoe)
    {
        sprintf(msg, "Couldn't parse %s", path_hoe);
        log_msg(ERRO, msg);
        return BUSINESS_OCR_FAIL;
    }

    // Parsing the `.it` file
    uint8_t *path_it = get_allitems_path(game_path);
    if (!path_it)
    {
        log_msg(ERRO, "Couldn't get path_it");
        return BUSINESS_OCR_FAIL;
    }
    it_file *it = parse_it_file(path_it);
    if (!it)
    {
        sprintf(msg, "Couldn't parse %s", path_it);
        log_msg(ERRO, msg);
        return BUSINESS_OCR_FAIL;
    }

    // Iterating through every item
    for (size_t i = 0; i < tm->len_item_sections; i++)
    {
        // we get the UID and we check if it is forbidden
        uint32_t uid = tm->items[i].item_location;
        if (is_item_uid_forbidden(uid))
        {
            continue;
        }

        // we check the item_type
        if (is_item_forbidden(ii_from_it(tm->items[i].item_id)))
        {
            continue;
        }

        // we get the new item type
        item_type new_item_type = get_random_item_type(allowed_items_to_appear,
            LEN_ALLOWED_ITEMS_TO_APPEAR);

        // we modify the `tm` file
        tm->items[i].item_id = (uint32_t)new_item_type;

        // we modify the `it` file
        it_item iti = {0};
        get_item_from_loc(it, uid, &iti);
        iti.item_id = new_item_type;
        edit_item_in_it(it, &iti);

        // we modify the `hoe` file
        replace_item_hoe(hoe, uid, new_item_type);
    }

    // Reserializing
    serialize_tm_file(tm, path_tm);
    serialize_hoe_file(hoe, path_hoe);
    serialize_it_file(it, path_it);

    // Freeing everything;
    free_it_file(it);
    free_hoe_file(hoe);
    free_tm_file(tm);

    free(path_hoe);
    free(path_it);
    free(path_tm);

    return BUSINESS_OCR_SUCCESS;
}

int modify_item(uint32_t uid, item_index ii)
{
    // we get the new item type
    item_type new_item_type = it_from_ii(ii);

    uint8_t *path_it = get_allitems_path(game_path);
    it_file *it = parse_it_file(path_it);

    for (size_t i = 0; i < it->len_items; i++)
    {
        if (it->items[i]->item_loc == uid)
        {
            it->items[i]->item_id = new_item_type;
        }
    }

    room_id ri = find_item(game_path, uid);

    uint8_t *path_tm = get_level_file_path(game_path, ri, "_n.tm");
    size_t len_path_tm = strlen(path_tm);
    tm_file *tm = parse_tm_file(path_tm);
    if (!tm)
    {
        log_msg(ERRO, "Couldn't parse _n.tm");
        return 1;
    }

    for (size_t i = 0; i < tm->len_item_sections; i++)
    {
        if (tm->items[i].item_location == uid)
        {
            tm->items[i].item_id = new_item_type;
        }
    }

    serialize_tm_file(tm, path_tm);
    free_tm_file(tm);

    path_tm[len_path_tm - 4] = 'd';
    tm = parse_tm_file(path_tm);
    if (tm)
    {
        for (size_t i = 0; i < tm->len_item_sections; i++)
        {
            if (tm->items[i].item_location == uid)
            {
                tm->items[i].item_id = new_item_type;
            }
        }
        serialize_tm_file(tm, path_tm);
        free_tm_file(tm);
    }

    uint8_t *path_hoe = get_level_file_path(game_path, ri, ".hoe");
    hoe_file *hoe = parse_hoe_file(path_hoe);
    if (!hoe)
    {
        log_msg(ERRO, "Couldn't parse hoe");
        return 1;
    }

    replace_item_hoe(hoe, uid, new_item_type);

    serialize_hoe_file(hoe, path_hoe);
    serialize_it_file(it, path_it);

    //serialize_it_file(it, path_it);
    free_it_file(it);
    free_hoe_file(hoe);

    free(path_hoe);
    free(path_it);
    free(path_tm);

    return 0;
}

int randomize_item(uint32_t uid)
{
    // we get the new item type
    int random_item_type = get_random_item_type(allowed_items_to_appear,
        LEN_ALLOWED_ITEMS_TO_APPEAR);
    return modify_item(uid, ii_from_it(random_item_type));
}

int randomize_all_rooms(void)
{
    log_msg(LOGA, "Randomizing all rooms");

    for (size_t i = 0; i < NB_ROOMS; i++)
    {
        char *ristr = ristr_from_ri((room_id)i);
        if (randomize_room(ristr) != BUSINESS_OCR_SUCCESS)
        {
            sprintf(msg, "Couldn't randomize room %s", ristr);
            log_msg(ERRO, msg);
            return BUSINESS_OCR_FAIL;
        }

        sprintf(msg, "Randomized room %s", ristr);
        log_msg(LOGM, msg);
    }

    log_msg(LOGA, "All rooms were randomized");

    return BUSINESS_OCR_SUCCESS;
}