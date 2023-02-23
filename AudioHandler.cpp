#include "AudioHandler.h"

std::vector<std::thread*> Audio::audioThreads;
std::vector<AudioFile> Audio::sounds;
std::mutex Audio::completionsMutex;
std::vector<bool> Audio::completions;

Audio::Audio() {
    sounds.push_back(loadFile("C:/Users/ellis/source/repos/05t4r/Waiting.wav"));        // mainTheme
    sounds.push_back(loadFile("C:/Users/ellis/source/repos/05t4r/Glitch_shoot.wav"));   // shoot
    sounds.push_back(loadFile("C:/Users/ellis/source/repos/05t4r/Glitch_dash.wav"));    // dash
}

Audio::~Audio() {
    for (int i = 0; i < audioThreads.size(); ++i) 
    {
        std::thread* audioThread = audioThreads[i];
        audioThread->join();
        delete audioThread;
    }
    PaError err = Pa_Terminate();
    if (err != paNoError) std::cerr << "Termination Errror: " << err << std::endl;
}

/* Loads an audiofile */
AudioFile Audio::loadFile(const char* path) {
    AudioFile file;
    ::memset(&file.info, 0, sizeof(file.info));
    file.file = sf_open(path, SFM_READ, &file.info);
    return file;
}

static int patestCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long                   framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void* userData) {
    /// Prevent warnings
    (void)inputBuffer;
    (void)timeInfo;
    (void)statusFlags;

    /// an AudioFile gets passed as userData
    AudioFile* file = (AudioFile*)userData;
    float* out = (float*)outputBuffer;

    sf_seek(file->file, file->readHead, SF_SEEK_SET);

    auto data = std::make_unique<float[]>(framesPerBuffer * file->info.channels);
    file->count = sf_read_float(file->file, data.get(),
        framesPerBuffer * file->info.channels);

    for (int i = 0; i < framesPerBuffer * file->info.channels; i++) {
        *out++ = data[i];
    }

    file->readHead += file->buffer_size;

    if (file->count > 0) return paContinue;
    else return paComplete;
}

void Audio::Play(sound sound)
{

    completionsMutex.lock();
    int numThreads = completions.size();
    completionsMutex.unlock();

    for (int i = 0; i < numThreads; ++i)
    {
        completionsMutex.lock();
        bool complete = completions[i];
        completionsMutex.unlock();
        if (complete)
        {
            audioThreads[i]->join();
            delete audioThreads[i];

            completionsMutex.lock();
            completions[i] = false;
            completionsMutex.unlock();

            audioThreads[i] = new std::thread(PlayFile, sounds[sound], i);
            return;

        }
    }
    completionsMutex.lock();
    completions.push_back(false);
    completionsMutex.unlock();
    audioThreads.push_back(new std::thread(PlayFile, sounds[sound], numThreads));
}

void Audio::PlayFile(AudioFile file, int threadIndex) 
{
    PaError err = Pa_Initialize();
    if (err != paNoError) std::cerr << "Initialization Error: " << err << std::endl;

    PaStream* stream = nullptr;
    PaStreamParameters params;
    params.device = Pa_GetDefaultOutputDevice();
    params.channelCount = file.info.channels;
    params.sampleFormat = paFloat32;
    params.suggestedLatency =
        Pa_GetDeviceInfo(params.device)->defaultLowOutputLatency;
    params.hostApiSpecificStreamInfo = nullptr;

    /// Check if params work
    err = Pa_IsFormatSupported(nullptr, &params, 48000);// file->info.samplerate);
    if (err != paFormatIsSupported) {
        std::cerr << "PAError 2: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    err = Pa_OpenStream(&stream, nullptr, &params, 48000,//file->info.samplerate,
        file.buffer_size, paClipOff,
        &patestCallback, &file);

    if (err != paNoError) std::cerr << "PAError 3: " << Pa_GetErrorText(err) << std::endl;

    err = Pa_StartStream(stream);
    if (err != paNoError)
        std::cerr << "PAError 4: " << Pa_GetErrorText(err) << std::endl;

    /// wait until file finishes playing
    while (file.count > 0) {}

    err = Pa_StopStream(stream);
    if (err != paNoError)
        std::cerr << "PAError 5: " << Pa_GetErrorText(err) << std::endl;

    err = Pa_CloseStream(stream);
    if (err != paNoError)
        std::cerr << "PAError 6: " << Pa_GetErrorText(err) << std::endl;

    err = Pa_Terminate();
    if (err != paNoError) std::cerr << "Termination Errror: " << err << std::endl;

    completionsMutex.lock();
    completions[threadIndex] = true;
    completionsMutex.unlock();

}