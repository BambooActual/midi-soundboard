#pragma once

#include "soundplayer.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>

extern std::string ContentDirectory;
extern std::string SoundDirectory;
extern std::string ConfigPath;
extern std::string MidiMapPath;

extern std::map<int, std::string> KeyMap;

// #ifndef GENERAL_H_
// #define GENERAL_H_
// template<typename Var>
struct ConfigurationVariables
{
	std::string MicrophoneOutput = "";
	std::string PlaybackOutput = "";

	int MuteSoundboard = -1;
	int PlayLastSound = -1;
};
extern struct ConfigurationVariables *Configs;
// #endif

// int getOutputDevices();

int changeConfig(std::string ValueToChange, std::string NewValue);

int initializeApplication();

void bindSoundToKey(std::string SoundName, int KeyCode);

void importSound();

int writeMidimap(std::string InMapping);

int loadMidimap();
