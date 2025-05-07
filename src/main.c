#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "allitems.h"
#include "commands.h"
#include "config.h"
#include "file_io.h"
#include "logger.h"
#include "room.h"
#include "path.h"
#include "state.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); // setting the seed for random numbers
    arfillzeros(path_items, 512); // path_items is the path to the allitems.it file
    arfillzeros(path_room, 512); // path_room is the path to a XXXX_n.tm file
    arfillzeros(path_game, 512); // path_game is the path to the Obscure folder
    arfillzeros(path_backups, 512); // path_backups is the path to the folder
                                    //where all the backups/states will be saved
    arfillzeros(path_backups_items, 512);
    arfillzeros(path_backups_room, 512);
    arfillzeros(path_logs, 512);
    arfillzeros(log_buf, LOG_BUF_SIZE);
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

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--log-path") == 0 && argc > i + 1)
        {
            log_path(argv[i + 1]);
        }
    }

    if (!log_file)
    {
        set_default_log_file();
    }

    // now we load the config file
    load_config();
    log(LOG_MINOR, "Loaded config\n");

    // first we check if one of the options is '--test', in which case
    // we shouldn't perform any other actions
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--test") == 0)
        {
            test();
            return 0;
        }
    }

    // if there's a script to be executed, we execute it and do nothing else
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--exec-script") == 0)
        {
            if (argc > i + 1)
            {
                script(argv[i + 1]);
                log(LOG_APP_CMD, "Exiting.\n");
                log_end_log();
                return 0;
            }

            log(LOG_APP_CMD, "Executing script .\\script.ocr\n");
            //printf("LOG: executing script .\\script.ocr\n");
            script(".\\script.ocr");
            log(LOG_APP_CMD, "Exiting.\n");
            log_end_log();
            return 0;
        }
    }

    log(LOG_APP_CMD, "Executing commands.\n");
    int ret = execute_commands(argc, argv);
    log(LOG_APP_CMD, "Exiting.\n");
    log_end_log();
    return ret;
}
