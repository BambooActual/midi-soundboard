#include "audioloader.h"
#include <filesystem>
#include <iostream>

std::string ContentDirectory, SoundDirectory;
std::string ConfigPath, MidiMapPath;


std::vector<std::string> getSoundNames()
{
	std::vector<std::string> ListOfSounds;

	for (const auto & Entry : std::filesystem::directory_iterator(SoundDirectory))
	{
		// std::cout << Entry.path().filename() << '\n';

		auto pos = ListOfSounds.end();
		ListOfSounds.insert(pos, Entry.path().filename());
	}

	return ListOfSounds;
}

std::vector<std::filesystem::path> getSoundPaths()
{
	std::vector<std::filesystem::path> ListOfSounds;

	for (const auto & Entry : std::filesystem::directory_iterator(SoundDirectory))
	{
		std::cout << Entry.path().filename() << std::endl;

		auto pos = ListOfSounds.end();
		ListOfSounds.insert(pos, Entry.path());
	}

	return ListOfSounds;
}

void validateSounds()
{

}
