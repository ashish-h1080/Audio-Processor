#include "MidiFile.h"
#include <iostream>
#include <vector>

typedef struct {
    float startTime;
    float endTime;
    float duration;
    int pitch;
    int velocity;
} Note;

typedef struct {
    std::vector <Note> notes;
} Track;

std::vector <Track> getTracks(const char* filename) {

    smf::MidiFile midi;

    if(!midi.read(filename)) {
        std::cout << "could not read midi file\n";
        return {};
    }

    midi.doTimeAnalysis();
    midi.linkNotePairs();

    int totalNotes = 0;
    int totalTracks = midi.getTrackCount();

    std::vector<Track> tracks(totalTracks);

    for(int t = 0; t < midi.getTrackCount(); t++) {

        for(int e = 0; e < midi[t].size(); e++) {

            auto &event = midi[t][e];
            if(event.isNoteOn()) {
                Note note;
                note.pitch = event.getKeyNumber();
                note.startTime = event.seconds;

                if(event.getLinkedEvent()) {
                    note.endTime =  event.getLinkedEvent()->seconds;
                }
                else {
                    note.endTime = note.startTime;
                }
                
                note.duration = note.endTime - note.startTime;
                note.velocity = event.getVelocity();

                tracks[t].notes.push_back(note);
                totalNotes++;
            }
        }   
    }

    //debug printing

    std::cout << "read:" << filename << ".mid\n";
    std::cout << "read:" << totalTracks << " tracks\n";
    std::cout << "read:" << totalNotes << " notes\n";
    
    return tracks;
}