const int numTracks = 5;

float bpmDB[numTracks] =        {130, 124, 169, 113, 193};
int ticksPerBeatDB[numTracks] = {  4,   2,   4,   2,   4};

int currTrack = 0; // starting track
int currBPM = bpmDB[currTrack];
int currTicksPerBeat = ticksPerBeatDB[currTrack];
int currMsPerTick = (60000 / currBPM) / currTicksPerBeat;
int lastTick = -1;
int trackStartTime = 0;