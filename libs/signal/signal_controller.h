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



#endif