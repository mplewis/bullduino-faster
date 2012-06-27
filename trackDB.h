const int numTracks = 5;

float bpmDB[numTracks] =        {130, 124, 169, 113, 193};
int ticksPerBeatDB[numTracks] = {  4,   2,   4,   2,   4};

int currTrack = 0; // starting track
int currBPM = bpmDB[currTrack];
int currTicksPerBeat = ticksPerBeatDB[currTrack];
int currMsPerTick = (60000 / currBPM) / currTicksPerBeat;
long currTick = -1;
long lastTick = -1;
long trackStartTime = 0;
int synth1TrackPos = -1;
long synth1LastTick = -1;