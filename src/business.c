#include "business.h"

#include "path.h"
#include "room.h"
#include "tm_file.h"

void parse_and_print_tm_file(struct room *my_room)
{
    set_path_room(my_room);
    tm_file *tm = parse_tm_file(path_room);
    print_tm_file(tm);
    free_tm_file(tm);
}