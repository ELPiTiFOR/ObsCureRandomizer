#include "path.h"

#include <stdio.h>
#include <string.h>

#include "logger.h"
#include "room.h"
#include "utils.h"

int are_paths_set = 0;

char path_game[512];
char path_backups[512];
char backup_name[64];

char path_items[512];
char path_room[512];

char path_backups_items[512];
char path_backups_room[512];

char path_logs[512];

size_t path_game_len = 0;
size_t path_backups_len = 0;
size_t backup_name_len = 0;
size_t path_logs_len = 0;

void set_path_game(char *path)
{
    // TODO: we should check the path exists at least
    strcpy(path_game, path);
    path_game_len = strlen(path);
    are_paths_set |= PATH_GAME_SET;
}

void set_path_items_from_path_game()
{
    strcpy(path_items, path_game);
    strcat(path_items, "\\data\\_common\\allitems.it");
    are_paths_set |= PATH_ITEMS_SET;
}

void set_path_room_from_path_game()
{
    strcpy(path_room, path_game);
    strcat(path_room, "\\data\\_levels\\x\\xyyy\\xyyy_n.tm");
    are_paths_set |= PATH_ROOM_SET;
}

void set_path_backups(char *path)
{
    strcpy(path_backups, path);
    path_backups_len = strlen(path);
    are_paths_set |= PATH_BACKUPS_SET;
}

void set_backup_name(char *name)
{
    strcpy(backup_name, name);
    backup_name_len = strlen(name);
    are_paths_set |= BACKUP_NAME_SET;
}

void set_path_backups_items_from_path_backups()
{
    if (!is_path_backups_set())
    {
        fprintf(stderr, "ERROR: Must provide path_backups before path_backups_items");
        return;
    }

    if (!is_backup_name_set())
    {
        fprintf(stderr, "ERROR: Must provide backup_name before path_backups_items");
        return;
    }

    strcpy(path_backups_items, path_backups);
    strcat(path_backups_items, "\\");
    strcat(path_backups_items, backup_name);
    strcat(path_backups_items, "\\_common\\allitems.it");
    are_paths_set |= PATH_BACKUPS_ITEMS_SET;
}

void set_path_backups_room_from_path_backups()
{
    if (!is_path_backups_set())
    {
        fprintf(stderr, "ERROR: Must provide path_backups before path_backups_items");
        return;
    }

    if (!is_backup_name_set())
    {
        fprintf(stderr, "ERROR: Must provide backup_name before path_backups_items");
        return;
    }

    strcpy(path_backups_room, path_backups);
    strcat(path_backups_room, "\\");
    strcat(path_backups_room, backup_name);
    strcat(path_backups_room, "\\_levels\\x\\xyyy\\xyyy_n.tm");
    are_paths_set |= PATH_BACKUPS_ROOM_SET;
}

// we assume all paths are set and we just have
// to replace the x and xyyy with the proper strings
// for example, b and b008 for the cafeteria
void set_path_room(struct room *my_room)
{
    set_path_room_str(my_room->id);
    /*
    if (!is_path_room_set())
    {
        fprintf(stderr, "ERROR: path_room not set\n");
        return;
    }

    path_room[path_game_len + 14] = my_room->id[0];
    strncpy(path_room + path_game_len + 16, my_room->id, 4);
    strncpy(path_room + path_game_len + 21, my_room->id, 4);

    if (!is_path_backups_room_set())
    {
        //print_array(path_backups_room, 512);
        printf("WARN: path_backups_room not set. Resuming...\n");
        return;
    }

    path_backups_room[path_backups_len + 14 - 5 + backup_name_len + 1] = my_room->id[0];
    strncpy(path_backups_room + path_backups_len + 16 - 5 + backup_name_len + 1, my_room->id, 4);
    strncpy(path_backups_room + path_backups_len + 21 - 5 + backup_name_len + 1, my_room->id, 4);
    */
}

void set_path_room_str(char *id)
{
    if (!is_path_room_set())
    {
        fprintf(stderr, "ERROR: path_room not set\n");
        return;
    }

    path_room[path_game_len + 14] = id[0];
    strncpy(path_room + path_game_len + 16, id, 4);
    strncpy(path_room + path_game_len + 21, id, 4);

    if (!is_path_backups_room_set())
    {
        //print_array(path_backups_room, 512);
        log(WARN_LOW, "path_backups_room not set. Resuming...\n");
        return;
    }

    path_backups_room[path_backups_len + 14 - 5 + backup_name_len + 1] = id[0];
    strncpy(path_backups_room + path_backups_len + 16 - 5 + backup_name_len + 1, id, 4);
    strncpy(path_backups_room + path_backups_len + 21 - 5 + backup_name_len + 1, id, 4);
}

void set_path_logs(char *path)
{
    strcpy(path_logs, path);
    path_logs_len = strlen(path);
    are_paths_set |= PATH_LOGS_SET;
}

int is_path_game_set()
{
    return are_paths_set & PATH_GAME_SET;
}

int is_path_backups_set()
{
    return are_paths_set & PATH_BACKUPS_SET;
}

int is_backup_name_set()
{
    return are_paths_set & BACKUP_NAME_SET;
}

int is_path_items_set()
{
    return are_paths_set & PATH_ITEMS_SET;
}

int is_path_room_set()
{
    return are_paths_set & PATH_ROOM_SET;
}

int is_path_backups_items_set()
{
    return are_paths_set & PATH_BACKUPS_ITEMS_SET;
}

int is_path_backups_room_set()
{
    return are_paths_set & PATH_BACKUPS_ROOM_SET;
}

void print_all_paths()
{
    printf("path_game: ");
    printf("%s", path_game);
    //print_array(path_game);
    puts("");

    printf("path_backups: ");
    printf("%s", path_backups);
    //print_array(path_backups);
    puts("");

    printf("path_backups_items: ");
    printf("%s", path_backups_items);
    //print_array(path_backups_items);
    puts("");

    printf("path_backups_room: ");
    printf("%s", path_backups_room);
    //print_array(path_backups_room);
    puts("");

    printf("path_items: ");
    printf("%s", path_items);
    //print_array(path_items);
    puts("");

    printf("path_room: ");
    printf("%s", path_room);
    //print_array(path_room);
    puts("");
}

void get_id_from_path_room(char *dst)
{
    strncpy(dst, path_room + path_game_len + 16, 4);
    dst[4] = 0;
}







