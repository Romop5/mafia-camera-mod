# AngeloFramework
A multi-purpose developer framework for creating run-time modifications of Mafia:The City of Lost Heaven.

## Features
- hooking game internals after injection
- providing access over game's input and render routines
- providign convenient utils (e.g. adaptation of ImGUI)

## Architecture
The project is split into 3 major modules:
- game - defines accessors to structures & methods which are part of the original game. Should have minimal dependencies to be easily taken out of project.
- core - defines functionality which resembles a framework - callbacks, handling of input and rendering, etc.
- utils - contains independent util clasesses & methods