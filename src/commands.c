#include "commands.h"

#include <stdio.h>
#include <string.h>

#include "utils.h"

void help()
{
    printf("DESCRIPTION:\n");
    printf("\tRandomizes items in ObsCure. You can only randomize these rooms as of today (work in progress):\n");
    printf("\t- Cafeteria (b008)\n");
    printf("\t- Infirmary (e103)\n");
    printf("WARNING:\n");
    printf("\tThis program modifies your game files. If something goes wrong, the game files could go corrupted\n");
    printf("\tand the game could stop working. You are responsible for the fate of your game files. Think about\n");
    printf("\tmaking a backup of your files just in case. Specially the folder `_levels` and the `allitems.it` file\n");
    printf("\twhich are the files this programs modifies. The paths to each one of them are the ones that follow:\n");
    printf("\t            `$OBS_PATH\\data\\_levels`\n");
    printf("\t            `$OBS_PATH\\data\\_common\\allitems.it`\n");
    printf("\tFor more information on this program, refer to the README.\n");
    printf("USAGE:\n");
    printf("\t.\\ObsCureRandomizer.exe [<options>]\n");
    printf("OPTIONS:\n");
    printf("\t--help: show this help\n");
    printf("\t--room <room_id>: select a room by ID\n");
    printf("\t--path <game_path>: select the absolute path where the game is installed (mandatory)\n");
    printf("\t--randomize: if a room was selected, it randomizes it. Otherwise, it randomizes the whole game\n");
    printf("\t--restore: if a room was selected, it restores it to its original state. Otherwise, it\n");
    printf("\t           randomizes the whole game\n");
    printf("EXAMPLES:\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --room b008 --randomize\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --room b008 --restore\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --randomize\n");
    printf("\t.\\ObsCureRandomizer.exe --path C:\\SteamLibrary\\steamapps\\common\\Obscure --restore\n");
    printf("\t.\\ObsCureRandomizer.exe --help\n");
}

void path(char *path)
{
    strcpy(path_items, path);
    strcpy(path_room, path);
    strcpy(path_game, path);
    strcat(path_items, "\\data\\_common\\allitems.it");
}
