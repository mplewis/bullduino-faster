/* musicScales.h by Matthew Lewis (mplewis.com; github.com/mplewis)
 * Provides methods for manipulating musical notes using scales (e.g. major, minor, blues)
 */

/* musicNoteFreqs.h
 * provides piano keys and frequencies
 * example usage:
 *     float freqMiddleC = pianoFreq[kC4];
 *     int keyA440 = kA4;
 */
#include "musicNoteFreqs.h"

/* note interval definitions
 * example usage:
 *     float freqPerfectFourth = pianoFreq[kC4 + per4];
 *     int keyTriTone = kC4 + triT;
 */
#define min2 1  // minor second
#define maj2 2  // major second
#define min3 3  // minor third
#define maj3 4  // major third
#define per4 5  // perfect fourth
#define triT 6  // tritone
#define per5 7  // perfect fifth
#define min6 8  // minor sixth
#define maj6 9  // major sixth
#define min7 10 // minor seventh
#define maj7 11 // major seventh
#define per8 12 // perfect octave

/* getMovedKey
 * utility function
 * TODO write docs
 */
int getMovedKey(int keyStart, int numAbove, int* scaleIntervals, int lenScaleArray) {
	
	int numNotesOctave = lenScaleArray + 1;
	
	if (numAbove < 0) {							// if numAbove is negative, move the key down
		while (numAbove <= -numNotesOctave) {		// bump an octave down every six scale notes
			numAbove += numNotesOctave;					// [octave] keys in the given scale
			keyStart -= per8;							// equals one octave in the chromatic scale
		}
		if (numAbove < 0) {							// if numAbove is < 0, indicating there are non-octaves to take care of,
			numAbove = numNotesOctave - 1 + numAbove;	// fix numAbove to refer to the proper interval index value
			keyStart -= (12 - scaleIntervals[numAbove]); // bump the key in the scale interval specified
		}

	} else if (numAbove > 0) {					// if numAbove is positive, move the key up
		while (numAbove >= numNotesOctave) { 		// bump an octave up every six scale notes
			numAbove -= numNotesOctave;					// [octave] keys in the given scale
			keyStart += per8;							// equals one octave in the chromatic scale
		}
		if (numAbove > 0) {							// if numAbove is > 0, indicating there are non-octaves to take care of,
			keyStart += scaleIntervals[numAbove - 1]; // bump the key in the scale interval specified
		}
	}

	if (keyStart < 0 || keyStart > 87) {		// if the note reaches beyond the piano keys
		return -1;									// return -1 error
	} else { 									// otherwise
		return keyStart;					// return the key requested
	}
}

/* freqScaleMajor
 * Takes in a starting keynum and a number of major scale steps to move up
 * Returns the freq value of that note as a float
 * The major scale in C is as follows:
 *      C      D      E      F      G      A      B      C ..... notes
 *      | maj2 | maj2 | min2 | maj2 | maj2 | maj2 | min2 | ..... interior intervals
 *      | maj2 | maj3 | per4 | per5 | maj6 | maj7 | per8 | ..... [C ... note] intervals
 * Example usage:
 *     float cMajorNoteFive = freqScaleMajor(C4, 3); // returns (float)391.995 (Hz, G4)
 */
float freqScaleMajor(int keyStart, int numAbove) {
	int scaleMajorIntervals[6] = {maj2, maj3, per4, per5, maj6, maj7};
	int lenMajorArray = 6;
	return pianoFreq[getMovedKey(keyStart, numAbove, scaleMajorIntervals, lenMajorArray)];
}

/* freqScaleMinor
 * Takes in a starting keynum and a number of minor scale steps to move up
 * Returns the freq value of that note as a float
 * The minor scale in A is as follows:
 *      A      B      C      D      E      F      G      A ..... notes
 *      | maj2 | min2 | maj2 | maj2 | min2 | maj2 | maj2 | ..... interior intervals
 *      | maj2 | min3 | per4 | per5 | min6 | min7 | per8 | ..... [C ... note] intervals
 * Example usage:
 *     float aMinorNoteThree = freqScaleMinor(A4, 2); // returns (float)523.251 (Hz, C5)
 */
float freqScaleMinor(int keyStart, int numAbove) {
	int scaleMinorIntervals[6] = {maj2, min3, per4, per5, min6, min7};
	int lenMinorArray = 6;
	return pianoFreq[getMovedKey(keyStart, numAbove, scaleMinorIntervals, lenMinorArray)];
}

/* freqScaleBlues
 * Takes in a starting keynum and a number of blues scale steps to move up
 * Returns the freq value of that note as a float
 * The blues scale in Bb is as follows:
 *     Bb      Db      Eb      E      F      Ab      Bb .... notes
 *      | min3  | maj2  | min2 | min2 | min3  | min2 | ..... interior intervals
 *      | min3  | per4  | triT | per5 | min7  | per8 | ..... [Bb ... note] intervals
 * Example usage:
 *     float bFlatBluesNoteFour = freqScaleBlues(kAs2, 3); // returns (float)164.814 (Hz, E3)
 */
float freqScaleBlues(int keyStart, int numAbove) {
	int scaleBluesIntervals[5] = {min3, per4, triT, per5, min7};
	int lenBluesArray = 5;
	return pianoFreq[getMovedKey(keyStart, numAbove, scaleBluesIntervals, lenBluesArray)];
}