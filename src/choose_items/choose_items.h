#ifndef CHOOSE_ITEMS_H
#define CHOOSE_ITEMS_H

#include <stddef.h>

#include "item.h"

#define LEN_FORBIDDEN_UIDS 22
#define LEN_FORBIDDEN_ITEMS 13
#define LEN_FORBIDDEN_ITEMS_TO_APPEAR 19
#define LEN_ALLOWED_ITEMS_TO_APPEAR 21

extern item_uid forbidden_uids[LEN_FORBIDDEN_UIDS];
extern item_index forbidden_items[LEN_FORBIDDEN_ITEMS];
extern item_index forbidden_items_to_appear[LEN_FORBIDDEN_ITEMS_TO_APPEAR];
extern item_index allowed_items_to_appear[LEN_ALLOWED_ITEMS_TO_APPEAR];

int is_item_uid_forbidden(item_uid uid);
int is_item_forbidden(item_index ii);
item_type get_random_item_type(item_index *group, size_t len_group);

#endif /* !CHOOSE_ITEMS_H */