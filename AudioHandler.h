#pragma once
#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <libsndfile/sndfile.hh>
#include <thread>
#include <vector>
#include <mutex>

enum sound {mainTheme, shoot, dash};

struct AudioFile {
	SNDFILE* file = nullptr;
	SF_INFO  info;
	int      buffer_size = 512;
	int      readHead = 0;
	sf_count_t count = 1;
};

/*
 Class for handling basic audio functions
*/
class Audio {
protected:
public:
	/// Constructor
	Audio();
	/// Destructor
	~Audio();
	/// Load an audio file
	static AudioFile loadFile(const char* path);
	/// Play an audio file
	static void PlayFile(AudioFile file, int threadIndex);

	static void Play(sound sound);
private:
	static std::vector<std::thread*> audioThreads;
	static std::vector<AudioFile> sounds;
	static std::mutex completionsMutex;
	static std::vector<bool> completions;
};
