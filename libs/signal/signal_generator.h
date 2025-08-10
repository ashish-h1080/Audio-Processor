#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <iostream>
#include <cmath>
#include <vector>

#define PI acos(-1)

std::vector<float> signalSine(float amplitude, float frequency, float time, int sampleRate) {

    std::vector<float> signal;

    int samples = static_cast<int>(sampleRate*time);

    for(int i = 0; i < samples; i++) {

        float t = static_cast<float>(i)/sampleRate;
        float y = amplitude* std::sin(2*PI*frequency*t);
        signal.push_back(y);
        
    }

    return signal;
}

std::vector<float> signalSquare(float amplitude, float frequency, float time, int sampleRate) {

    std::vector<float> signal;

    int samples = static_cast<int>(sampleRate*time);

    for(int i = 0; i < samples; i++) {
        float t = static_cast<float>(i)/sampleRate;
        float y = amplitude*std::sin(2*PI*frequency*t) >= 0 ? 1 : -1;
        signal.push_back(y);
    }

    return signal;
}

std::vector<float> signalSawtooth(float frequency, float sampleRate, float time) {
    int totalSamples = static_cast<int>(sampleRate * time);

    std::vector<float> signal(totalSamples);

    for (int i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        signal[i] = 2 * (t * frequency - std::floor(0.5 + t * frequency));
    }

    return signal;
}

std::vector<float> signalAdd (std::vector<float>const &a, std::vector<float>const &b, int sampleRate, float time = 0) {

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

std::vector <float> signalJoin (std::vector <float>a, std::vector<float>b, int sampleRate){
    std::vector <float> result;
    for(float i : a) {
        result.push_back(i);
    }

    for(float i : b) {
        result.push_back(i);
    }
}

#endif