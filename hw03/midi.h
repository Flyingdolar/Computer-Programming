#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum hFormat {
    SINGLE_TRACK = 0,
    MULTI_TRACK = 1,
    MULTI_SONG = 2
} hFormat;

// MIDI HEADER CHUNK
typedef struct midi_header_chunk {
    uint32_t chunkID;  // Midi Header Chunk: "MThd"
    uint32_t len;      // Header Length: 6 (Regular MIDI) or 7 (MIDI with extensions)
    uint16_t format;   // Format: 0 = [single track], 1 = [multi track], 2 = [multi song]
    uint16_t track;    // Number of Tracks:  1, 2, 3, ...
    uint16_t div;      // Time Division: 0xppqq, pp: ticks per beat, qq: frames per second
} __attribute__((packed)) midiHead;

// MIDI TRACK CHUNK
typedef struct midi_track_chunk {
    uint32_t type;  // Midi Track Chunk: "MTrk"
    uint32_t len;   // Track Length: 0xnnnnnnnn
    uint8_t starPos;
    uint8_t setCmd;
    uint8_t setIns;
} __attribute__((packed)) midiTrack;

// MIDI EVENT
typedef struct midi_longevent {
    uint8_t start;        // MIDI Event Status
    uint8_t noteOn;       // MIDI Event Data 1
    uint8_t note1;        // MIDI Event Data 1
    uint8_t speed1;       // MIDI Event Data 2
    uint16_t delta_time;  // Delta Time
    uint8_t noteOff;      // MIDI Event Data 2
    uint8_t note2;        // MIDI Event Data 1
    uint8_t speed2;       // MIDI Event Data 2
} __attribute__((packed)) midiEventL;

typedef struct midi_shortevent {
    uint8_t start;       // MIDI Event Status
    uint8_t noteOn;      // MIDI Event Data 1
    uint8_t note1;       // MIDI Event Data 1
    uint8_t speed1;      // MIDI Event Data 2
    uint8_t delta_time;  // Delta Time
    uint8_t noteOff;     // MIDI Event Data 2
    uint8_t note2;       // MIDI Event Data 1
    uint8_t speed2;      // MIDI Event Data 2
} __attribute__((packed)) midiEventS;

// MIDI FUNCTION
int parseMidi(FILE *txt, FILE *midi);
