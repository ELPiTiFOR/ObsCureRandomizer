#ifndef PATHS_H
#define PATHS_H

#include <stdint.h>
#include <string.h>

#define PATHS_SUCCESS 0

extern uint8_t game_path[1024];

void set_game_path(uint8_t *path);
int setup_paths(void);

#endif /* !PATHS_H */