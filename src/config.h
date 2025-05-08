#ifndef CONFIG_H
#define CONFIG_H

#define DIFFICULTIES_NB 3

enum difficulty
{
    EASY = 0,
    NORMAL,
    HARD,
    NODIFF
};

extern int general_item_spawn_prob[DIFFICULTIES_NB];
extern int weapon_spawn_prob[DIFFICULTIES_NB];
extern enum difficulty difficulty_mode;
extern int difficulties[DIFFICULTIES_NB];
extern char *difficulties_str[DIFFICULTIES_NB];

void load_config();
void set_difficulty(int my_difficulty);
enum difficulty get_difficulty_from_string(char *str);
enum log_type get_log_type_from_string(char *str);

#endif // CONFIG_H
