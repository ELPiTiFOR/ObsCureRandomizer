#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

struct pair
{
    const char *key;
    char *value;
};

typedef struct
{
    struct pair *pairs;
    size_t pairs_count;
} config;

config *read_config(char *path);
char *get_config_value(config *config, char *key);
int is_key_in_config(config *config, char *key);
void free_config(config *config);
void print_config(config *config);

#endif /* !CONFIG_H */