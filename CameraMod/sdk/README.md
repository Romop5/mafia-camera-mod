# CameraMod's SDK

## About

This folder contains classes / code which is used to interconnect CameraMod with different games and projects.

- *CAPI.hpp* defines "C" methods which are called by core to setup game's wrapper
- *CGenericGame* class defines a set of methods that MUST be provided by game to run CameraMod

### How to implement a game wrapper for CameraMod

Have a look at reference implementations (e.g. Mafia).
A typical game wrapper contains:
- hooks for controlling game (rendering, keyboard inputs)

### Note
This folder *should be self-containing* with all files included. No includes outside should be allowed here in order to be able to copy SDK's file to other C/C++ projects. 