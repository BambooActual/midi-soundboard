#include "midi-handler.h"
// #include "rtmidi/RtMidi.h"


// std::map<int, const char> SoundPath;
RtMidiIn *MidiIn;
std::vector<unsigned char> Message;
std::vector<std::string> MidiPortNames;
int Bytes, i, PortCount;
const char *SoundEffect;


std::vector<std::string> getMidiPortNames()
{
	// PortCount = MidiIn->getPortCount();
	//
	// if (PortCount == 0)
	// {
	// 	std::cout << "No Active Midi Inputs Found!\n";
	//
	// 	delete MidiIn;
	// }
	//
	// for (i = 0; i < PortCount; i++)
	// {
	// 	MidiPortNames.push_back(MidiIn->getPortName(i).c_str());
	// }
	return MidiPortNames;
}

void openPort()
{
	MidiIn->closePort();

	// Open port for input reading.
	MidiIn->openPort(Configs->MidiPort);
	std::cout << "Reading Input from MIDI Port: " << Configs->MidiPort << '\n';
	MidiIn->ignoreTypes(false, false, false);
}

void setUpMidiHandler()
{
	MidiIn = new RtMidiIn();

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
		// std::cout << "Found Midi Input: " << MidiIn->getPortName(i) << std::endl;
		MidiPortNames.push_back(MidiIn->getPortName(i));
	}

	openPort();

	Done = false;
	// (void) signal(SIGINT, finish);
}

int awaitInput()
{
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

void startMidiReader()
{
	setUpMidiHandler();

	while (!Done)
	{
		// Play on loop for debugging
		// SoundEffect = KeyMap[0].c_str();
		// SoundThread1 = std::jthread{playSound, SoundEffect};
		// std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// std::jthread SoundInstace(playSound, SoundEffect);

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
		const char *Output = (char *)"Virtual Mic";

		// SoundThread1 = std::jthread{playSound, SoundEffect};
		std::jthread SoundInstance(playSound, SoundEffect);
		SoundInstance.detach();

		std::cout << "Played sound\n";
	}

	delete MidiIn;
}
