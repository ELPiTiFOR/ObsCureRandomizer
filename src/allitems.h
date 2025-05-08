#ifndef ALLITEMS_H
#define ALLITEMS_H

#define WEAPONS_NB 14
#define DAMAGE_WEAPONS_NB 10
#define RESSOURCES_NB 6
#define GUNS_NB 7

#define ITEMS_GROUPS_NB 5

#include <stddef.h>
#include <stdint.h>

/*
** Items:
** ENER = Energy Drink
** FAID = First Aid Kit
** TAPE = Sticky Tape
** DISC = Disc
**
** Weapons:
** BBAT = Yellow Bat
** MBAR = Metal Bar
** ABAT = Aluminium Bat
** OPIS = Old Pistol
** APIS = Automatic Pistol
** FGUN = Gun with Flashlight
** LGUN = Gun with Laser
** REVO = Revolver
** SHOT = Shotgun
** DBAR = Double Barrel Shotgun
**
** LASR = Laser
**
** FLSW = Flashlight Weak
** FLSY = Flashlight Yellow
** FLSP = Flashlight Powerful
**
** PAMM = Pistoll Ammo
** SAMM = Shotgun Ammo
*/

// this enum associates each pickable item with its location,
// which is used in both allitems.it and each one of the
// level files
enum item_loc
{
    CAFE_ENER = 0x00020403,
    CAFE_BBAT = 0X00020402,
    CAFE_DISC = 0x00020401,
    INFI_DISC = 0x00053383,
    INFI_FAID1 = 0x00053386,
    INFI_FAID2 = 0x00053385,
    INFI_FAID3 = 0x00053384
};

// this enum associates each pickable item with its ID
enum item_id
{
    // WEAPONS
    BBAT = 0x00CA,
    MBAR = 0x00D1,
    ABAT = 0x00DA,

    OPIS = 0x00CB,
    APIS = 0x00D7,
    FGUN = 0x00D3,
    REVO = 0x00D8,
    LGUN = 0x00DC,
    SHOT = 0x00CE,
    DBAR = 0x00D9,

    LASR = 0x00CC,

    FLSW = 0X00C9,
    FLSY = 0x00D5,
    FLSP = 0x00D6,



    // RESSOURCE ITEMS
    ENER = 0x0264,
    DISC = 0x025A,
    FAID = 0x025F,
    TAPE = 0x025B,

    SAMM = 0x0130,
    PAMM = 0x0132,

    NONE = 0x0000
};

enum items_group
{
    ALLITEMS = 0,
    WEAPONS,
    DAMAGE_WEAPONS,
    GUNS,
    RESSOURCES
};

extern size_t allitems_nb;
extern enum item_id allitems[256];
extern enum item_id weapons[WEAPONS_NB];
extern enum item_id damage_weapons[DAMAGE_WEAPONS_NB];
extern enum item_id guns[GUNS_NB];
extern enum item_id ressource_items[RESSOURCES_NB];
extern enum item_id *items_groups[ITEMS_GROUPS_NB];
extern size_t items_groups_lens[ITEMS_GROUPS_NB];

enum item_id get_random_item_with_prob();
enum item_id get_random_item(enum item_id *group, size_t group_len);
void groupscat();
int restore_allitems();
void replace_item_allitems(uint32_t loc, uint32_t id);
int is_a_valid_item_id(uint32_t id);

#endif // ALLITEMS_H
