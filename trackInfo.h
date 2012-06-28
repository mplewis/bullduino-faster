#include "musicNoteFreqs.h"

const int pRock1Length = 20; // used for array definition
const int pRock2Length = 0;

//                                          par    ty  rock    is    in   the house                to night                                  ev   ery    bo    dy   gon    na  have     a  good        time
//                                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
int pRock1Key[pRock1Length]  = {            kC5,  kC5, kDs4, kDs4, kDs4, kDs4, kDs5,             kDs5, kCs5,                               kGs4, kAs4,  kC5, kAs4, kGs4, kAs4, kDs4, kDs4, kAs4,       kGs4, -1};
int pRock1Tick[pRock1Length] = {              4,    6,    8,   10,   12,   14,   16,               22,   24,                                                                };
//                                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
int pRock2Key[pRock2Length]  = {kF2,  kF3,  kF2,  kF3,  kF2,  kF3,  kF2,  kF3, kDs2, kDs3, kDs2, kDs3, kCs2, kCs3, kCs2, kCs3,  kF2,  kF3,  kF2,  kF3,  kF2,  kF3,  kF2,  kF3, kDs2, kDs3, kDs2, kDs3, kCs2, kCs3, kCs2, kCs3};
int pRock2Tick[pRock2Length] = {  0,    2,    4,    6,    8,   10,   12,   14,   16,   18,   20,   22,   24,   26,   28,   30,   32,   34,   36,   38,   40,   42,   44,   46,   48,   50,   52,   54,   56,   58,   60,   62,   64};
//                                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
// int pRock1Dura[pRock1Length] = {};
// int pRock2Dura[pRock2Length] = {};

const int numTracks = 5;

int bpm = 130;
int ticksPerBeat = 4;
int msPerTick = (60000 / bpm) / ticksPerBeat;

long currTick = -1;
long lastTick = -1;
long lastTickMs = -1;

int synth1TrackPos = -1;

long trackStartTime = millis();

     |     |