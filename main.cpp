// #include "soundplayer.h"
#include "intermediary.h"
// #include "gui.h"
#include "main-window.hpp"
#include "midi-handler.h"
// #include "rtmidi/RtMidi.h"

#include <iostream>
#include <cstdlib>
#include <map>
// #include <signal.h>
// #include <vector>

extern std::map<std::string, int> SoundBindings;
extern std::map<int, std::string> KeyMap;


// RtMidiIn *MidiIn;
// std::vector<unsigned char> Message;
// int Bytes, i;
// const char *SoundEffect;
// int PortCount;

bool Done;
static void finish(int ignore)
{
	Done = true;
}

int main()
{
	std::cout << "Starting Soundboard...\n";
	if (initializeApplication() == -1)
	{
		std::cout << "Soundboard Failed To Find or Create Required Directories!" << '\n';
		std::cout << "Please Check Application And Directory Priviliges!" << '\n';
		return -1;	/* In the case that directory for saved data
					   couldn't be found or created close application. */
	}
	// std::jthread Window(createWindow);	// Opens a window for the application.
	std::jthread MidiHandler(startMidiReader);

	// Start application
	SoundboardGui GuiWindow;
	GuiWindow.Run();

	std::cout << "Window Closed\n";
	return 0;

	// Declare some midi input variables.
	// MidiIn = new RtMidiIn();
	// std::vector<unsigned char> Message;
	// int Bytes, i;

	// std::map<int, const char> SoundPath;
	// const char *SoundEffect;

	// Midi input checking
	// PortCount = MidiIn->getPortCount();
	// if (PortCount == 0)
	// {
	// 	std::cout << "No Active Midi Inputs Found!\n";
	//
	// 	delete MidiIn;
	// 	return 1;	// General fail
	// }
	//
	// for (i = 0; i < PortCount; i++)
	// {
	// 	std::cout << "Found Midi Input: " << MidiIn->getPortName(i) << std::endl;
	// }
	//
	// // Open port for input reading.
	// MidiIn->openPort(Configs->MidiPort);
	// std::cout << "Reading Input from MIDI Port: " << Configs->MidiPort << '\n';
	// MidiIn->ignoreTypes(false, false, false);
	//
	// Done = false;
	// (void) signal(SIGINT, finish);

	// Read midi input.
	// while (!Done)
	// {
	// 	// Play on loop for debugging
	// 	// SoundEffect = KeyMap[0].c_str();
	// 	// SoundThread1 = std::jthread{playSound, SoundEffect};
	// 	// std::this_thread::sleep_for(std::chrono::milliseconds(500));
	// 	// std::jthread SoundInstace(playSound, SoundEffect);
	//
	// 	MidiIn->getMessage(&Message);
	// 	Bytes = Message.size();
	//
	// 	// Check if key is mapped (WIP)
	// 	if (Bytes < 3) continue;
	// 	if ((int)Message[2] == 0) continue;
	//
	// 	// Get path of audio file and check validity
	// 	if (!KeyMap.contains((int)Message[1])) continue;
	// 	SoundEffect = KeyMap[(int)Message[1]].c_str();
	// 	if (SoundEffect == 0) continue;
	//
	// 	// Play sound on another thread to enable multiple simultaneous sounds
	// 	const char *Output = (char *)"VirtualMic";
	//
	// 	// SoundThread1 = std::jthread{playSound, SoundEffect};
	// 	std::jthread SoundInstance(playSound, SoundEffect);
	// 	SoundInstance.detach();
	//
	// 	std::cout << "Played sound\n";
	// }

	// Close application.
	// delete MidiIn;
}
