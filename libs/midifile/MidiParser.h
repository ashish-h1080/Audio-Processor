#pragma once
#ifndef MIDIPARSER_H
#define MIDIPARSER_H

#include <vector>

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

#endif