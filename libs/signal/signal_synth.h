#ifndef SIGNAL_SYNTH_H
#define SIGNAL_SYNTH_H

#include <iostream>
#include <cmath>
#include <vector>

#include "./midifile/MidiParser.h"

#define PI acos(-1)

std::vector<float> signalADSR (float time, float sampleRate, float aCurvature, float aMagnitude, float dCurvature, float dMagnitude,float rCurvature, float A, float D, float S, float R) {

    std::vector <float> signal;

    float aLength = (time*A)/(A+D+S+R);
    float dLength = (time*D)/(A+D+S+R);
    float sLength = (time*S)/(A+D+S+R);
    float rLength = (time*R)/(A+D+S+R);

    for(int i = 0; i < aLength*sampleRate; i++) {
        float k = i/sampleRate;
        signal.push_back(pow((k/aLength),aCurvature)*aMagnitude);
    }

    for(int i = 0; i < dLength*sampleRate; i++) {
        float k = i/sampleRate;
        signal.push_back((1 - pow((k/dLength),dCurvature)*dMagnitude)*aMagnitude);
    }

    for(int i = 0; i < sLength*sampleRate; i ++) {
        float k = i/sampleRate;
        signal.push_back((1-dMagnitude)*aMagnitude);
    }

    for(int i = 0; i < rLength*sampleRate; i++) {
        float k = i/sampleRate;
        signal.push_back((1- pow((k/rLength),rCurvature))*(1-dMagnitude)*aMagnitude);
    }

    return signal;

}

std::vector <float> synthAD (float sampleRate, float aLength,float aCurvature, float dLength, float dCurvature, float dMagnitude) {
    
    std::vector<float> AD;
    
    for(int i = 0; i < aLength*sampleRate; i++) {
        float k = i/sampleRate;
        AD.push_back(pow((k/aLength),aCurvature));
    }

    for(int i = 0; i < dLength*sampleRate; i++) {
        float k = i/sampleRate;
        AD.push_back((1 - pow((k/dLength),dCurvature)*dMagnitude));
    }

    return AD;

}

std::vector <float> synthR (float sampleRate, float rLength, float rMagnitude, float rCurvature) {

    std::vector <float> signal;

    for(int i = 0; i < rLength*sampleRate; i++) {
        float k = i/sampleRate;
        signal.push_back((1- pow((k/rLength),rCurvature))*(1-rMagnitude));
    }

    return signal;
}

float synthSquare(float amplitude, float frequency, float time) {
    float y = amplitude*(std::sin(2*PI*frequency*time) >= 0 ? 1 : -1);
    return y;
}

float synthSine(float amplitude, float frequency, float time) {
    float y = amplitude*std::sin(2*PI*frequency*time);
    return y;
}

float pitchFrequency (int note, float detune) {

    float N = static_cast<float>(note) + detune*0.01;
    return 440.0 * pow(2.0, (N - 69) / 12.0);

}

void synthMain(Track &T, std::vector <float> &buffer) {

    float sampleRate = 44100.0;

    std::vector <float> AD = synthAD(sampleRate, 1, 0.5, 1, 0.2, 0.3);
    std::vector <float> R  = synthR(sampleRate, 0.5, 0.3, 0.2);

    float sustain = 0.8;

    for (auto &note : T.notes) {

        int start = static_cast<int>(note.startTime * sampleRate);
        int end   = static_cast<int>(note.endTime   * sampleRate);
        int duration = end - start;

   
        std::vector<float> ads;
        if (duration <= (int)AD.size()) {
            ads.assign(AD.begin(), AD.begin() + duration);
        } 
        else {
            ads = AD;
            ads.resize(duration, sustain);
        }

        
        int length = duration + R.size();

        for (int i = 0; i < length; i++) {

            float ts = i / sampleRate;

            float value = 0.0f;
            value += synthSine(0.8f, pitchFrequency(note.pitch, 20), ts);
            value += synthSquare(0.6f, pitchFrequency(note.pitch, -20), ts);

            if (i < duration) {
                value *= ads[i];
            } 
            else {
                value *= R[i - duration];
            }

            buffer[start + i] += value;

            }

        } 

    signalNormalize(buffer);

}

#endif