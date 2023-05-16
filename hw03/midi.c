#include "midi.h"

char *instrument[128] = {
    "Acoustic Grand Piano", "Bright Acoustic Piano", "Electric Grand Piano", "Honky-tonk Piano",
    "Electric Piano 1 (Rhodes Piano)", "Electric Piano 2 (Chorused Piano)", "Harpsichord", "Clavinet", "Celesta",
    "Glockenspiel", "Music Box", "Vibraphone", "Marimba", "Xylophone", "Tubular Bells", "Dulcimer (Santur)",
    "Drawbar Organ (Hammond)", "Percussive Organ", "Rock Organ", "Church Organ", "Reed Organ", "Accordion (French)",
    "Harmonica", "Tango Accordion (Band neon)", "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)",
    "Electric Guitar (jazz)", "Electric Guitar (clean)", "Electric Guitar (muted)", "Overdriven Guitar",
    "Distortion Guitar", "Guitar harmonics", "Acoustic Bass", "Electric Bass (fingered)", "Electric Bass (picked)",
    "Fretless Bass", "Slap Bass 1", "Slap Bass 2", "Synth Bass 1", "Synth Bass 2", "Violin", "Viola", "Cello",
    "Contrabass", "Tremolo Strings", "Pizzicato Strings", "Orchestral Harp", "Timpani", "String Ensemble 1 (strings)",
    "String Ensemble 2 (slow strings)", "SynthStrings 1", "SynthStrings 2", "Choir Aahs", "Voice Oohs", "Synth Voice",
    "Orchestra Hit", "Trumpet", "Trombone", "Tuba", "Muted Trumpet", "French Horn", "Brass Section", "SynthBrass 1",
    "SynthBrass 2", "Soprano Sax", "Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe", "English Horn", "Bassoon",
    "Clarinet", "Piccolo", "Flute", "Recorder", "Pan Flute", "Blown Bottle", "Shakuhachi", "Whistle", "Ocarina",
    "Lead 1 (square)", "Lead 2 (sawtooth)", "Lead 3 (calliope)", "Lead 4 (chiff)", "Lead 5 (charang)", "Lead 6 (voice)",
    "Lead 7 (fifths)", "Lead 8 (bass + lead)", "Pad 1 (new age)", "Pad 2 (warm)", "Pad 3 (polysynth)", "Pad 4 (choir)",
    "Pad 5 (bowed)", "Pad 6 (metallic)", "Pad 7 (halo)", "Pad 8 (sweep)", "FX 1 (rain)", "FX 2 (soundtrack)",
    "FX 3 (crystal)", "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)", "FX 7 (echoes)", "FX 8 (sci-fi)",
    "Sitar", "Banjo", "Shamisen", "Koto", "Kalimba", "Bagpipe", "Fiddle", "Shanai", "Tinkle Bell", "Agogo",
    "Steel Drums", "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum", "Reverse Cymbal", "Guitar Fret Noise",
    "Breath Noise", "Seashore", "Bird Tweet", "Telephone Ring", "Helicopter", "Applause", "Gunshot"};

midiHead newHead(hFormat type, int trackNum, int timeDivision) {
    midiHead head;
    head.chunkID = 0x4D546864;
    head.len = 6;
    head.format = type, head.track = trackNum;
    head.div = timeDivision;
    head.chunkID = __builtin_bswap32(head.chunkID);
    head.len = __builtin_bswap32(head.len);
    head.format = __builtin_bswap16(head.format);
    head.track = __builtin_bswap16(head.track);
    head.div = __builtin_bswap16(head.div);
    return head;
}

midiTrack newTrack(char *fInstr) {
    midiTrack track;
    track.type = 0x4D54726B, track.len = 0;
    track.len = 0, track.starPos = 0;
    track.setCmd = 0xC0;
    for (size_t idx = 0; idx < 128; idx++) {
        if (strcmp(fInstr, instrument[idx]) == 0) {
            track.setIns = idx;
            break;
        }
    }
    track.type = __builtin_bswap32(track.type);
    track.len = __builtin_bswap32(track.len);
    return track;
}

midiEventL newEventL(int note, int len, int spd) {
    midiEventL evtL;
    evtL.noteOn = 0x90, evtL.noteOff = 0x80;
    evtL.note1 = note, evtL.note2 = note;
    evtL.speed1 = spd, evtL.speed2 = spd;
    evtL.delta_time = len, evtL.start = 0;
    evtL.delta_time = __builtin_bswap16(evtL.delta_time);
    return evtL;
}

midiEventS newEventS(int note, int len, int spd) {
    midiEventS evtS;
    evtS.noteOn = 0x90, evtS.noteOff = 0x80;
    evtS.note1 = note, evtS.note2 = note;
    evtS.speed1 = spd, evtS.speed2 = spd;
    evtS.delta_time = len, evtS.start = 0;
    return evtS;
}

int isNum(char *str) {
    if (str[0] < '0' || str[0] > '9') return -1;
    return *str - '0';
}

int transLen(int len) {
    if (len < 128) return len;
    int ret = 0;
    while (len > 0) {
        ret = (ret << 8) | (len & 0x7F);
        ret |= 0x80;
        len >>= 7;
    }
    return ret - 0x80;
}

int parseMidi(FILE *txt, FILE *midi) {
    char txtLine[1000], *str;
    int track = 0, div = 480;
    int noteNum[8] = {0, 0, 2, 4, 5, 7, 9, 11};
    int noteOct[10] = {12, 24, 36, 48, 60, 72, 84, 96, 108, 120};

    fseek(txt, 0, SEEK_SET), fseek(midi, 0, SEEK_SET);
    while (fgets(txtLine, 1000, txt) != NULL)
        if (txtLine[0] == '[') track++;
    midiHead head = newHead(MULTI_TRACK, track, div);
    fwrite(&head, 1, sizeof(midiHead), midi);
    fseek(txt, 0, SEEK_SET), track = -1;

    while (fgets(txtLine, 1000, txt) != NULL) {
        if (txtLine[0] == '[') {
            track++;
            midiTrack mTrack = newTrack(strtok(txtLine, "[]"));
            fwrite(&mTrack, sizeof(midiTrack), 1, midi);
            if (track > 0) {
                // Write end of track
                char end[3] = {0x00, 0xFF, 0x2F};
                fwrite(end, 3, 1, midi);
            }
            continue;
        }
        str = txtLine;
        while (*str != '\n' && *str != 0) {
            int note = isNum(str), len = 480, oct = 4;
            note = noteNum[note];
            str++;
            while (isNum(str) != -1 && *str != '\n' && *str != 0) {
                if (*str == '-') len *= 2;
                if (*str == '.') len += len / 2;
                if (*str == ';') len /= 2;
                if (*str == '\'') oct++;
                if (*str == ',') oct--;
                str++;
            }
            len = transLen(len), note += noteOct[oct];
            if (len > 127) {
                midiEventL mEvtL = newEventL(note, len, 0x20);
                fwrite(&mEvtL, sizeof(midiEventL), 1, midi);
            } else {
                midiEventS mEvtS = newEventS(note, len, 0x20);
                fwrite(&mEvtS, sizeof(midiEventS), 1, midi);
            }
        }
    }
}
