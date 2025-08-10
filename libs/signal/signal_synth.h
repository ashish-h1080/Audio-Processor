#ifndef SIGNAL_SYNTH_H
#define SIGNAL_SYNTH_H

#include <iostream>
#include <cmath>
#include <vector>

#include "./midifile/MidiParser.h"

#define PI acos(-1)

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

        for(auto& note : T.notes) {

            int start = static_cast<int>(note.startTime*44100);
            int end   = static_cast<int>(note.endTime*44100);

            for (int i = start; i < end; i++) {
                float ts = (i - start) / static_cast<float>(44100);

                buffer[i] += synthSine(0.8, pitchFrequency(note.pitch, 20), ts);
                buffer[i] += synthSquare(0.6, pitchFrequency(note.pitch, -20),ts);

            }

        }   

    signalNormalize(buffer);
}

#endif