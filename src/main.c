#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "allitems.h"
#include "commands.h"
#include "file_io.h"
#include "room.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); // setting the seed for random numbers
    arfillzeros(path_items, 512); // path_items is the path to the allitems.it file
    arfillzeros(path_room, 512); // path_room is the path to a XXXX_n.tm file
    arfillzeros(path_game, 512); // path_game is the path to the Obscure folder
    groupscat(); // this concatenates all the different item groups into the allitems group

    if (argc < 2)
    {
        printf("usage: .\\ObsCureRandomizer.exe [<options>]\n");
        printf("You may want to execute .\\ObsCureRandomizer.exe --help");
        return 1;
    }

    // first we check if one of the options is '--help', in which case
    // we shouldn't perform any other actions
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            help();
            return 0;
        }
    }

    int performed_action = 0;
    enum room_id e_room = NOROOM;

    // now we manage all other commands/actions
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--path") == 0 && argc > i + 1)
        {
            if (argc > i + 1)
            {
                path(argv[i + 1]);
                i++;
            }
            else
            {
                fprintf(stderr, "ERROR: You must provide a path after the `--path` option");
            }
        }

        if (strcmp(argv[i], "--room") == 0 && argc > i + 1)
        {
            if (path_game[0] == 0)
            {
                fprintf(stderr, "ERROR: You must provide a path before specifying the room\n");
                return 1;
            }

            if (argc > i + 1)
            {
                if (strcmp(argv[i + 1], "b008") == 0)
                {
                    e_room = B008;
                }
                else
                {
                    fprintf(stderr, "ERROR: No such room: %s\n", argv[i + 1]);
                    return 1;
                }

                set_path_room(rooms[e_room]);
                i++;
            }
            else
            {
                fprintf(stderr, "ERROR: You must provide a room ID after the `--room` option");
            }

        }

        if (strcmp(argv[i], "--randomize") == 0)
        {
            if (path_game[0] == 0)
            {
                fprintf(stderr, "ERROR: You must provide a path before randomizing\n");
                return 1;
            }

            performed_action = 1;
            if (e_room == NOROOM)
            {
                for (size_t i = 0; i < ROOMS_NB; i++)
                {
                    set_path_room(rooms[rooms_ids[i]]);
                    randomize_room(rooms_ids[i]);
                }
            }
            else
            {
                randomize_room(e_room);
            }
        }

        if (strcmp(argv[i], "--restore") == 0)
        {
            if (path_game[0] == 0)
            {
                fprintf(stderr, "ERROR: You must provide a path before restoring\n");
                return 1;
            }

            performed_action = 1;
            if (e_room == NOROOM)
            {
                for (size_t i = 0; i < ROOMS_NB; i++)
                {
                    set_path_room(rooms[rooms_ids[i]]);
                    restore_room(rooms[rooms_ids[i]]);
                }
            }
            else
            {
                restore_room(rooms[e_room]);
            }
        }
    }

    if (!performed_action)
    {
        printf("WARN: It seems like you didn't perform any action, wanna try --randomize or --restore?\n");
    }

    return 0;
}
