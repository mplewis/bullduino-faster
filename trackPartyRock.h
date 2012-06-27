#include "musicNoteFreqs.h"

const int partyRockLength = 20; // used for array definition
//                                       par   ty  rck   is   in   the hou   to  nit   ev  ery   bo   dy  gon   na  hav    a   gd  tme
float partyRock1Note[partyRockLength] = {nC5, nC5, nE4, nE4, nE4, nE4, nE5, nE5, nD5, nA4, nB4, nC4, nB4, nA4, nB4, nE4, nE4, nB4, nA4, -1};
int   partyRock1Tick[partyRockLength] = {  0,   2,   4,   6,   8,  10,  12,  18,  20,  32,  34,  36,  38,  40,  42,  44,  46,  48,  52, 60};
int   partyRock1Dura[partyRockLength] = {  2,   2,   2,   2,   2,   2,   6,   2,   8,   2,   2,   2,   2,   2,   2,   2,   2,   3,   6, -1};