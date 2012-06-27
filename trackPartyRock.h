#include "musicNoteFreqs.h"

const int partyRockLength = 20; // used for array definition
//                                       par   ty  rck   is   in   the hou   to  nit   ev  ery   bo   dy  gon   na  hav    a   gd  tme
float partyRock1Note[partyRockLength] = {nB4, nB4, nD4, nD4, nD4, nD4, nD5, nD5, nC5, nG4, nA4, nB4, nA4, nG4, nA4, nD4, nD4, nA4, nG4, -1};
int   partyRock1Tick[partyRockLength] = {  0,   2,   4,   6,   8,  10,  12,  18,  20,  32,  34,  36,  38,  40,  42,  44,  46,  48,  52, 64};
int   partyRock1Dura[partyRockLength] = {  2,   2,   2,   2,   2,   2,   6,   2,   8,   2,   2,   2,   2,   2,   2,   2,   2,   3,   6, -1};