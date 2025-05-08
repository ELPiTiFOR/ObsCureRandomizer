# Intro
This documentation is not perfect, it will be upgraded and updated little by
little.

## Warning
This program modifies your game files. If something goes wrong, a file could go
corrupted and the game could stop working. I'm not responsible for the state of
your game files, you should maybe make some backup of the files just in case.
Specifically, you should make backups for all of the
`$OBS_PATH\data\_levels\x\xxxx_n.tm` files and the
`$OBS_PATH\data\_common\allitems.it` file, which are the only files this
program modifies.

You can also make backups with `ObsCureRandomizer.exe`, for more information
about making backups, read the section "How to use".

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

# Usage
## What this tool can do
- This tool can make backups of the `allitems.it` file and the `_n.tm` files,
which are the files it modifies.
- It can restore the files using a backup.
- It can randomize all the rooms, even though not all items are yet supported
(It's just a matter of time that I add them). And I still don't manage the items
that only spawn after some event correctly. The randomization can be done in 3
difficulties (which are not related to the difficulty of the game), each one
has different probabilities for an item/weapon to spawn.

## How to use the tool
This tool can only be used through a CLI, so first of all open `cmd` in Windows
and go to the directory where the executable is located with the command `cd`.
For example, if `ObsCureRandomizer.exe` is at
`C:\User\Downloads\ObsCureRandomizer.exe`, run:\
`cd C:\User\Downloads`\
To invoke the executable, run:
`.\ObsCureRandomizer.exe [<params>]`\
Where `[<params>]` is the list of arguments passed to the program.

For each feature of the program, you need to add an option, and sometimes a
parameter after that. For example, if you want to select a specific room:\
`.\ObsCureRandomizer.exe --room b008`.

Here's a list of the options:
- `--help`: show some info about the tool
- `--room <room_id>`: select a specific room with its ID. This doesn't do
anything on its own, you should put it just before one of the two following
options:
- `--randomize`: randomizes the selected room if one has been specified, or
the whole game otherwise
- `--restore`: restores the selected room if one has been specified, or the
whole game otherwise. To restore one or more rooms, you need to specify a
path for the backups and a name for the backup to restore the files from.
- `--path <game_path>`: specify the path to the game
- `--backup-path <backup_path>`: specify the path to the where the backups
should be saved
- `--backup-name <backup_name>`: specify the name of the backup, so the backup
will be saved at `<backup_path>/<backup_name>/`
- `--backup`: perform the backup according to the given `<backup_path>` and
`<backup_name>`
- `--log-path <log_path>`: specify the path to the file where the logs should be
saved.
- `--exec-script [<script_path>]`: execute the given script instead of executing
the commands written on `cmd`. You write the script the same way you would
write commands to the `cmd`, except that words are delimited by semi colons `;`
and CRLF `\r\n` instead of spaces, and you don't write the name of the program
`.\ObsCureRandomizer.exe`. If no script path is given, `script.ocr` will be read
(in the same directory as the executable).
- `--item <item_location>`: specify the location of an item to replace it with
another one (read the info on the `--replace` option). You have to specify a
room ID first.
- `--replace <item_id>`: specify the id of the item for which the item at
`<item_location>` should be replaced. You have to specify an item location
first.
- `--delete <item_location>`: delete the item at location `<item_location>`. You
have to specify a room first.

To avoid specifying the paths each time, you can use a `OCR.config` file in the
same directory as the executable file. Its syntax must be the following. Each
line of the `OCR.config` file must follow this regex: `[^=\r\n]*=[^=\r\n]*`.
In other words, a string without any equal sign nor CRLF, followed by an equal
sign, followed by another string without any equal sign nor CRLF. Example: see
section "Quickstart".

The `difficulty` must be "EASY", "NORMAL" or "HARD".
The `log_level` must be "LOG_VERY_MINOR", "LOG_MINOR", "LOG_APP_CMD",
"WARN_LOW", "WARN_HIGH", "ERROR" or "INFO".
The `reset_log` specifies whether the log file should be reset with each
execution of `ObsCureRandomizer.exe` or not. If set to "false", the log file
might become pretty large pretty quick.

## Quickstart
Follow this little quickstart guide to get started in only a couple of minutes.
Download the `ObsCureRandomizer.exe`, move it to whatever directory you like. In
the same folder, create an `OCR.config` file with the following content:
```
game-path=C:\some\path\Obscure
backup-path=E:\some\other\path with spaces\ObsCureRandomizer
backup-name=Original
log-path=E:\some\other path\example.log
difficulty=EASY
log_level=LOG_APP_CMD
reset_log=true
```
Change `C:\some\path\Obscure` to the path where your ObsCure game is installed.
If you don't know where it is, you can easily get to the installation folder
through Steam. Change `E:\some\other\path with spaces\ObsCureRandomizer` with
the path where you want your backups to be stored. You can delete the
`log-path=E:\some\other path\example.log` line, the log file will then be
`OCR.log` and it will be created in the same folder as the tool.

Once you've done this, open a `cmd` or `powershell` window (you can press the
Windows key and search "cmd" or "powershell" on the searchbar on Windows 10, I'm
not sure how it works on Windows 11), write the following command, and then
press Enter:
```
.\ObsCureRandomizer --backup
```
This will create a backup of your `_n.tm` and `allitems.it` files at the
`backup-path` location, inside an `Original` folder (which is the `backup-name`
in the `OCR.config` file). If you haven't modified the game files yet, then you
would just have created a backup of the original files of the game. This way,
you can go back to the original state of the files, and all the items will be
placed in their original position as intended by the developers of the game. If
you want to make sure to not lose the content of this backup, you can make
another backup with another name (for example, "Original 2") if you feel unsafe.
But unless you execute a `--backup` command on your "Original" backup while the
game files are in a modified state, you shouldn't worry about losing your
backup. So, each time you make a backup, be sure to not overwrite another backup
and make sure you're saving the items position you think you're saving! The tool
won't ask you if you're sure you want to perform an action, so think before
executing a command.

Now that we've made a backup, we can randomize the game without losing the
original state of the game files. Execute the following command:
```
.\ObsCureRandomizer --randomize
```
This will randomize the whole game. You can now launch the game and check some
items to see that they've indeed been randomized. You can actually use the tool
while the game is open, but the new changes will only take effect once the game
loads a new room or reloads the current one. For example, when loading a save
game, or when moving to another room.

If you want to save the current state of the items, you can make a new backup.
To do that, change the line:
```
backup-name=Original
```
With (change "Your New Backup Name" with whatever name you want to give it):
```
backup-name=Your New Backup Name
```
And then execute the command we wrote earlier to make the first backup:
```
.\ObsCureRandomizer --backup
```

If you want to go back to one of your backups, you just need to set the name of
the backup you want to go back to in your `OCR.config` file and execute the
`--restore` command. For example, if we have an "Original" and a "Your New
Backup Name" backups and we want to restore the game to the "Original" state,
you have to set the `backup-name` just like we did earlier:
```
backup-name=Original
```
And then execute the command:
```
.\ObsCureRandomizer --restore
```

You've just learned how to make a backup, how to randomize the game, and how
to restore it using a backup, congratulations! Now you can learn more about the
tool reading the rest of the README.

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

The third integer's role is unknown to me.\
The third integer's role depends on the ID of the item. If it's a statuette (with
ID 0x0191), it represents which of the 4 statuettes it is. It can then have the
values 0x01, 0x02, 0x03 and 0x04.
0x00: A statuette
0x01: A statuette

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

4 bytes: The ID of this section. For items, it's 0x00000008\
4 bytes: This field tells hoy many bytes takes the current item\
         (including these 4 bytes)
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

- b008: Cafeteria
- e103: Infirmary
- b100: Attic
- and much more