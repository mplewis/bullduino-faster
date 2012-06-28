#include "musicNoteFreqs.h"

const int pRock1Length = 20; // used for array definition
//                                par   ty  rock    is    in   the  hous    to  nite    ev   ery   bo    dy   gon    na  have     a  good  time
float pRock1Note[pRock1Length] = {nC5, nC5, nDs4, nDs4, nDs4, nDs4, nDs5, nDs5, nCs5, nGs4, nAs4, nC5, nAs4, nGs4, nAs4, nDs4, nDs4, nAs4, nGs4, -1};
int   pRock1Tick[pRock1Length] = {  0,   2,    4,    6,    8,   10,   12,   18,   20,   32,   34,   36,  38,   40,   42,   44,   46,   48,   52, 64};
int   pRock1Dura[pRock1Length] = {  2,   2,    2,    2,    2,    2,    6,    2,    8,    2,    2,    2,   2,    2,    2,    2,    2,    8,    8, -1};

const int pRock2Length = 0;
float pRock2Note[pRock2Length] = {};
int   pRock2Tick[pRock2Length] = {};
int   pRock2Dura[pRock2Length] = {};

const int numTracks = 5;

int bpm = 130;
int ticksPerBeat = 4;
int msPerTick = (60000 / bpm) / ticksPerBeat;

long currTick = -1;
long lastTick = -1;

int synth1TrackPos = -1;

long trackStartTime = millis();