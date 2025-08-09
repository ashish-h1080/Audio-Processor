#pragma once
#include <vector>
#include "MidiFile.h"

struct Note {
    float startTime;
    float endTime;
    float duration;
    int pitch;
    int velocity;
};

struct Track {
    std::vector<Note> notes;
};

std::vector<Track> getTracks(const char* filename);
