#include "tm_file.h"

#include <stdlib.h>

#include "file_io.h"
#include "logger.h"
#include "path.h"
#include "utils.h"

uint32_t get_first_number(FILE *file)
{
    uint32_t res = 0;
    uint8_t buf[4];
    size_t r = fread(buf, 1, 4, file);
    if (!r)
    {
        log(ERROR, "Couldn't get first number\n");
        return -1;
    }

    for (size_t i = 0; i < 4; i++)
    {
        res = res << 8;
        res |= buf[i];
    }

    return res;
}

long get_len_of_section_tm_file(FILE *file)
{
    /*
    printf("Before printing this buf at offset\n");
    char buf[32];
    fread(buf, 1, 16, file);
    fseek(file, -16, SEEK_CUR);
    print_hex(buf, 20);
    puts("");
    printf("After printing this buf at offset\n");
    */

    // we skip the id of the type of section
    fseek(file, 4, SEEK_CUR);

    // we get the length of the section
    unsigned char len_buf[5];
    size_t read = fread(len_buf, 1, 4, file);
    if (read == 0)
    {
        return 0;
    }

    len_buf[4] = 0;
    fseek(file, -8, SEEK_CUR);
    long len = buf_to_long(len_buf);

    /*
    print_hex(len_buf, 4);
    puts("");
    printf("len from get_len_of_section_tm_file = %zu", len);
    */
    return len;
}

// doesn't fseek back in place
uint8_t *get_content_of_section_tm_file(FILE *file, size_t len)
{
    uint8_t *res = malloc(len);
    if (!res)
    {
        log(ERROR, "Couldn't alloc content\n");
        return NULL;
    }

    fseek(file, 4, SEEK_CUR);
    size_t r = 0;
    if ((r = fread(res, 1, len, file)) == 0)
    {
        free(res);
        return NULL;
    }

    //fseek(file, -4 - r, SEEK_CUR);
    return res;
}

int get_tm_item_section(FILE *tm_file, tm_item_section *item)
{
    // type
    uint32_t type = get_first_number(tm_file);
    item->type = type;
    item->len_content = get_first_number(tm_file);
    item->item_id = get_first_number(tm_file);
    item->item_location = get_first_number(tm_file);
    item->x_pos = get_first_number(tm_file);
    item->y_pos = get_first_number(tm_file);
    item->z_pos = get_first_number(tm_file);
    item->x_rot = get_first_number(tm_file);
    item->y_rot = get_first_number(tm_file);
    item->z_rot = get_first_number(tm_file);

    uint8_t buf[24];
    fread(buf, 1, 24, tm_file);
    for (size_t i = 0; i < 24; i++)
    {
        item->unknown[i] = buf[i];
    }

    item->len_info_diff = get_first_number(tm_file);

    uint8_t *info_diff = malloc(item->len_info_diff);
    if (!info_diff)
    {
        log(ERROR, "Couldn't malloc for info_diff\n");
        return 1;
    }

    fread(info_diff, 1, item->len_info_diff, tm_file);

    item->info_diff = info_diff;

    return 0;
}

size_t get_offset_of_item_tm_file(FILE *file, uint64_t id_loc)
{
    //size_t old_offset = ftell(file);

    initialize_tm_file(file);
    goto_items_tm_file(file);

    do
    {
        uint64_t curr_item = get_item_id_and_location_tm_file(file);
        if (lsb_from_long(id_loc) == lsb_from_long(curr_item))
        {
            // this is the item we were looking for
            size_t res = ftell(file);
            /*
            printf("Before printing this buf at offset\n");
            char buf[20];
            fread(buf, 1, 16, file);
            print_hex(buf, 20);
            printf("Before printing this buf at offset\n");
            */

            //fseek(file, old_offset, SEEK_SET);
            return res;
        }
    } while (!goto_next_section_tm_file(file));

    // TODO: fseek() si no se encuentra?
    //fseek(file, old_offset, SEEK_SET);
    return 0;
}


void initialize_tm_file(FILE *file)
{
    fseek(file, 4, SEEK_SET);
}

int goto_next_section_tm_file(FILE *file)
{
    long len = get_len_of_section_tm_file(file);
    char buf_log[512];
    //sprintf(buf_log, "Len of current section: %d\n", len);
    //log(LOG_VERY_MINOR, buf_log);

    fseek(file, len + 4, SEEK_CUR);
    size_t final_offset = ftell(file);

    char buf[2];
    if (!fread(buf, 1, 1, file))
    {
        return 1;
    }

    //printf("is feof: %d\n", feof(file));
    //log(LOG_VERY_MINOR, "Went to next section\n");
    fseek(file, final_offset, SEEK_SET);
    return 0;
}

int get_id_of_section_tm_file(FILE *file, unsigned char *buf)
{
    int eof = fread(buf, 1, 4, file);
    buf[4] = 0;
    fseek(file, -4, SEEK_CUR);
    return eof == 0;
}

uint64_t get_item_id_and_location_tm_file(FILE *file)
{
    unsigned char data[8];
    int err = get_buf_from_file(file, 8, 15, data);
    if (err)
    {
        return 0;
    }

    return buf_to_long_long(data);
}

void print_tm_item_section(tm_item_section *item)
{
    printf("Type: %d\n", item->type);
    size_t len_content = item->len_content;
    printf("Length of section: %d\n", item->len_content);
    printf("    Item ID: %04X\n", item->item_id);
    printf("    Item location: %06X\n", item->item_location);

    printf("    X pos: %d\n", item->x_pos);
    printf("    Y pos: %d\n", item->y_pos);
    printf("    Z pos: %d\n", item->z_pos);

    printf("    X rot: %d\n", item->x_rot);
    printf("    Y rot: %d\n", item->y_rot);
    printf("    Z rot: %d\n", item->z_rot);

    for (size_t i = 0; i < 24; i++)
    {
        if (i % 16 == 0)
        {
            printf("    ");
        }

        printf("%02X ", item->unknown[i]);

        if ((i + 1) % 16 == 0)
        {
            putchar('\n');
        }
    }
    putchar('\n');

    printf("    Length of Info and Diff: %d\n", item->len_info_diff);
    uint32_t len_info_diff = item->len_info_diff;
    printf("    Info diff: <");
    for (size_t i = 0; i < len_info_diff; i++)
    {
        printf("%c", item->info_diff[i]);
    }

    puts(">");
}

int goto_items_tm_file(FILE *file)
{
    int eof = 0;
    char id[5];
    get_id_of_section_tm_file(file, id);
    while (id[3] != '\x08' && !eof)
    {
        char buf_log[512];
        sprintf(buf_log, "At section 0x%02X\n", id[3]);
        //log(LOG_VERY_MINOR, buf_log);
        eof = goto_next_section_tm_file(file);
        eof = get_id_of_section_tm_file(file, id);
    }

    return eof;
}

void goto_end_tm_file(FILE *file)
{
    int eof = 0;
    unsigned char id[5];
    get_id_of_section_tm_file(file, id);
    while (id[3] != '\x0A'&& !eof)
    {
        char buf_log[512];
        sprintf(buf_log, "At section 0x%02X\n", id[3]);
        //log(LOG_VERY_MINOR, buf_log);
        eof = goto_next_section_tm_file(file);
        eof = get_id_of_section_tm_file(file, id);
    }
}

// return 0 if the item is there, 1 otherwise
int check_item(FILE *room_file, uint64_t item)
{
    initialize_tm_file(room_file);
    goto_items_tm_file(room_file);

    int loc = lsb_from_long(item);

    //printf("Entering check_item loop\n");
    do
    {
        uint64_t curr_item = get_item_id_and_location_tm_file(room_file);
        int curr_loc = lsb_from_long(curr_item);

        if (loc == curr_loc)
        {
            return 0;
        }
    } while (!goto_next_section_tm_file(room_file));

    return 1;
}

int check_items_in_room(struct room *room)
{
    FILE *room_file = fopen(path_room, "rb");
    if (!room_file)
    {
        char buf_log[1024];
        sprintf(buf_log, "Couldn't load FILE room_file: %s\n", path_room);
        log(ERROR, buf_log);
        return 2;
    }

    for (size_t i = 0; room->items[i]; i++)
    {
        char buf_log[512];
        sprintf(buf_log, "Checking item %016llX\n", room->items[i]);
        log(LOG_VERY_MINOR, buf_log);
        //printf("Checking item %016llX\n", room->items[i]);

        if (check_item(room_file, room->items[i]) != 0)
        {
            fclose(room_file);
            sprintf(buf_log, "Missing item %016llX\n", room->items[i]);
            //printf("Missing item %016llX\n", room->items[i]);
            log(LOG_VERY_MINOR, buf_log);
            return 1;
        }
        //printf("Checked item %016llX\n", room->items[i]);
        sprintf(buf_log, "Checked item %016llX\n", room->items[i]);
        log(LOG_VERY_MINOR, buf_log);
    }

    fclose(room_file);
    return 0;
}


tm_file *parse_tm_file(char *path)
{
    FILE *file = fopen(path, "r+b");
    if (!file)
    {
        char log_buf[512];
        sprintf(log_buf, "Couldn't open tm_file %s\n", path);
        log(ERROR, log_buf);
        return NULL;
    }

    tm_file *tm = malloc(sizeof(tm_file));
    if (!tm)
    {
        log(ERROR, "Couldn't alloc tm\n");
        fclose(file);
        return NULL;
    }

    uint32_t fn = get_first_number(file);
    if (fn == -1)
    {
        fclose(file);
        free_tm_file(tm);
        return NULL;
    }

    size_t len_sections = 0;
    size_t len_item_sections = 0;
    char buf_id[5];
    tm_generic_section *sections = NULL;
    tm_item_section *item_sections = NULL;

    while (get_id_of_section_tm_file(file, buf_id) == 0)
    {
        uint32_t type = 0;
        for (size_t i = 0; i < 4; i++)
        {
            type = type << 8;
            type |= buf_id[i];
        }

        if (type == ITEM_SECTION)
        {
            len_item_sections += 1;
            tm_item_section *item_sections2 = realloc(item_sections, len_item_sections * sizeof(tm_item_section));
            if (!item_sections2)
            {
                fclose(file);
                free_tm_file(tm);
                return NULL;
            }

            item_sections = item_sections2;
            tm_item_section *tm_item_section = item_sections + len_item_sections - 1;
            get_tm_item_section(file, tm_item_section);
            /*
            printf("#################\n");
            print_tm_item_section(item_sections);
            printf("#################\n");
            */
            continue;
        }

        len_sections += 1;

        tm_generic_section *sections2 = realloc(sections, len_sections * sizeof(tm_generic_section));
        if (!sections2)
        {
            fclose(file);
            free_tm_file(tm);
            return NULL;
        }

        sections = sections2;
        tm_generic_section *tm_section = sections + len_sections - 1;

        tm_section->type = type;
        size_t len_content = get_len_of_section_tm_file(file);
        tm_section->len_content = len_content;
        uint8_t *content = get_content_of_section_tm_file(file, len_content);
        if (!content)
        {
            fclose(file);
            free_tm_file(tm);
            return NULL;
        }

        tm_section->content = content;
    }

    tm->first_number = fn;
    tm->len_sections = len_sections;
    tm->sections = sections;
    tm->len_item_sections = len_item_sections;
    tm->items = item_sections;
}

void free_tm_file(tm_file *tm)
{
    tm_generic_section *p = tm->sections;
    size_t len_sections = tm->len_sections;
    for (size_t i = 0; i < len_sections; i++)
    {
        free(p->content);
    }

    free(tm->sections);
}

void print_tm_generic_section(tm_generic_section *tm_sec)
{
    printf("Type: %d\n", tm_sec->type);
    size_t len_content = tm_sec->len_content;
    printf("Length of section: %d\n", tm_sec->len_content);

    for (size_t i = 0; i < len_content; i++)
    {
        printf("%02X ", tm_sec->content[i]);

        if ((i + 1) % 16 == 0)
        {
            putchar('\n');
        }
    }

    putchar('\n');
}


void print_tm_file(tm_file *tm)
{
    printf("# First number: %d\n", tm->first_number);
    size_t len_sections = tm->len_sections;
    size_t len_item_sections = tm->len_item_sections;
    printf("# Sections: \n");
    for (size_t i = 0; i < len_sections; i++)
    {
        print_tm_generic_section(&tm->sections[i]);
    }

    printf("# Items: \n");
    for (size_t i = 0; i < len_item_sections; i++)
    {
        tm_item_section *it = &tm->items[i];
        print_tm_item_section(it);
    }
}
