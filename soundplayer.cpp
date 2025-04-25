#include "soundplayer.h"
#include <iostream>
#include <vlc/libvlc_media_player.h>

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <vlc/vlc.h>


	libvlc_instance_t *Instance;
	libvlc_media_player_t *MediaPlayerMic;
	libvlc_media_player_t *MediaPlayerMonitor;
	libvlc_media_t *Media;

	int ActivePlayers;
	int MaxPlayers = 3;

void setOutputDevice(libvlc_media_player_t *MediaPlayer, char *OutputDevice)
{
	libvlc_audio_output_device_set(MediaPlayer, NULL, OutputDevice);
	libvlc_audio_set_volume(MediaPlayer, 85);
}

void prepPlayers()
{
	// libvlc_instance_t *Instance;
	// libvlc_media_player_t *MediaPlayerMic;
	// libvlc_media_player_t *MediaPlayerMonitor;
	// libvlc_media_t *Media;

	Instance = libvlc_new(0, NULL);
	MediaPlayerMic = libvlc_media_player_new(Instance);
	MediaPlayerMonitor = libvlc_media_player_new(Instance);
}

std::vector<std::string> getOutputDevices()
{
	// libvlc_instance_t *Instance;
	// libvlc_media_player_t *MediaPlayerMic;
	// libvlc_media_player_t *MediaPlayerMonitor;

	std::vector<std::string> OutputDevices;
	libvlc_audio_output_device_t *OutputDevice;
	int OutputDeviceCount;
	OutputDeviceCount = 1;

	Instance = libvlc_new(0, NULL);
	MediaPlayerMic = libvlc_media_player_new(Instance);
	OutputDevice = libvlc_audio_output_device_enum(MediaPlayerMic);

	OutputDevice = libvlc_audio_output_device_enum(MediaPlayerMic);
	while (OutputDevice != NULL)
	{
		OutputDevices.insert(OutputDevices.end(), OutputDevice->psz_device);
		OutputDevice = OutputDevice->p_next;
	}

	return OutputDevices;

}

void playSound(const char *AudioClipPath)
{
	// libvlc_instance_t *Instance;
	// libvlc_media_player_t *MediaPlayerMic;
	// libvlc_media_player_t *MediaPlayerMonitor;
	// libvlc_media_t *Media;

	// ActivePlayers++;
	// std::cout << ActivePlayers << '\n';
	//
	if (libvlc_media_player_is_playing(MediaPlayerMic))
	{
		libvlc_media_player_stop(MediaPlayerMic);
		std::cout << "Stopped Media Player!\n";
	}
	while (libvlc_media_player_is_playing(MediaPlayerMic)) continue;

	// Create vlc player and playable instance
	Instance = libvlc_new(0, NULL);
	Media = libvlc_media_new_path(Instance, AudioClipPath);
	MediaPlayerMic = libvlc_media_player_new_from_media(Media);
	MediaPlayerMonitor = libvlc_media_player_new_from_media(Media);
	// libvlc_media_player_set_media(MediaPlayerMic, Media);
	// libvlc_media_player_set_media(MediaPlayerMonitor, Media);

	// Create window if applicable
	#if 0
		libvlc_media_player_set_xwindow(MediaPlayerMic, xid);
		libvlc_media_player_set_xwindow(MediaPlayerMonitor, xid);
	#endif

	// Set output device
	if (Configs->MicrophoneOutput == "" && Configs->PlaybackOutput == "") 
	{
		std::cout << "Failed To Play Sound " << AudioClipPath << "!\n";
		std::cout << "No Device To Playback On!\n";
		return;
	}
	if (!libvlc_media_player_is_playing(MediaPlayerMic))
	{
		setOutputDevice(MediaPlayerMic, (char*)Configs->MicrophoneOutput.c_str());
		// std::cout << Configs->MicrophoneOutput << '\n';
	}
	if (!libvlc_media_player_is_playing(MediaPlayerMonitor))
		setOutputDevice(MediaPlayerMonitor, (char*)Configs->PlaybackOutput.c_str());

	std::string SoundName = AudioClipPath;
	SoundName = SoundName.substr(SoundName.find_last_of('/'));
	std::cout << "Playing sound " << SoundName << '\n';

	// Start playing media and wait until media player has loaded
	if (Configs->MicrophoneOutput != "")
		libvlc_media_player_play(MediaPlayerMic);
	if (Configs->PlaybackOutput != "")
		libvlc_media_player_play(MediaPlayerMonitor);
	while (libvlc_media_get_state(Media) != libvlc_Playing) {continue;}

	// Wait until media has played
	// std::this_thread::sleep_for(std::chrono::milliseconds(libvlc_media_get_duration(Media)));
	while (libvlc_media_player_is_playing(MediaPlayerMic)) continue;

	// Remove media player
	// if (Configs->MicrophoneOutput != "")
	// libvlc_media_player_release(MediaPlayerMic);
	// libvlc_media_player_release(MediaPlayerMonitor);
	// libvlc_release(Instance);
	std::cout << "Ended Playback\n";

	// ActivePlayers--;
}
