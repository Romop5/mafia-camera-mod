# CameraMod (Mafia:The city of Lost Heaven) #

CameraMod is a fan modification for game called Mafia:The city of Lost Heaven. It is devoted to serve as a tool for movie makers and for taking screenshoots. It allows its users to use free camera mod, to create camera paths and then move camera along them, to hide game GUI and to control game speed (e.g. slow motion effect).

This mod was originally coded by Romop5, the author of Lost Heaven Multiplayer (http://lh-mp.eu) in November 2014.  

<div align=center>

<a href="http://www.youtube.com/watch?feature=player_embedded&v=cbWiwHvqdX8
" target="_blank"><img src="http://img.youtube.com/vi/cbWiwHvqdX8/0.jpg" 
alt="CameraMod preview" width="480" height="360" border="10" /></a>

</div>

## TODO
- refactor: 
    - remove inverse dependencies
    - replace current GUI with ImGUI
    - added different types of camera transients (different types of curves, etc)
    - add some nice features to alter rendering (mark PEDs/cars with specified colour, etc)

## Features

- **Run-time attachment** to Mafia process
- **Linear point-to-point **camera transitions**
- **HUD disablement**
- **Game speed control**

## Usage

Place the files into directory with Game.exe, run Game.exe and run CameraModInjector.exe, now go to mission (tutorial, freeride, etc.) and press **F1**.
Controls:
- **F2** - free cam
- **F3** - menu/GUI
- **F4** - creates a point at freecam position
- **F5** - creates a path - you need to select 2 points on screen using mouse
- **F6** - plays the first path
- **F7** - pause/run the game 

## Binaries

Currently, there is a version running from 2014. See releases.

## Compilation

### How to compile & run app ###

* Clone whole repo
* Open CameraMod.sln in Visual Studio 2013 or higher
* Compile whole project as Release
* At the end you will get CameraMod.dll and Injector.exe
* Copy them into directory with Game.exe
* Enjoy

### Why is this project open-source? ###

* This project shows how ingame modification is built, so you can learn something new
* If you feel that something is wrong or something could be even better, you can take a part in developing and suggest changes

### What licence does it belong to ? ###

This project uses beerware licence. Do whatever you want with it, but when you meet me, buy me a beer, please :)
