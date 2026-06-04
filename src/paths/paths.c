#include "paths.h"

#include "ocr_config.h"

uint8_t game_path[1024] = {0};

void set_game_path(uint8_t *path)
{
    strncpy(game_path, path, 1024);
}

int setup_paths(void)
{
    // at this point, the config file is supposed to exist and have the
    // `game_path` setting
    // TODO: check that the `data` folder exists, and log a WARN if it doesn't
    set_game_path(get_config_value(ocr_config, "game_path"));
    return PATHS_SUCCESS;
}