#include "ocr_config.h"

#include "config.h"
#include "logger.h"

config *ocr_config = NULL;
char *ocr_error_messages[] =
{
    "The task succeeded.",
    "The config file `ocr.config` was not found.",
    "The \"game_path\" setting was not found in the config.",
    "The ocr config is already set up.",
};

int setup_ocr_config(void)
{
    // we only do the setup once
    if (ocr_config)
    {
        return OCR_CONFIG_ALREADY_SETUP;
    }

    ocr_config = read_config("ocr.config");
    if (!ocr_config)
    {
        return OCR_CONFIG_FILE_NOT_FOUND;
    }

    // now we check all the settings that are expected
    if (!get_config_value(ocr_config, "game_path"))
    {
        return OCR_CONFIG_GAME_PATH_NOT_FOUND;
    }

    return OCR_CONFIG_SUCCESS;
}

char *get_ocr_config_error_msg(int status)
{
    return ocr_error_messages[status];
}