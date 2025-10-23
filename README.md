# QUAKE 4 MOD

### Build instructions

Create .env file in the directory
```
MODNAME="name"
QUAKEDIR="path/to/quake"
BUILDDIR="path/to/build"
```
- MODNAME = name of the mod, doesn't really matter
- QUAKEDIR = path to where the quake 4 executable is
- BUILDDIR = where the game dll is stored after it is done building

Doesn't work if there are spaces in any of the fields

Alternatively, run `make manual` to create a folder in this directory

  ### RUN GAME

  Assuming the .env is made correctly, type ./run in the terminal to run the mod.

  ### HOW TO PLAY

  Enemies spawn in endless waves.
  Kill them for Strogg Hearts, which you can use to upgrade your player.
  E - toggle stats viewer
  F1 - upgrade damage
  F2 - upgrade speed
  F3 - upgrade max health
  F4 - upgrade health regen
  F5 - upgrade clip size

  Game ends when you die
  Boss spawns every 10 waves
