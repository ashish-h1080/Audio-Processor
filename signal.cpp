#define DR_WAV_IMPLEMENTATION

#include "dr_wav.h"
#include "signal_complex.h"
#include "signal_generator.h"
#include "signal_controller.h"

#define DSR 44100

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

void writeCsv (const std::vector<float> &samples, int channels, int sampleRate, int totalPCMFrameCount) {
    std::ofstream csv("waveform.csv");
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

void exportWav(std::vector <float> &baseline) {

    drwav_data_format format;
    format.container     = drwav_container_riff;
    format.format        = DR_WAVE_FORMAT_IEEE_FLOAT;  
    format.channels      = 1;                         
    format.sampleRate    = 44100;
    format.bitsPerSample = 32;                  
    drwav out;
    if (!drwav_init_file_write(&out, "baseline.wav", &format, nullptr)) {
        std::cout << "Failed to create baseline.wav\n";
        return;
    }
    drwav_uint64 framesWritten = drwav_write_pcm_frames(&out, baseline.size(), baseline.data());
    drwav_uninit(&out);

}

std::vector<float> importWav(const std::string& filename) {
    drwav wav;
    if (!drwav_init_file(&wav, filename.c_str(), nullptr)) {
        std::cerr << "Failed to open WAV file: " << filename << std::endl;
        return {};
    }

    std::cout << "FILE INFO\n";
    std::cout << "Channels: " << wav.channels << "\n";
    std::cout << "Sample Rate: " << wav.sampleRate << " Hz\n";
    std::cout << "Bits per Sample: " << wav.bitsPerSample << "\n";
    std::cout << "Total PCM Frame Count: " << wav.totalPCMFrameCount << "\n";

    int channels = wav.channels;
    int totalFrames = wav.totalPCMFrameCount;

    std::vector<float> samples(totalFrames * channels);
    drwav_read_pcm_frames_f32(&wav, totalFrames, samples.data());
    drwav_uninit(&wav);

    std::vector<float> monoSamples(totalFrames);

    if (channels == 1) {
       
        monoSamples = samples;
    }
    else {
        
        for (int i = 0; i < totalFrames; ++i) {
            float sum = 0.0;
            for (int c = 0; c < channels; ++c) {
                sum += samples[i * channels + c];
            }
            monoSamples[i] = sum / channels;
        }
    }

    return monoSamples;
}


int main() {

    //char* filename = "hello.wav";
    //std::vector<float> baseline = importWav(filename);

    std::vector <float> baseline = signalSquare(1,10,2,DSR);
    exportWav(baseline);
    writeCsv(baseline,1,44100,baseline.size());

    int ret = std::system("waveform.py");
  
    return 0;
}