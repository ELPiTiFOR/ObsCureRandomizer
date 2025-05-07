#include "tm_file.h"

#include "file_io.h"
#include "logger.h"
#include "path.h"
#include "utils.h"

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

// moves file to the next section
// returns whether EOF was reached
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






