#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "file_io.h"
#include "logger.h"
#include "path.h"
#include "utils.h"

int general_item_spawn_prob[] = { 75, 40, 10 };
int weapon_spawn_prob[] = { 40, 25, 10 };
enum difficulty difficulty_mode = NORMAL;
int difficulties[] = { EASY, NORMAL, HARD };
char *difficulties_str[] = { "EASY", "NORMAL", "HARD" };

// modifies the path variables according to .\OCR.config
void load_config()
{
    char *config_str = str_from_file("OCR.config", NULL);
    char *line = config_str;

    //printf("config-str = <%s>\n", config_str);

    char ***config = config_from_str(config_str);

    for (size_t i = 0; config[i]; i++)
    {
        if (strcmp(config[i][0], "game-path") == 0)
        {
            path(config[i][1]);
        }
        if (strcmp(config[i][0], "backup-path") == 0)
        {
            backup_path(config[i][1]);
        }
        if (strcmp(config[i][0], "backup-name") == 0)
        {
            backup_name_cmd(config[i][1]);
        }
        if (strcmp(config[i][0], "log-path") == 0)
        {
            log_path(config[i][1]);
        }
        if (strcmp(config[i][0], "difficulty") == 0)
        {

            enum difficulty diff = get_difficulty_from_string(config[i][1]);
            if (diff == NODIFF)
            {
                char buf_log[512];
                sprintf(buf_log, "\"%s\" is not a valid difficulty. Please use \"EASY\", \"NORMAL\" or \"HARD\".\n", config[i][1]);
                log(WARN_HIGH, buf_log);
                log(INFO, "The default difficulty is NORMAL.\n");
            }
            else
            {
                difficulty_mode = diff;
            }

        }
        else if (strcmp(config[i][0], "log-level") == 0)
        {
            enum log_type log_t = get_log_type_from_string(config[i][1]);
            if (log_t == NOLOG)
            {
                char buf_log[512];
                sprintf(buf_log, "\"%s\" is not a valid log level. Please use \"EASY\", \"NORMAL\" or \"HARD\".\n", config[i][1]);
                log(WARN_HIGH, buf_log);
                log(INFO, "The default log level is LOG_APP_CMD.\n");
            }
            else
            {
                log_level = log_t;
            }
        }
        else if (strcmp(config[i][0], "reset-log") == 0)
        {
            int bool_res = bool_str(config[i][1]);
            if (bool_res == -1)
            {
                char buf_log[64];
                sprintf(buf_log, "The given reset_log setting is invalid. The default setting is False. Resuming.\n");
                log(WARN_LOW, buf_log);
            }
            else
            {
                reset_log = bool_res;
                char buf_log[64];
                sprintf(buf_log, "Reset Log option set to %d.\n", bool_res);
                log(LOG_MINOR, buf_log);
            }
        }

    }

    //print_config(config);
    //print_all_paths();
    free(config_str);
    free_triple_char_pointer(config);
}

void set_difficulty(int my_difficulty)
{
    difficulty_mode = my_difficulty;
}

enum difficulty get_difficulty_from_string(char *str)
{
    for (size_t i = 0; i < 3; i++)
    {
        if (strcmp(difficulties_str[i], str) == 0)
        {
            return i;
        }
    }

    return NODIFF;
}

enum log_type get_log_type_from_string(char *str)
{
    for (size_t i = 0; i < LOG_TYPES_NB; i++)
    {
        if (strcmp(log_type_str_conf[i], str) == 0)
        {
            return i;
        }
    }

    return NOLOG;
};
