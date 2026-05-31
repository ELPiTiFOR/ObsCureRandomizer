#ifndef TM_FILE_H
#define TM_FILE_H

#include <stdint.h>
#include <stdio.h>

#include "room.h"

#define ITEM_SECTION 8

// tm files

typedef struct
{
    uint32_t type;
    uint32_t len_content;
    uint8_t *content;
} tm_generic_section;

typedef struct
{
    uint32_t type;
    uint32_t len_content;
    // change to enum item_id!!!
    uint32_t item_id;
    uint32_t item_location;
    uint32_t x_pos;
    uint32_t y_pos;
    uint32_t z_pos;
    uint32_t x_rot;
    uint32_t y_rot;
    uint32_t z_rot;
    uint8_t unknown[24];
    uint32_t len_info_diff;
    uint8_t *info_diff;
} tm_item_section;

typedef struct
{
    int first_number;
    size_t len_sections;
    size_t len_item_sections;
    tm_generic_section *sections;
    tm_item_section *items;
} tm_file;

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

tm_file *parse_tm_file(char *path);
void print_tm_file(tm_file *tm);
void free_tm_file(tm_file *tm);

#endif // TM_FILE_H
