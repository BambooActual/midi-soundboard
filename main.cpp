#include "soundplayer.h"
#include "audioloader.h"
#include "intermediary.h"
#include "gui.h"

#include <iostream>
#include <cstdlib>
#include <map>
#include <signal.h>
#include <vector>
#include "rtmidi/RtMidi.h"


bool Done;
static void finish(int ignore)
{
	Done = true;
}

int main()
{
	if (initializeApplication() == -1)
	{
		std::cout << "Soundboard Failed To Find or Create Required Directories!" << '\n';
		std::cout << "Please Check Application And Directory Priviliges!" << '\n';
		return -1;	/* In the case that directory for saved data
					   couldn't be found or created close application. */
	}
	// std::jthread Window(createWindow);	// Opens a window for the application.
	// std::thread SoundThread;
	// std::thread SoundThread(prepPlayers);


	// Declare some midi input variables.
	RtMidiIn *MidiIn = new RtMidiIn();
	std::vector<unsigned char> Message;
	int Bytes, i;

	std::map<int, const char> SoundPath;
	const char *SoundEffect;

	// Midi input checking
	int PortCount = MidiIn->getPortCount();
	if (PortCount == 0)
	{
		std::cout << "No Ports!\n";

		delete MidiIn;
		return 1;	// General fail
	}
	
	for (i = 0; i < PortCount; i++)
	{
		std::cout << MidiIn->getPortName(i) << std::endl;
	}

	// Open port for input reading.
	MidiIn->openPort(1);
	MidiIn->ignoreTypes(false, false, false);

	Done = false;
	(void) signal(SIGINT, finish);

	// Read midi input.
	while (!Done)
	{
		MidiIn->getMessage(&Message);
		Bytes = Message.size();

		// Check if key is mapped (WIP)
		if (Bytes < 3) continue;
		if ((int)Message[2] == 0) continue;

		// Get path of audio file and check validity
		if (!KeyMap.contains((int)Message[1])) continue;
		SoundEffect = KeyMap[(int)Message[1]].c_str();
		if (SoundEffect == 0) continue;

		// Play sound on another thread to enable multiple simultaneous sounds
		const char *Output = (char *)"VirtualMic";

		std::jthread SoundInstace(playSound, SoundEffect);
		SoundInstace.detach();

		// std::thread SoundThread(playSound, SoundEffect);
		std::cout << "Played sound\n";

		// std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	// Close application.
	delete MidiIn;
	return 0;
}
