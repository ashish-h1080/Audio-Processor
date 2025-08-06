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

std::vector<float> ADSR (float time, float sampleRate, float aCurvature, float aMagnitude, float dCurvature, float dMagnitude,float rCurvature, float A, float D, float S, float R) {

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

//square wave
//padding
//clipping
