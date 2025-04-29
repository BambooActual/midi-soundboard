#include "midi-handler.h"
// #include "rtmidi/RtMidi.h"


// std::map<int, const char> SoundPath;

void setUpMidiHandler()
{
	// Midi input checking
	PortCount = MidiIn->getPortCount();
	if (PortCount == 0)
	{
		std::cout << "No Active Midi Inputs Found!\n";

		delete MidiIn;
		return;	// General fail
	}
	
	for (i = 0; i < PortCount; i++)
	{
		std::cout << "Found Midi Input: " << MidiIn->getPortName(i) << std::endl;
	}

	// Open port for input reading.
	MidiIn->openPort(Configs->MidiPort);
	std::cout << "Reading Input from MIDI Port: " << Configs->MidiPort << '\n';
	MidiIn->ignoreTypes(false, false, false);

	Done = false;
	// (void) signal(SIGINT, finish);
}

int awaitInput()
{
	// setUpMidiHandler();

	// Read midi input.
	while (!Done)
	{
		MidiIn->getMessage(&Message);
		Bytes = Message.size();

		// Check if key is mapped (WIP)
		if (Bytes < 3) continue;
		if ((int)Message[2] == 0) continue;

		int KeyPressed = (int)Message[1];

		return KeyPressed;
	}
	return -1;
}
