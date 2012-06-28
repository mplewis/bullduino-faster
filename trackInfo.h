#include "musicNoteFreqs.h"

// I2C synth addresses
const int addrSyn1 = 02;
const int addrSyn2 = 03;

const int pRock1Length = 20; // used for array definition
const int pRock2Length = 33;

//                             |     |     |  par|   ty| rock|   is|   in|  the|house|     |     |   to|night|     |     |     |     |     |   ev|  ery|   bo|   dy| just|     | have|    a| good|     | time|     |     |     |     |     |     |     |  and|   we|  gon| make|  you| lose|     |     | your| mind|     |     |     |     |     |   ev|  ery|   bo|   dy| just|     | have|    a| good|     | time|     |     |     |     |     |     |  par|   ty| rock|   is|   in|  the|house|     |     |   to|night|     |     |     |     |     |   ev|  ery|   bo|   dy| just|     | have|    a| good|     | time|     |     |     |     |     |     |     |  and|   we|  gon| make|  you| lose|     |     | your| mind|     |     |     |     |     |   we| just|  wan|   na|  see|  you|     |     |     |     |     |     |     |shake| that|
int pRock1Key[pRock1Length]  = {              kC5,  kC5, kDs4, kDs4, kDs4, kDs4, kDs5,             kDs5, kCs5,                               kGs4, kAs4,  kC5, kAs4, kGs4,       kDs4, kDs4, kAs4,       kGs4,                     -1};
int pRock1Tick[pRock1Length] = {                4,    6,    8,   10,   12,   14,   16,               22,   24,                                 36,   38,   40,   42,   44,         48,   50,   52,         56,                     64};
int pRock1Dura[pRock1Length] = {                2,    2,    2,    1,    2,    1,    5,                2,    8,                                  2,    1,    2,    1,    4,          2,    1,    3,          7,                     -1};
//                             |     |     |  par|   ty| rock|   is|   in|  the|house|     |     |   to|night|     |     |     |     |     |   ev|  ery|   bo|   dy| just|     | have|    a| good|     | time|     |     |     |     |     |     |     |  and|   we|  gon| make|  you| lose|     |     | your| mind|     |     |     |     |     |   ev|  ery|   bo|   dy| just|     | have|    a| good|     | time|     |     |     |     |     |     |  par|   ty| rock|   is|   in|  the|house|     |     |   to|night|     |     |     |     |     |   ev|  ery|   bo|   dy| just|     | have|    a| good|     | time|     |     |     |     |     |     |     |  and|   we|  gon| make|  you| lose|     |     | your| mind|     |     |     |     |     |   we| just|  wan|   na|  see|  you|     |     |     |     |     |     |     |shake| that|
int pRock2Key[pRock2Length]  = {  kF2,  kF3,  kF2,  kF3,  kF2,  kF3,  kF2,  kF3, kDs2, kDs3, kDs2, kDs3, kCs2, kCs3, kCs2, kCs3,  kF2,  kF3,  kF2,  kF3,  kF2,  kF3,  kF2,  kF3, kDs2, kDs3, kDs2, kDs3, kCs2, kCs3, kCs2, kCs3,   -1};
int pRock2Tick[pRock2Length] = {    0,    2,    4,    6,    8,   10,   12,   14,   16,   18,   20,   22,   24,   26,   28,   30,   32,   34,   36,   38,   40,   42,   44,   46,   48,   50,   52,   54,   56,   58,   60,   62,   64};
int pRock2Dura[pRock2Length] = {    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,   -1};

int resetTick = 64;

int bpm = 130;
int ticksPerBeat = 4;
int msPerTick = (60000 / bpm) / ticksPerBeat;

long currTick = -1;
long lastTick = -1;
long lastTickMs = 0;

int pRock1Pos = 0;
int pRock2Pos = 0;
int pRock1NoteOffTick = -1;
int pRock2NoteOffTick = -1;