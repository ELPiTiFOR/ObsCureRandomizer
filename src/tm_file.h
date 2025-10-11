#ifndef TM_FILE_H
#define TM_FILE_H

#include <stdint.h>
#include <stdio.h>

#include "room.h"

// tm files

// get the length of the current section of the `.tm` file
// this function should be used when the offset of the FILE * is at the first
// byte of the id of the section
long get_len_of_section_tm_file(FILE *file);
size_t get_offset_of_item_tm_file(FILE *file, uint64_t id_loc);

// set the file offset to the first byte of the first section ID
// in other words, skip the first 4 bytes of the file
void initialize_tm_file(FILE *file);

// moves file to the next section
// returns whether EOF was reached
int goto_items_tm_file(FILE *file);

// moves file to last non 0x0A section or to EOF
void goto_end_tm_file(FILE *file);

// get the item id and location in an 8 byte integer
// the file must be at the first byte of an item section
uint64_t get_item_id_and_location_tm_file(FILE *file);

// moves file to the 1st byte of the next section
// if EOF is reached, file stays in EOF
int goto_next_section_tm_file(FILE *file);

// returns 0 if all the items in room are in the respective `.tm` file
// returns 1 otherwise
// path_room MUST BE SET
int check_items_in_room(struct room *room);

#endif // TM_FILE_H
