#define DR_WAV_IMPLEMENTATION

#include "dr_wav.h"
#include "signal_complex.h"
#include "signal_generator.h"

#include <iostream>
#include <vector>
#include <fstream>

void write_csv (const std::vector<float> &samples, int channels, int sampleRate, int totalPCMFrameCount) {
    std::ofstream csv("audio_samples.csv");
    if (!csv) {
        std::cout << "Failed to write CSV file.\n";
        return;
    }

    csv << "Time,Amplitude\n";
    
    for (int i = 0; i < totalPCMFrameCount; ++i) {
        float time = static_cast<float>(i) / static_cast<float>(sampleRate);
        float amplitude = samples[i * channels];
        csv << time << "," << amplitude << "\n";
    }

    csv.close();
}

int main() {

    const char* filename = "hello.wav";
    drwav wav;
    bool read = drwav_init_file(&wav, filename, nullptr);

    if (!read) {
        std::cout << "failed to open .wav file\n";
        return 1;
    }

    std::cout << "===============FILE INFO===============\n";
    std::cout << "Channels : "<< wav.channels << "\n";
    std::cout << "Sample Rate : " << wav.sampleRate << "Hz \n";
    std::cout << "Bits per sample " << wav.bitsPerSample << "\n";
    std::cout << "Total PCM Frame count " << wav.totalPCMFrameCount << "\n";


    // allocate buffer for audio samples
    //samples.data() is a raw pointer to the first element the allocated buffer
    //drwav_read_pcm_frames_f32 reads and normalises the values between -1 and 1 for 16 bit PCM and 0 and 1 for 8 bit unsigned PCM

    bool STEREO = (wav.channels == 2 ? 1 : 0);
    int channels = wav.channels;
    int sampleRate = wav.sampleRate;
    int bitsPerSample = wav.bitsPerSample;
    int totalPCMFrameCount = wav.totalPCMFrameCount;

    std::vector<float> samples(wav.totalPCMFrameCount * wav.channels);
    drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, samples.data());

    //take the samples from the wav files and create a complex sample vector that has real component as sample and
    //imaginary component initialized to zero

    std::vector<Complex> samplesComplex(samples.size());

    for(int i = 0; i < samples.size(); i++) {
        samplesComplex[i].real = samples[i];
        samplesComplex[i].imaginary = 0; 
    }

    std::vector<float> samplesL;
    std::vector<float> samplesR;

    //sample vectors - samples, samplesComplex, samplesL, samplesR
    //samplesL and samplesR contain Left channel and Right channel samples respectively for stereo file types

    if(STEREO) {
        
        for(int i  = 0; i < wav.totalPCMFrameCount; i++) {
            samplesL.push_back(samples[i*wav.channels + 0]);
            samplesR.push_back(samples[i*wav.channels + 1]);
        }
    }

    //TODO : split channels and create corresponding complex sample vectors
    //TODO : scale down the WAV 

    drwav_uninit(&wav);

    std::cout << "===============WAV FILE LOADED===============\n";

    std::cout << "read " << samples.size() << " total samples\n";

    if(STEREO) {
        std::cout << "read " << samplesL.size() << " L channel samples\n";
        std::cout << "read " << samplesR.size() << " R channel samples\n";
    }

    signalGain(samples,0.5);
    signalGain(samplesL,0.5);
    signalGain(samplesR,0.5);

    std::vector<float> baseline;
    baseline = signalAdd(baseline, signalSin(0.5,10,5,44100,1),44100);
    baseline = signalAdd(baseline, signalSin(1,12,3,44100,1),44100,5);

    write_csv(baseline,1,44100,baseline.size());
    
    return 0;
}
