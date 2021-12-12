# Devolo
Tool for reading Diablo II resources and documentation of file formats.

## Roadmap

- [ ] Write readme with overview and plan for program
- [ ] Start Qt-Frontend (layout: filebrowser, display area)
- [ ] Integrate my bitstream class
- [ ] Dc6 file format
- [ ] Palette file format
- [ ] Wav file format (FFmpeg)
- [ ] Bik file format (FFmpeg)
- [ ] MPQ Packages
- [ ] Animations: cof, d2, dcc, ...
- [ ] Data files: dat, tbl, bin, txt
- [ ] Savegame: d2s (maybe the others if feasible)

## Overview of File Formats
The game files consist of graphical resources (**dc6**, **dcc**, **ds1**), sound files (**wav**), data for the visuals (**cof**, **d2**, **dat**, **dt1**, **tbl**), data for the game logic (**bin**, **txt**), and video files in the proprietary **bik** format. These resources are stored in **mpq** packages, possibly compressed.

sprites, animated sprites, tiles, text comprise the visuals
static sprites (mainly in **dc6**) e.g. for skill icons
tiles of the map are found in **ds1** files
animated sprites (characters and monsters) are found in **dcc** files (although some exceptions in **dc6**), they consist of several layers: think of displaying different weapons, different armor; and different animations for those layers, therefore they have configuration files **cof**, **d2**, **dat** (and sometimes others)

sound files are stored as **wav** compressed with IMA ADPCM (adaptive differential pulse-code modulation), which is apparently implemented by FFmpeg adpcm\_ima\_wav, but a description of the algorithm can be found here https://wiki.multimedia.cx/index.php?title=IMA_ADPCM

data for game logic consists of e.g. armor classes, drop chances, monster spawns, item names, experience levels, difficulties, etc.
most (all?) of it is stored as tabular data (csv) in **txt** files
TODO: what is in **bin** files????

video files for the cutscenes are stored in the proprietary **bik** format by the bink video company, now epic game tools. but they have been reverse engineered and can be played with ffmpeg

additionally, it should be mentioned that there are files for the character savegames with the following file formats **d2s**, **key**, **ma0**, **map**

## Detailled Descriptions of File Formats


