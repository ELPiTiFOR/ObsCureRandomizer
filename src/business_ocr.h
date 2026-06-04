#ifndef BUSINESS_OCR_H
#define BUSINESS_OCR_H

#include <stdint.h>

#include "item.h"

#define BUSINESS_OCR_SUCCESS 0
#define BUSINESS_OCR_FAIL 1

int randomize_room(char *room_id_string);
int randomize_item(uint32_t uid);
int modify_item(uint32_t uid, item_index ii);
int randomize_all_rooms(void);

#endif /* !BUSINESS_OCR_H */