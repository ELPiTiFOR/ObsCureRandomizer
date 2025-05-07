#include "state.h"

#include <stddef.h>

#include "file_io.h"
#include "logger.h"
#include "path.h"
#include "room.h"
#include "room_vars.h"
#include "utils.h"

// We need to save _common\\allitems.it and all the
// _levels\\x\\xxxx\\xxxx_n.tm
void save_state()
{
    log(LOG_MINOR, "Saving state\n");

    if (!is_backup_name_set())
    {
        fprintf(stderr, "ERROR: backup_name not set\n");
        return;
    }
    if (!is_path_backups_set())
    {
        fprintf(stderr, "ERROR: path_backups not set\n");
        return;
    }
    if (!is_path_backups_items_set())
    {
        fprintf(stderr, "ERROR: path_backups_items not set\n");
        return;
    }
    if (!is_path_backups_room_set())
    {
        fprintf(stderr, "ERROR: path_backups_room not set\n");
        return;
    }

    // saving allitems.it
    copy_file(path_items, path_backups_items);

    // saving levels
    struct room *my_room;
    for (size_t i = 0; i < ROOMS_IDS_STR_NB; i++)
    {
        set_path_room(rooms[i]);

        char buf_log[1024];
        sprintf(buf_log, "Copying file %s\n", path_room);
        log(LOG_VERY_MINOR, buf_log);

        copy_file(path_room, path_backups_room);
    }
}
