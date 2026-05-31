#include "hoe_file.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allitems.h"
#include "file_io.h"
#include "file_read.h"
#include "file_write.h"
#include "logger.h"
#include "path.h"
#include "utils.h"

int replace_hidden_item(enum item_loc item_loc, enum item_id item_id)
{
    // we modify the path_room variable
    size_t len = strlen(path_room);
    strcpy(path_room + len - 5, ".hoe");
    path_room[len - 1] = 0;


    FILE *hoe_file = fopen(path_room, "r+b");


    if (!hoe_file)
    {
        char log_buf[512];
        sprintf(log_buf, "Couldn't open hoe file %s\n", path_room);
        log(ERROR, log_buf);
        // we modify the path_room variable again
        strcpy(path_room + len - 5, "_n.tm");
        path_room[len] = 0;
        return 1;
    }

    // we modify the path_room variable again
    strcpy(path_room + len - 5, "_n.tm");
    path_room[len] = 0;

    char loc_buf[4];
    itoa_hex(item_loc, loc_buf);

    ssize_t offset = search_pattern(hoe_file, loc_buf, 4);
    if (offset == -1)
    {
        log(ERROR, "Couldn't find pattern in hoe file\n");
        fclose(hoe_file);
        return 1;
    }


    char zero_buf[3];
    zero_buf[0] = 0;
    zero_buf[1] = 0;
    zero_buf[2] = 0;
    ssize_t curr_offset = offset;
    int checked = 0;

    curr_offset -= 8;
    fseek(hoe_file, curr_offset, SEEK_SET);
    checked = check_pattern(hoe_file, zero_buf, 3);
    //fseek(hoe_file, curr_offset, SEEK_SET);

    if (curr_offset <= 0)
    {
        log(ERROR, "Start of file reached when looking for hidden id\n");
        fclose(hoe_file);
        return 1;
    }

    char hidd_id_c[4];
    enum hidden_id hidd_id = get_hidden_id_from_item_id(item_id);
    itoa_hex(hidd_id, hidd_id_c);

    if (!checked)
    {
        /*
        log(ERROR, "Can't modify this hidden item, other items are of the same type\n");
        fclose(hoe_file);
        return 1;
        */

        // need to add 8 bytes
        curr_offset += 4;
        fclose(hoe_file);

        strcpy(path_room + len - 5, ".hoe");
        path_room[len - 1] = 0;

        size_t written = 0;
        char *content = str_from_file(path_room, &written);

        char *new_content = malloc(written + 8);
        if (!new_content)
        {
            log(ERROR, "Couldn't allocate new_content when inserting hidden item\n");
            return 1;
        }

        for (size_t i = 0; i < curr_offset; i++)
        {
            new_content[i] = content[i];
        }

        char new_hidd_item[8];
        arfillzeros(new_hidd_item, 8);
        new_hidd_item[3] = 1;
        new_hidd_item[7] = hidd_id_c[3];

        for (size_t i = 0; i < 8; i++)
        {
            new_content[curr_offset + i] = new_hidd_item[i];
            printf("writing %02X at offset %d\n", new_hidd_item[i], curr_offset + i);
        }

        size_t to_write = written - curr_offset;
        for (size_t i = 0; i < to_write; i++)
        {
            new_content[curr_offset + 8 + i] = content[curr_offset + i];
            if (i < 30)
            {
                printf("writing (content) %02X at offset %d\n", content[i], curr_offset + i + 8);
            }
        }



        // NOW WE HAVE TO ADD 1 TO THE COUNT OF GROUPS OF 8 AFTER THE STRINGS
        // IN THE EVENT

        unsigned char event[8];
        event[0] = 0x00;
        event[1] = 0x00;
        event[2] = 0x80;
        event[3] = 0x40;
        event[4] = 0x05;
        event[5] = 0x00;
        event[6] = 0x00;
        event[7] = 0x00;

        curr_offset -= search_pattern_str_backwards(new_content + curr_offset, curr_offset + 1, event, 8);
        curr_offset -= 7;
        printf("offset pattern event = %d\n", curr_offset);
        curr_offset += 8;

        while (new_content[curr_offset + 4] != 0)
        {
            printf("let's skip this string at offset %d\n", curr_offset);
            unsigned char tmp[4];
            arfillzeros(tmp, 4);
            for (size_t i = 0; i < 4; i++)
            {
                tmp[i] = new_content[curr_offset + i];
            }

            printf("tmp = %02X %02X %02X %02X\n", tmp[0], tmp[1],tmp[2],tmp[3]);

            // THIS IS DIRTY ASFFFFFFF!!!!!

            curr_offset += 4 + tmp[3];
            printf("let's skip this string at offset %d by 4 and 0x%02X\n", curr_offset, tmp[3]);

            int l = 0;
            for (size_t i = 0; i < 4; i++)
            {
                tmp[i] = new_content[curr_offset + i];
            }
            l = tmp[3];
            while (!l || l == 1)
            {
                curr_offset += 4;
                for (size_t i = 0; i < 4; i++)
                {
                    tmp[i] = new_content[curr_offset + i];
                }

                l = tmp[3];
                printf("l = %d | %02X\n", l, tmp[3]);
            }
        }

        new_content[curr_offset + 3] += 1;

        file_from_string(path_room, new_content, written + 8);

        // we modify the path_room variable again
        strcpy(path_room + len - 5, "_n.tm");
        path_room[len] = 0;

        free(content);
        free(new_content);
        return 0;
    }

    write_at_offset(hoe_file, curr_offset, hidd_id_c, 4);

    fclose(hoe_file);
    return 0;
}