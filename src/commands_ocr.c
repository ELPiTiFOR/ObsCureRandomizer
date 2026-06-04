#include "commands.h"

#include <stdio.h>
#include <string.h>

#include "business_ocr.h"
#include "logger.h"

void help()
{
    printf("usage: .\\ObsCureRandomizer.exe [OPTIONS]\n");
    printf("Here's a list of options:\n");
    printf("    --randomize-all: randomizes all rooms\n");
}

int execute_commands_ocr(int argc, char **argv)
{
    if (argc == 1)
    {
        help();
    }

    // we iterate through every argument
    for (int i = 1; i < argc; i++)
    {
        // RANDOMIZE ALL ROOMS
        // no args needed
        if (strcmp(argv[i], "--randomize-all") == 0)
        {
            if (randomize_all_rooms() != BUSINESS_OCR_SUCCESS)
            {
                log_msg(ERRO, "Error while randomizing all rooms");
            }
        }
        // UNKNOWN COMMAND
        else
        {
            char msg[512] = {0};
            sprintf(msg, "Unknown command \"%s\", ignoring.", argv[i]);
            log_msg(WARN, msg);
        }
    }

    return 0;
}