#pragma once
#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <libsndfile/sndfile.hh>

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
	AudioFile loadFile(const char* path);
	/// Play an audio file
	void playFile(AudioFile* file);
};
