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
    //print_every_room();
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
    log_file = fopen(path_logs, "a+");
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
                fprintf(stderr, "ERROR: You must provide a path after the `--path` option");
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
                fprintf(stderr, "ERROR: You must provide a path after the `--backup-path` option");
            }
        }

        /*
        if (strcmp(argv[i], "--log-path") == 0 && argc > i + 1)
        {
            if (argc > i + 1)
            {
                log_path(argv[i + 1]);
                i++;
            }
            else
            {
                fprintf(stderr, "ERROR: You must provide a path after the `--backup-path` option");
            }
        }
        */

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

                // TODO: improve format log
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



























