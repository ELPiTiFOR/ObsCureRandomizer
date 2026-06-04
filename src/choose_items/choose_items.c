#include "choose_items.h"

#include <stddef.h>

#include "random.h"

item_uid forbidden_uids[LEN_FORBIDDEN_UIDS] =
{
    A004_OLD_PISTOL,
    A004_TAPE,
    A004_WEAK_FLASHLIGHT,
    B008_BAT,
    B008_ENERGY_DRINK,
    B010_REEL_OF_WIRE,
    B102_SCREWDRIVER,
    B106_KEY1,
    B106_KEY2,
    C004_OLD_PISTOL,
    C009_NEEDLE,
    C109_NEEDLE,
    E102_FUSES1,
    E103_WHEEL1,
    F003_WOOD_PLANK1,
    G004_PLIERS,
    G004_HOOK,
    G016_OLD_PISTOL,
    I001_KEY1,
    I001_BASEMENT_PLAN2,
    I001_DOCUMENT3,
    J001_CELLPHONE,
};

item_index forbidden_items[LEN_FORBIDDEN_ITEMS] =
{
    KEY,
    NEEDLE,
    HOOK,
    PLIERS,
    PIECE_OF_PAPER,
    CELLPHONE,
    FUSES,
    STATUETTE,
    LEVER,
    WHEEL,
    PHOTO,
    FILM,
    SAFE,
};

item_index forbidden_items_to_appear[LEN_FORBIDDEN_ITEMS_TO_APPEAR] =
{
    STATUETTE,
    SAFE,
    HOOK,
    FUSES,
    WOOD_PLANK,
    NEEDLE,
    SCREWDRIVER,
    PIECE_OF_PAPER,
    LEVER,
    FILM,
    PHOTO,
    DOCUMENT,
    MAP,
    BASEMENT_PLAN,
    KEY,
    PLIERS,
    WHEEL,
    CELLPHONE,
    VIDEO_TAPE,
};

item_index allowed_items_to_appear[LEN_ALLOWED_ITEMS_TO_APPEAR] =
{
    WEAK_FLASHLIGHT,
    BAT,
    OLD_PISTOL,
    LASER,
    SHOTGUN,
    METAL_BAR,
    LIGHT_GRENADE,
    GUN_WITH_FLASHLIGHT,
    YELLOW_FLASHLIGHT,
    POWERFUL_FLASHLIGHT,
    AUTOMATIC_PISTOL,
    REVOLVER,
    DOUBLE_BARREL_SHOTGUN,
    ALUMINIUM_BAT,
    LASER_GUN,
    SHOTGUN_AMMO,
    HANDGUN_AMMO,
    DISC,
    TAPE,
    MEDKIT,
    ENERGY_DRINK,
};

int is_item_uid_forbidden(item_uid uid)
{
    for (size_t i = 0; i < LEN_FORBIDDEN_UIDS; i++)
    {
        if (forbidden_uids[i] == uid)
        {
            return 1;
        }
    }

    return 0;
}

int is_item_forbidden(item_index ii)
{
    for (size_t i = 0; i < LEN_FORBIDDEN_ITEMS; i++)
    {
        if (forbidden_items[i] == ii)
        {
            return 1;
        }
    }

    return 0;
}

item_type get_random_item_type(item_index *group, size_t len_group)
{
    size_t random_index = random_number(0, len_group - 1);
    item_index random_ii = group[random_index];
    return it_from_ii(random_ii);
}