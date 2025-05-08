#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "logger.h"
#include "path.h"
#include "room.h"
#include "room_debug.h"
#include "room_vars.h"
#include "state.h"
#include "utils.h"

void help()
{
    printf("DESCRIPTION:\n");
    printf("\tRandomizes items in ObsCure.\n");
    printf("WARNING:\n");
    printf("\tThis program modifies your game files. If something goes wrong, the game files could go corrupted\n");
    printf("\tand the game could stop working. You are responsible for the fate of your game files. Think about\n");
    printf("\tmaking a backup of your files just in case. Specially the folder `_levels` and the `allitems.it` file\n");
    printf("\twhich are the files this programs modifies. The paths to each one of them are the ones that follow:\n");
    printf("\t            `$OBS_PATH\\data\\_levels`\n");
    printf("\t            `$OBS_PATH\\data\\_common\\allitems.it`\n");
    printf("\tFor more information on this program, refer to the README.\n");
    printf("USAGE:\n");
    printf("\t.\\ObsCureRandomizer.exe [<options>]\n");
    printf("OPTIONS:\n");
    printf("\t--help: show this help\n");
    printf("\t--room <room_id>: select a room by ID\n");
    printf("\t--path <game_path>: select the absolute path where the game is installed (mandatory)\n");
    printf("\t--randomize: if a room was selected, it randomizes it. Otherwise, it randomizes the whole game\n");
    printf("\t--restore: if a room was selected, it restores it to its original state. Otherwise, it\n");
    printf("\t           randomizes the whole game\n");
    printf("\tAnd more, read the README please.\n");
    printf("EXAMPLES:\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --room b008 --randomize\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --room b008 --restore\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --randomize\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --restore\n");
    printf("\t.\\ObsCureRandomizer.exe --help\n");
}

void test()
{
    //path("C:\\SteamLibrary\\steamapps\\common\\Obscure");
    print_every_room();
    //print_struct_rooms_ids();
    //print_enum_rooms_ids();
    //print_enum_rooms_array();
}

// sets up path_game and path_items
void path(char *path)
{
    char buf_log[1024];
    sprintf(buf_log, "Setting game path: %s\n", path);
    log(LOG_APP_CMD, buf_log);
    // TODO: we should check the path exists at least
    set_path_game(path);
    set_path_items_from_path_game();
    set_path_room_from_path_game();
}

void log_path(char *path)
{
    set_path_logs(path);

    char fopen_flag[] = "a+";
    if (reset_log)
    {
        strcpy(fopen_flag, "w+");
    }

    char buf_log[1024];
    sprintf(buf_log, "Opening %s with %s flags.\n", path, fopen_flag);
    log(LOG_MINOR, buf_log);

    log_file = fopen(path_logs, fopen_flag);
    if (!log_file)
    {
        fprintf(stderr, "ERROR: couldn't open log_file: %s", path_logs);
        set_default_log_file();
        return;
    }

    log_start_log();
}

void backup_path(char *path)
{
    char buf_log[1024];
    sprintf(buf_log, "Setting backup path: %s\n", path);
    log(LOG_APP_CMD, buf_log);
    set_path_backups(path);
}


void backup_name_cmd(char *name)
{
    char buf_log[1024];
    sprintf(buf_log, "Setting backup name: %s\n", name);
    log(LOG_APP_CMD, buf_log);
    set_backup_name(name);
    set_path_backups_items_from_path_backups();
    set_path_backups_room_from_path_backups();
    //print_all_paths();
}

int execute_commands(int argc, char **argv)
{
    int performed_action = 0;
    enum room_id e_room = NOROOM;
    uint32_t item = 0x00;
    uint32_t item_id = 0x00;

    // now we manage all other commands/actions
    for (int i = 0; i < argc; i++)
    {
        char buf_log[1024];
        sprintf(buf_log, "Managing argument number %d: %s\n", i, argv[i]);
        log(LOG_VERY_MINOR, buf_log);

        if (strcmp(argv[i], "--path") == 0 && argc > i + 1)
        {
            if (argc > i + 1)
            {
                path(argv[i + 1]);
                i++;
            }
            else
            {
                log(ERROR, "You must provide a path after the `--path` option\n");
            }
        }

        if (strcmp(argv[i], "--backup-path") == 0 && argc > i + 1)
        {
            if (argc > i + 1)
            {
                backup_path(argv[i + 1]);
                i++;
            }
            else
            {
                log(ERROR, "You must provide a path after the `--backup-path` option\n");
            }
        }

        if (strcmp(argv[i], "--backup-name") == 0 && argc > i + 1)
        {
            if (!is_path_backups_set())
            {
                log(ERROR, "You must provide path_backup before backup_name\n");
                return 1;
            }

            if (argc > i + 1)
            {
                backup_name_cmd(argv[i + 1]);
                i++;
            }
            else
            {
                log(ERROR, "You must provide a path after the `--backup-path` option\n");
                return 1;
            }
        }

        if (strcmp(argv[i], "--room") == 0 && argc > i + 1)
        {
            if (path_game[0] == 0)
            {
                log(ERROR, "You must provide a path before specifying the room\n");
                return 1;
            }

            if (argc > i + 1)
            {
                e_room = get_e_room_from_id(argv[i + 1]);

                sprintf(log_buf, "Setting room %s.\n", rooms[e_room]->id);
                log(LOG_APP_CMD, log_buf);

                if (e_room == NOROOM)
                {
                    sprintf(log_buf, "No such room: %s\n", argv[i + 1]);
                    log(ERROR, log_buf);
                    return 1;
                }

                set_path_room(rooms[e_room]);
                i++;
            }
            else
            {
                log(ERROR, "You must provide a room ID after the `--room` option\n");
            }
        }

        if (strcmp(argv[i], "--randomize") == 0)
        {
            if (!is_path_game_set())
            {
                log(ERROR, "You must provide a path before randomizing\n");
                return 1;
            }

            performed_action = 1;
            if (e_room == NOROOM)
            {
                log(LOG_APP_CMD, "Randomizing game\n");
                /*
                for (size_t i = 0; i < ROOMS_NB; i++)
                {
                    set_path_room(rooms[rooms_ids[i]]);
                    randomize_room(rooms_ids[i]);
                }
                */
                for (size_t i = 0; i < ROOMS_IDS_STR_NB; i++)
                {
                    struct room *my_room = rooms[rooms_ids[i]];
                    set_path_room(my_room);
                    randomize_room2(my_room);
                }
            }
            else
            {
                char buf_log[512];
                sprintf(buf_log, "Randomizing room %s\n", rooms_ids_str[e_room]);
                log(LOG_APP_CMD, buf_log);
                struct room *my_room = rooms[e_room];
                randomize_room2(my_room);
            }
        }

        if (strcmp(argv[i], "--restore") == 0)
        {
            if (!is_path_game_set())
            {
                log(ERROR, "You must provide a path before restoring\n");
                return 1;
            }

            performed_action = 1;
            if (e_room == NOROOM)
            {
                log(LOG_APP_CMD, "Restoring game\n");
                restore_allitems();
                for (size_t i = 0; i < ROOMS_IDS_STR_NB; i++)
                {
                    //set_path_room(rooms[rooms_ids[i]]);
                    //restore_room(rooms[rooms_ids[i]]);
                    restore_room2(rooms_ids[i]);
                }
            }
            else
            {
                // TODO: format log
                log(LOG_APP_CMD, "Restoring room\n");
                //restore_room(rooms[e_room]);
                restore_room2(e_room);
            }
        }

        if (strcmp(argv[i], "--backup") == 0)
        {
            if (!is_path_backups_set())
            {
                log(ERROR, "You must provide a path for the backups before making a backup\n");
                return 1;
            }
            if (!is_backup_name_set())
            {
                log(ERROR, "You must provide a backup name before making the backup\n");
                return 1;
            }

            log(LOG_APP_CMD, "Making backup\n");
            performed_action = 1;
            save_state();
        }

        else if (strcmp(argv[i], "--item") == 0)
        {
            if (argc <= i + 1)
            {
                log(ERROR, "You must provide an item location (a 4 bytes integer in hexadecimal format) after the `--item` option.\n");
                return 1;
            }

            item = my_atoi_base(argv[i + 1], 16);
            if (item == 0)
            {
                log(ERROR, "Invalid item location.\n");
                return 1;
            }

            sprintf(buf_log, "Item location %06lX set.\n", item);
            log(LOG_APP_CMD, buf_log);

            i++;
        }

        else if (strcmp(argv[i], "--replace") == 0)
        {
            if (argc <= i + 1)
            {
                log(ERROR, "You must provide an item ID (a 4 bytes integer in hexadecimal format) after the `--replace` option.\n");
                return 1;
            }

            item_id = my_atoi_base(argv[i + 1], 16);
            if (item_id == 0 || !is_a_valid_item_id(item_id))
            {
                log(ERROR, "Invalid item ID\n");
                return 1;
            }

            if (e_room == NOROOM)
            {
                log(ERROR, "You must provide a room before replacing an item\n");
                return 1;
            }

            // opening room_file
            FILE *room_file = fopen(path_room, "r+b");
            if (!room_file)
            {
                log(ERROR, "Couldn't fopen() path_room when replacing\n");
                return 1;
            }

            // opening items_file
            FILE *items_file = fopen(path_items, "r+b");
            if (!items_file)
            {
                log(ERROR, "Couldn't fopen() path_items when replacing\n");
                fclose(room_file);
                return 1;
            }

            sprintf(buf_log, "Replacing item at location %06lX with item with ID %04lX.\n", item, item_id);
            log(LOG_APP_CMD, buf_log);
            replace_item(room_file, items_file, item, item_id);
            performed_action = 1;
            fclose(items_file);
            fclose(room_file);
        }
        else if (strcmp(argv[i], "--delete") == 0)
        {
            if (argc <= i + 1)
            {
                log(ERROR, "You must provide an item ID (a 4 bytes integer in hexadecimal format) after the `--delete` option.\n");
                return 1;
            }

            item = my_atoi_base(argv[i + 1], 16);
            if (item == 0)
            {
                log(ERROR, "Invalid item location\n");
                return 1;
            }

            if (e_room == NOROOM)
            {
                log(ERROR, "You must provide a room before replacing an item\n");
                return 1;
            }

            i++;
            sprintf(buf_log, "Deleting item at location %06lX from room with ID %s.\n", item, rooms_ids_str[e_room]);
            log(LOG_APP_CMD, buf_log);
            delete_item(item);
            performed_action = 1;
        }
    }

    if (!performed_action)
    {
        log(WARN_HIGH, "It seems like you didn't perform any action, wanna try --randomize or --restore?\n");
    }

    return 0;
}

void script(char *path_script)
{
    char *script_content = str_from_file(path_script, NULL);
    int argc;
    char **argv = str_to_argv(script_content, &argc);
    if (!argv)
    {
        log(ERROR, "Couldn't str_to_argv\n");
        return;
    }

    free(script_content);
    execute_commands(argc, argv);
    free_argv(argv);
}



























