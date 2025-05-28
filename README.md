# Midi Soundboard
A simple Soundboard for linux designed to be used with a Midi input device. 
This is my first c++ project so unoptimized and barely functional code is to be expected.

## Insallation
Extract files to desired directory and navigate there. Then run the command ``` make soundboard ```.
Now you should be able to run the soundboard with ``` ./soudboard ``` in the installation directory.

## Dependencies
The Soundboard uses libvlc to play sounds, so at it's current state vlc is required for it to function.

## Current State
Currently you can import sounds and bind midi keys to them.
To play sounds through your mic manual setup is required to route sound from the soundboard to your mic.

## Planned Features
Ability to import entire directories to have an organized sound library in addition to a sounboard.
Automatic setup for a virtual output to minimize manual fiddling.
