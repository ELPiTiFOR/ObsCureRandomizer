#ifndef TM_FILE_H
#define TM_FILE_H

#include <stdint.h>
#include <stdio.h>

#include "room.h"

// tm files
long get_len_of_section_tm_file(FILE *file);
size_t get_offset_of_item_tm_file(FILE *file, uint64_t id_loc);
void initialize_tm_file(FILE *file);
int goto_items_tm_file(FILE *file);
void goto_end_tm_file(FILE *file);
uint64_t get_item_id_and_location_tm_file(FILE *file);
int goto_next_section_tm_file(FILE *file);
int check_items_in_room(struct room *room);

#endif // TM_FILE_H
