# Intro
This documentation is not perfect, it will be upgraded and updated little by
little.

## Warning
This program modifies your game files. If something goes wrong, a file could go
corrupted and the game could stop working. I'm not responsible for the state of
your game files, you should maybe make some backup of the files just in case.
Specifically, you should make backups for all of the
`$OBS_PATH\\data\\_levels\\x\\xxxx_n.tm` files and the
`$OBS_PATH\\data\\_common\allitems.it` file, which are the only files this
program modifies.

## Goal of the project
The goal of this project is to provide a way to randomize the videogame ObsCure
by Hydravision Entertainment. These are the different goals that could be
achieved:\
[ ] Items that already exist can be randomized\
[ ] Monsters that already exist can be randomized\
[ ] More items can be placed appart from the ones that already exist\
[ ] More monsters can be placed appart from the ones that already exist\

## Version used
The version that I'm using to develop and test this tool is the Steam version,
developed by Mighty Rocket Studio. The patch used is the latest one as of
April 10, 2025.

## Some definitions
When I'll talk about paths, "$OBS_PATH" will represent the absolute path to the
folder where the game is installed.

## Files used
The game files that I'll be talking about are the ones that can be extracted
from all the `.hvp` files. To extract those files, check for some tool that can
do that, there are several. I'll be considering you have a folder called `data`
in $OBS_PATH, that means that you have a `data` folder at the same level as the
executable of the game, `Obscure.exe`. I'll also be considering that you have
all your folders called `_common`, `_kine`, `_levels`, [...] inside `data`.
Those folders are the ones that can be extracted from the `.hvp` files.


# Items
## How it works
### allitems.it
The game has a list of all items that can be picked up in
`$OBS_PATH/data/_common/allitems.it`. The structure of this file is the
following:
Each item has 5 integer fields, each one of them takes 4 bytes, so each item
takes 20 bytes. The `.it` file is just a list of items. All the data is in
big-endian (most significant byte first).

The first integer (that means, the first 4 bytes) gives us the ID of the item.
The ID is really small, so the first 2 bytes will always be 0. The other 2 bytes
are the ID. But, the 3rd byte also tells us the type of item it is. If the 3rd
byte is 0x00, the item is a weapon. If it is 0x02 instead, it is a ressource
item (a non unique item that is stored in the items inventory), like an energy
drink, a first aid kit, or a disc. I believe 0x01 items are key items that are
used for puzzles or advancing in the story, like keys, statuettes, levers, etc.
The list of all items can be found in Item List in this documentation.

The second integer (the next 4 bytes) gives us the location of the item (not
the same as the position).

The third integer's role is unknown to me.

The fourth integer is a multiplier, it is used for ammo (might work on some other
items too). If set to 1, pistol ammo boxes will give 15 bullets and shotgun ammo
boxes will give 7 bullets. If set to X, those 15 and 7 bullets will be
multiplied by X. That value, X, must be the same as another value that is
present in the `_n.tm` files, we'll see that later.

The fifth integer tells us in what difficulties the item can be picked up. The
first 3 bytes are always 0x00. The fourth byte can be any number from 0x01
to 0x08, although most of the time you'll see 0x07 and 0x03. The fourth byte
must be considered in its binary form: 0x07 is 0b0111, 0x03 is 0b0011, 0x08 is
0b1111, etc. In fact, each bit represents a difficulty or mode. Starting from
the right, the first bit represents Easy difficulty, the next one is the Normal
difficulty, next the Hard one, and finally the Special Mode.

So, if the value of the last integer field of an item is 0b0111 (0x07) it means
the item is present in all difficulties (regardless of the Mode). If it's
0b0100 (0x4), it means it's only present in Hard difficulty. If it's 0b0011
(0x03) it means it's present in both Easy and Normal difficulty. If it's 0b1111
(0x08) it means it's present in all difficulties, but only in Special Mode (like
the Gun with the laser, for example).

# _level
Each level has the information of what items can be picked up in it. This
information can be found in `$OBS_PATH/data/_levels/*/*/*_n.tm`.
For example, `$OBS_PATH/data/_levels/b/b008/b008_n.tm` stores the information
of the items that can be picked in the Cafeteria room (the one where you start
the game after Kenny's Prologue). These files store some other information too,
like the camera angle of the room, but we're not diving into that. So "b008"
represents the Cafeteria room, let's say it's its ID. A list of all rooms with
their ID can be found in the Room list section.

For now, I don't know the whole structure of these `_n.tm` files, but here's what
I've been able to understand: At the end of these files lie all the information
of the pickable items in the room. This data must go accordingly to the data in
the `allitems.it` file. Each item has this structure:

4 bytes: Unknown to me, most of the time it's just 0x00000008\
4 bytes: Unknown to me, most of the time it's just 0x00000045\
4 bytes: The ID of the item, for example 0x000000CA for the yellow baseball bat\
4 bytes: The location of the item, must be the same as in the `allitems.it` file\
4 bytes: The position of the item alon the X axis.\
4 bytes: The position of the item alon the Y axis.\
4 bytes: The position of the item alon the Z axis.\
4 bytes: The rotation of the item alon the X axis (probably).\
4 bytes: The rotation of the item alon the Y axis (probably).\
4 bytes: The rotation of the item alon the Z axis (probably).\
4 bytes: Unknown to me.\
4 bytes: Unknown to me.\
4 bytes: Unknown to me.\
4 bytes: Unknown to me.\
4 bytes: Unknown to me.\
4 bytes: Unknown to me.\
4 bytes: This field seems to tell how many bytes are left in the item structure,
         Let's say its value is K\
K bytes: This field gives us some information on the difficulties in which the
         item can be found, the multiplier, and some other info that is unknown
         to me. This field is an array of characters that shows a series of
         numbers separated by slashes ('/'). For example: "1/1/7", which in hex
         is: 0x31 0x2F 0x31 0x2F 0x37 (in this case K = 5). The last number
         represents the value of the last integer of the item in the
         `allitems.it` file. In this case, "7" comes from 0x00000007 which means
         the item is pickable in all difficulties, no matter the mode (special/
         normal). The first number is the multiplier, it must be the same as in
         the `allitems.it` file. If in the `.it` file the integer is 0x00000001,
         then the first number in this field must be "1" (0x31). If we want to
         multiply the ammo by 10, then we can write "10/1/7" for example
         (in hex: 0x31 0x30 0x2F 0x31 0x2F 0x37), but in the `.it` file the
         integer must be 0x0000000A, and the previous field should be updated
         to tell that this one occupies 6 bytes and not 5 anymore. The role of
         the 2nd number is unknown to me.
         In some cases, this field will just be "1" (0x31), in which case the
         previous field has the value 0x00000001, since this last field is
         1 byte long (that's my guess).


# Item list
This is the list of all items in the `allitems.it` sorted by ID. This list will
keep growing as the project goes on.
All item's IDs start with 0x0000 (first two bytes, check Items > How it works
section to understand this).

Weapons:
They all start with 0x00
- 0xC9: Bad quality flashlight
- 0xCA: Yellow baseball bat
- 0xCB: Old pistol
- 0xCC: Laser
- 0xCE: Shotgun
- 0xD1: Metal bar
- 0xD2: Light grenade
- 0xD3: Gun with flashlight
- 0xD4: Empty space
- 0xD5: Yellow good quality flashlight
- 0xD6: Powerful flashlight
- 0xD7: Automatic pistol
- 0xD8: Revolver
- 0xD9: Double barrel shotgun
- 0xDA: Aluminium bat
- 0xDC: Gun with laser

Ressource Items:
They all start with 0x02
- 0x5F: First aid kit
- 0x5A: Disc
- 0x64: Energy drink
- 0x59: Generic key?
- 0x5B: Sticky tape
- 0x5C: Pliers
- 0x5E: Wheel
- 0x60: Film 24
- 0x61: Surveillance VHS
- 0x63: Reel of wire

Key Items:
They all start with 0x01
- 0x91: Statuette (Arms along the body, hands on the neck)
- 0x9C: Lever
- 0x92: The safe?

# Room list
Here's a list of all rooms sorted by their ID. The list is incomplete as of
today, it will keep growing as the project advances.

## Randomizable:
- b008: Cafeteria
- e103: Infirmary

## Not yet randomizable:
- b100: Attic
- and much more