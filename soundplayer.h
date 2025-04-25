#pragma once

#include "intermediary.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <thread>
#include <chrono>

// extern std::string MicrophoneOutput;
// extern std::string PlaybackOutput;


extern bool Done;

void prepPlayers();

std::vector<std::string> getOutputDevices();

void playSound(const char *AudioClipPath);
