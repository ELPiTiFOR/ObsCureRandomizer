#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "business_ocr.h"
#include "commands_ocr.h"
#include "config.h"
#include "item.h"
#include "logger.h"
#include "ObsCureInfoHeader/obscure_paths.h"
#include "ocr_config.h"
#include "paths.h"
#include "room.h"
#include "tm_file.h"

int main(int argc, char **argv)
{
    // setting the seed for random numbers
    srand(time(NULL));

    // we setup the config of the program
    int config_status;
    if ((config_status = setup_ocr_config()) != OCR_CONFIG_SUCCESS)
    {
        log_msg(ERRO, get_ocr_config_error_msg(config_status));
        return 1;
    }

    // we setup the paths needed by the program
    if (setup_paths() != PATHS_SUCCESS)
    {
        log_msg(ERRO, "Error while setting up paths.");
        return 1;
    }

    // we read the user's arguments and execute the commands
    return execute_commands_ocr(argc, argv);
}