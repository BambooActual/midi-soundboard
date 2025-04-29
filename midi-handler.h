#pragma once

#include "intermediary.h"
#include "rtmidi/RtMidi.h"
#include <map>

extern bool Done;
extern std::map<int, std::string> KeyMap;
extern struct ConfigurationVariables *Configs;

// extern RtMidiIn *MidiIn;
// extern std::vector<unsigned char> Message;
// extern int Bytes, i;
// extern const char *SoundEffect;
// extern int PortCount;


void startMidiReader();

int awaitInput();
