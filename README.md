# ObsCure Randomizer
This project aims to randomize the game ObsCure. To be more specific, the
current goals to achieve are the following:
- Randomize all items
- Randomize all monsters
- Maybe randomize all doors?

This is still WIP, please be patient. It's a rewrite of the old randomizer, so
some features are still missing.

# Requirements
You need
[ObsCureFileParser v0.2.0](https://github.com/ELPiTiFOR/ObsCureFileParser/releases/tag/v0.2.0)
and
[ObsCureInfo v0.1.0](https://github.com/ELPiTiFOR/ObsCureInfo/releases/tag/v0.1.0).
Both DLLs are included in the releases of ObsCureRandomizer.

# Usage
First, you need to make an `ocr.config` file (you have an example in the source
code and in the release) with the `game_path` setting, with the following
format:
```
game_path=C:\SteamLibrary\steamapps\common\Obscure
```
Then, in order to randomize all the rooms, use the following command.
```
.\ObsCureRandomizer.exe --randomize-all
```