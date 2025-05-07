#ifndef PATH_H
#define PATH_H

#define PATH_GAME_SET 1
#define PATH_BACKUPS_SET 2
#define BACKUP_NAME_SET 4
#define PATH_ITEMS_SET 8
#define PATH_ROOM_SET 16
#define PATH_BACKUPS_ITEMS_SET 32
#define PATH_BACKUPS_ROOM_SET 64
#define PATH_LOGS_SET 128

#include <stddef.h>

#include "room.h"

extern int are_paths_set;

extern char path_items[512];
extern char path_room[512];
extern char path_game[512];
extern char path_backups[512];
extern char path_backups_items[512];
extern char path_backups_room[512];
extern char backup_name[64];
extern char path_logs[512];

extern size_t path_game_len;
extern size_t path_backups_len;
extern size_t backup_name_len;
extern size_t path_logs_len;

void set_path_game(char *path);
void set_path_items_from_path_game();
void set_path_room_from_path_game();
void set_path_backups(char *path);
void set_backup_name(char *name);
void set_path_backups_items_from_path_backups();
void set_path_backups_room_from_path_backups();
void set_path_room(struct room *my_room);
void set_path_logs(char *path);
int is_path_game_set();
int is_path_backups_set();
int is_backup_name_set();
int is_path_items_set();
int is_path_room_set();
int is_path_backups_items_set();
int is_path_backups_room_set();

void print_all_paths();
void get_id_from_path_room(char *dst);
void set_path_room_str(char *id);

#endif // PATH_H
