#ifndef SIGNAL_CONTROLLER_H
#define SIGNAL_CONTROLLER_H

#include <iostream>
#include <cmath>
#include <vector>

#define PI acos(-1)

void signalGain (std::vector<float> &sample, float gain) {

    for(int i = 0; i < sample.size(); i++) {
        sample[i] *= gain;
    }
}

void signalNormalize (std::vector<float>& signal) {
    float maxAmp = 0.0;

    for (auto& sample : signal) {
        float absSample = std::abs(sample);
        if (absSample > maxAmp) {
            maxAmp = absSample;
        }
    }

    if (maxAmp == 0.0) {
        return;
    }

    for (auto& sample : signal) {
        sample /= maxAmp;
    }
    
}

void signalEnvelope (std::vector<float> &sample, std::vector<float> &envelope) {

    envelope.resize(sample.size(),1);
    for(int i = 0; i < sample.size(); i++) {
        sample[i] *= envelope[i];
    }
}

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

#endif