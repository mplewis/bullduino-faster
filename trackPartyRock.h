#include "musicNoteFreqs.h"

const int partyRockLength = 20; // used for array definition
//                                       par   ty  rock    is    in   the  hous    to  nite    ev   ery   bo    dy   gon    na  have     a  good  time
float partyRock1Note[partyRockLength] = {nC5, nC5, nDs4, nDs4, nDs4, nDs4, nDs5, nDs5, nCs5, nGs4, nAs4, nC5, nAs4, nGs4, nAs4, nDs4, nDs4, nAs4, nGs4, -1};
int   partyRock1Tick[partyRockLength] = {  0,   2,    4,    6,    8,   10,   12,   18,   20,   32,   34,   36,  38,   40,   42,   44,   46,   48,   52, 64};
int   partyRock1Dura[partyRockLength] = {  2,   2,    2,    2,    2,    2,    6,    2,    8,    2,    2,    2,   2,    2,    2,    2,    2,    8,    8, -1};