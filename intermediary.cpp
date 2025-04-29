#include "intermediary.h"
#include "soundplayer.h"
#include <cstdio>
// #include "audioloader.h"
// #include "soundplayer.h"
// #include <filesystem>
// #include <map>
// #include <string>

#define GET_VARIABLE_NAME(Variable)(#Variable)

// std::string MicrophoneOutput;
// std::string PlaybackOutput;
// int MuteSoundboard;
// int PlayLastSound;
std::ofstream ConfigFile;

std::map<int, std::string> KeyMap;
std::map<std::string, int> SoundBindings;

ConfigurationVariables *Configs = new ConfigurationVariables();
// #endif

void initializeConfigFile()
{
	// std::ofstream ConfigFile;
	ConfigFile.open(ConfigPath);

	if(!ConfigFile.is_open()) return;

	ConfigFile << "# Audio configuration" << '\n';
	ConfigFile <<  "MicrophoneOutput = \"\"" << '\n';
	ConfigFile <<  "PlaybackOutput   = \"\"" << '\n';

	ConfigFile << '\n';
	ConfigFile << "# Bindings" << '\n';
	ConfigFile << "MuteSoundboard   = \"\"" << '\n';
	ConfigFile << "PlayLastSound    = \"\"" << '\n';

	ConfigFile << '\n';
	// ConfigFile << "# Midi Port" << '\n';
	ConfigFile << "MidiPort   = \"\"" << '\n';

	ConfigFile.close();
	return;
}

int changeConfig(std::string ValueToChange, std::string NewValue)
{
	std::vector<std::string> FileLines = {};

	std::ifstream ConfigFileIn;
	std::string Line;
	std::string Variable;
	std::string VariableToChange;
	std::string Value;

	int i = 0;
	int LinePos = 0;
	int VariableLinePos = 0;
	// std::ofstream ConfigFile;
	ConfigFileIn.open(ConfigPath);

	while (std::getline(ConfigFileIn, Line))
	{
		if (Line.size() == 0)
		{
			FileLines.push_back(Line);
			LinePos++;
			continue;
		}
		if (Line.at(0) == '#')
		{
			FileLines.push_back(Line);
			LinePos++;
			continue;
		}
		FileLines.push_back(Line);

		// Get Variable name
		int VarStart = Line.find_first_not_of(" ");
		int VarEnd = Line.find_first_of(" ");
		Variable = Line.substr(VarStart, VarEnd - VarStart);

		// VariableLinePos = (int)ConfigFileIn.tellg();
		// std::cout << VariableLinePos << '\n';

		if (Variable == ValueToChange)
		{
			VariableLinePos = LinePos;
			ValueToChange = Variable;
		}

		LinePos++;
		i++;
	}
	ConfigFileIn.close();


	ConfigFile.open(ConfigPath);

	if(!ConfigFile.is_open()) return -1;

	for (int i = 0; i < FileLines.size(); i++)
	{
		if (i == VariableLinePos)
		{
			ConfigFile << ValueToChange << " = " << '\"' << NewValue << "\"\n";
			continue;
		}

		ConfigFile << FileLines[i] << '\n';
	}
	ConfigFile.close();

	if (ValueToChange == "MicrophoneOutput")
		Configs->MicrophoneOutput = NewValue;

	if (ValueToChange == "PlaybackOutput")
		Configs->PlaybackOutput = NewValue;

	return 0;
}

void loadConfig()
{
	std::string Line;
	std::string Variable;
	std::string Value;
	std::ifstream ConfigFileIn;

	ConfigFileIn.open(ConfigPath);

	if (!ConfigFileIn.is_open()) return;
	while (std::getline(ConfigFileIn, Line))
	{
		if (Line.size() == 0) continue;
		if (Line.at(0) == '#') continue;

		// Get Variable name
		int VarStart = Line.find_first_not_of(" ");
		int VarEnd = Line.find_first_of(" ");
		int EqualSign = Line.find_first_of("=");
		Variable = Line.substr(VarStart, VarEnd - VarStart);
		// std::cout << Variable << "\n";

		// Get Value of Variable
		int ValueStart = Line.substr(EqualSign + 2).find_first_not_of(" ") + EqualSign;
		int ValueEnd = Line.find_last_not_of(" ");
		Value = Line.substr(ValueStart + 2, ValueEnd - ValueStart + 1);
		Value = (Value.at(0) == '\"') ? Value.substr(1, Value.find_last_of('\"') - 1) : Value;
		// std::cout << Value << "\n";

		if (Variable == "MicrophoneOutput")
			Configs->MicrophoneOutput = Value;
		if (Variable == "PlaybackOutput")
			Configs->PlaybackOutput = Value;
		if (Variable == "MuteSoundboard" && Value != "")
			Configs->MuteSoundboard = std::stoi(Value);
		if (Variable == "PlayLastSound" && Value != "")
			Configs->PlayLastSound = std::stoi(Value);
		if (Variable == "MidiPort" && Value != "")
			Configs->MidiPort = std::stoi(Value);
	}
	ConfigFileIn.close();
}

void importSound()
{
	std::string ImportablePath;
	char FilePath[1024];

	FILE *File = popen("zenity --file-selection", "r");
	fgets(FilePath, 1024, File);
	ImportablePath = FilePath;
	if (ImportablePath == "")
		return;
	ImportablePath = ImportablePath.substr(0, ImportablePath.size() - 1);

	std::string FileName = FilePath;
	FileName = FileName.substr(FileName.rfind("/") + 1, -1);

	std::filesystem::path From = ImportablePath;
	std::filesystem::path To = SoundDirectory + FileName;
	std::filesystem::copy_file(From, To);

	bindSoundToKey(-1, SoundDirectory + FileName);

	std::cout << "Imported Sound: " << FileName;
}

int addMidimapping(int Key, std::string InMapping)
{
	std::string MidiMapping = std::to_string(Key) + ", " + InMapping;
	std::ofstream Midimap;


	Midimap.open(MidiMapPath, std::ios::app);

	// If file didn't open, exit with errorcode
	if (!Midimap.is_open()) return -1;

	// Write to file
	Midimap << '{' << MidiMapping << "},\n";

	Midimap.close();
	return 0;
}

void removeKeyFromMap(int KeyToRemove)
{
	std::string Line;
	std::ifstream Midimap;
	std::ofstream MidimapWrite;
	std::vector<std::string> KeepBinds;
	Midimap.open(MidiMapPath);

	while (std::getline(Midimap, Line))
	{
		int Start = Line.find_first_of('{');
		int SecondArg = Line.find_first_of(',');
		// int End = Line.find_first_of('}');

		int KeyCode = std::stoi(Line.substr(Start + 1, SecondArg - 1));
		if (KeyCode != KeyToRemove)
			KeepBinds.push_back(Line);
		// std::string SoundPath = Line.substr(SecondArg + 2, End - SecondArg - 2);
		// KeyMap.insert({KeyCode, SoundPath});

		// std::string SoundName = SoundPath.substr(SoundPath.rfind('/') + 1);
		// SoundBindings.insert({SoundName, KeyCode});
	}
	Midimap.close();

	MidimapWrite.open(MidiMapPath);

	// If file didn't open, exit with errorcode
	if (!MidimapWrite.is_open()) return;

	for (int i = 0; i < KeepBinds.size(); i++)
	{
		// Write to file
		MidimapWrite << KeepBinds[i] << '\n';
		std::cout << KeepBinds[i] << '\n';
	}

	MidimapWrite.close();
}

void bindSoundToKey(int KeyCode, std::string SoundName)
{
	if (KeyMap.contains(KeyCode))
	{
		std::cout << "Couldn't Bind To Key!";
		std::cout << "Key Already Has a Sound Bound!";
		return;
	}

	std::cout << SoundBindings.find(SoundName)->second << '\n';
	int SoundKey = SoundBindings[SoundName];
	KeyMap.erase(SoundBindings[SoundName]);
	SoundBindings.erase(SoundName);
	removeKeyFromMap(SoundKey);

	KeyMap.insert({KeyCode, SoundDirectory + SoundName});
	SoundBindings.insert({SoundName, KeyCode});
	SoundBindings[SoundName] = KeyCode;
	// std::cout << SoundBindings[SoundName] << '\n';
	addMidimapping(KeyCode, SoundDirectory + SoundName);
}

int loadMidimap()
{
	std::string Line;
	std::ifstream Midimap;
	Midimap.open(MidiMapPath);

	// If file didn't open, exit with errorcode
	if (!Midimap.is_open()) return -1;

	while (std::getline(Midimap, Line))
	{
		int Start = Line.find_first_of('{');
		int End = Line.find_first_of('}');
		int SecondArg = Line.find_first_of(',');

		int KeyCode = std::stoi(Line.substr(Start + 1, SecondArg - 1));
		std::string SoundPath = Line.substr(SecondArg + 2, End - SecondArg - 2);
		KeyMap.insert({KeyCode, SoundPath});

		std::string SoundName = SoundPath.substr(SoundPath.rfind('/') + 1);
		SoundBindings.insert({SoundName, KeyCode});
		// SoundBindings[SoundName.c_str()] = KeyCode;
		// std::cout << SoundBindings[SoundName] << '\n';
		// std::cout << SoundName << ", " << KeyCode << '\n';
	}

	Midimap.close();
	return 0;
}

int createDirectories()
{
	try
	{
		std::filesystem::create_directory(ContentDirectory);
		std::cout << "created content directory\n";
		std::filesystem::create_directory(SoundDirectory);
		std::cout << "created sounds directory\n";

		std::ofstream ConfigFile (ContentDirectory + "config.conf");
		ConfigPath = std::string(ContentDirectory + "config.conf");
		std::cout << "created config file\n";

		std::ofstream MidimapFile (ContentDirectory + "midimap.csv");
		MidiMapPath = std::string(ContentDirectory + "midimap.csv");
		std::cout << "created midimap file\n";

		initializeConfigFile();
	}
	catch (const std::exception &e)
	{
		return -1;	// Create data directory error
	}

	return 0;	// No directory was found but was created successfully
}

int initializeApplication()
{
	// std::string DataDirectory;
	// std::string MidiMapPath;
	bool ConfigExists = std::filesystem::exists(ContentDirectory);

	// prepPlayers();
	getOutputDevices();

	if (std::getenv("XDG_DATA_HOME") != NULL)
	{
		ContentDirectory = std::string(std::getenv("XDG_DATA_HOME")).append("/soundboard/");
		ConfigExists = std::filesystem::exists(ContentDirectory);
		SoundDirectory = ContentDirectory + "sounds/";
	}
	else if (std::getenv("HOME") != NULL)
	{
		ContentDirectory = std::string(std::getenv("HOME")).append("/.local/share/soundboard/");
		ConfigExists = std::filesystem::exists(ContentDirectory);
		SoundDirectory = ContentDirectory + "sounds/";
	}

	if (ConfigExists)
	{
		std::cout << "Directory Found\n";

		// loadConfig(ContentDirectory, MidiMapPath);
		SoundDirectory = ContentDirectory + "sounds/";
		MidiMapPath = std::string(ContentDirectory + "midimap.csv");
		ConfigPath = std::string(ContentDirectory + "config.conf");

		loadMidimap();
		if (std::filesystem::exists(ContentDirectory + "config.conf") 
			&& !std::filesystem::is_empty(ContentDirectory + "config.conf"))
		{
			loadConfig();
			std::cout << "Loaded Config\n";
		}
		else
		{
			std::cout << "Config was not found or was empty" << '\n';
			std::cout << "Populated config file" << '\n';
			initializeConfigFile();
		}

		// std::cout << "Directory Found\n";
		return 0;	// Config exists and was found
	}
	else
	{
		if (createDirectories() == 0)
		{
			std::cout << "Directory Created.\n";
			return 0;
		}
		else
		{
			std::cout << "Failed to create directory!\n";
			return -1;
		}
	}
}
