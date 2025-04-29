#pragma once

// #include <iostream>

// #include "soundplayer.h"
// #include "audioloader.h"
#include "intermediary.h"
#include "midi-handler.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// #include <vector>
// #include <string>
// #include <thread>
// #include <stdlib.h>
// #include <stdio.h>
#include <charconv>
#include <GLFW/glfw3.h>

extern bool Done;
extern std::map<std::string, int> SoundBindings;

int createWindow();
