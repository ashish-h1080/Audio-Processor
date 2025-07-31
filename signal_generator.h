#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <iostream>
#include <cmath>
#include <vector>

#define PI acos(-1)

void signalGain (std::vector<float> &sample, float gain) {
    for(int i = 0; i < sample.size(); i++) {
        sample[i] *= gain;
    }
}

std::vector<float> signalSin(float amplitude, float frequency, float time, int sampleRate, int channels) {

    std::vector<float> signal;

    int samples = static_cast<int>(sampleRate*time);

    for(int i = 0; i < samples; i++) {

        float t = static_cast<float>(i)/sampleRate;
        float y = amplitude* std::sin(2*PI*frequency*t);

        for(int j = 0; j < channels; j++) {
            signal.push_back(y);
        }
        
    }

    return signal;
}

std::vector<float> signalCos(float amplitude, float frequency, float time, int sampleRate, int channels) {

    std::vector<float> signal;

    int samples = static_cast<int>(sampleRate*time);

    for(int i = 0; i < samples; i++) {

        float t = static_cast<float>(i)/sampleRate;
        float y = amplitude* std::cos(2*PI*frequency*t);

        for(int j = 0; j < channels; j++) {
            signal.push_back(y);
        }
        
    }

    return signal;
}


std::vector<float> signalAdd (std::vector<float>a, std::vector<float>b, int sampleRate, float time = 0) {

    int buffer = static_cast<int>(time*sampleRate);

    std::vector<float> result;
    result.resize(std::max(buffer + b.size(), a.size()),0);

    for(int i = 0; i < a.size(); i++) {
        result[i] += a[i];
    }
    
    for(int i = 0; i < b.size(); i++ ) {
        result[i+buffer] +=  b[i];
    }
    

    return result;
}

#endif