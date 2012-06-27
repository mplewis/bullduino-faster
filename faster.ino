// Friendly Automated Speaking Technological Expert Runner

#include "Button.h"				// for button (next track, pedometer) handling
#include "LiquidCrystal.h"		// for lcd display
#include "Wire.h"				// must be included before synthControl.h
#include "synthControl.h"		// for controlling the ATtiny synths; requires Wire library
#include "pulseSensor.h"		// for reading the pulse sensor module
// #include "digitalWriteFast.h"	// faster digital write library

#include "trackDB.h"			// stores the reference information for each track and initializes track bookkeeping vars
#include "trackPartyRock.h"		// LMFAO - Party Rock Anthem

/* lcd display: 20 x 4
 * 4 lines: HR sliding bar display, HR numbers, steps, current track
 */
#define LCD_HORZ 20
#define LCD_VERT 4

const int pinLcdData = 12;
const int pinLcdClk = 13;
const int pinLcdLatch = 11;

// Connect LCD via SPI using the Adafruit library: https://github.com/adafruit/LiquidCrystal
LiquidCrystal lcd(pinLcdData, pinLcdClk, pinLcdLatch);

// tracks: Party Rock Anthem, Harder Better Faster Stronger, Sabotage, D.A.N.C.E., Trololo
const int pinNextTrack = 2;
Button nextTrack = Button(pinNextTrack, BUTTON_PULLUP);

const int pinPedometer = 3;
Button pedometer = Button(pinPedometer, BUTTON_PULLUP);
const int tempoArraySize = 5;
int lastTempo[tempoArraySize] = {0};
int tempoArrayPos = 0;

const int pinDebug = 13;

const int numSynths = 1;
int synthAddr[numSynths] = {02};

void setup() {
	Serial.begin(115200);

	pinMode(pinLcdData, OUTPUT);
	pinMode(pinLcdClk, OUTPUT);
	pinMode(pinLcdLatch, OUTPUT);

	pinMode(pinNextTrack, INPUT);
	digitalWrite(pinNextTrack, HIGH);
	pinMode(pinPedometer, INPUT);
	digitalWrite(pinPedometer, HIGH);

	pinMode(pinDebug, OUTPUT);

	setupTimersHeartMon();

	Wire.begin();

	for (int i = 0; i < numSynths; i++) {
		synthOff(synthAddr[i]);
		synthLedOff(synthAddr[i]);
	}

	lcd.begin(LCD_HORZ, LCD_VERT);
	lcd.setBacklight(LOW);
	lcd.setCursor(0, 1);
	lcd.print("90      ...      200");

	updateTrackDisplay();

	Serial.println("Ready to go!");
}

void loop() {

	readButtons();
	checkPulse();
	playNote();

}

void readButtons() {
	/* if (pedometer.uniquePress()) {
		updateTempo();
	} */
	if (nextTrack.uniquePress()) {
		changeTrack();
	}
	// digitalWrite(pinDebug, nextTrack.isPressed());
}

void checkPulse() {
	if (B) { // B is true when a pulse is triggered
		// Serial.print(" B");
		B = false; // Reset B for next time
	}
	if (QS) { // QS is true when a heart rate is calculated
		// Serial.println();
		// Serial.print(BPM);
		updateHR();
		QS = false; // reset for next time
	}
}

void updateHR() {
	lcd.setCursor(8, 1);
	if (BPM < 100) {
		lcd.print(" ");
	}
	lcd.print(BPM);
}

void playNote() {

	currTick = ((millis() - trackStartTime) / currMsPerTick) - 1; // -1 to start indexing at 0
	if (currTick > lastTick) {
		/* Serial.print("Tick: ");
		Serial.println(currTick);
		Serial.print(partyRock1Tick[synth1TrackPos]);
		Serial.print('\t');
		Serial.println(synth1TrackPos); */
		lastTick = currTick;
		digitalWrite(pinDebug, !digitalRead(pinDebug));
	}
	if (partyRock1Note[synth1TrackPos] == -1) {
		restartTrack();
	}
	if (partyRock1Tick[synth1TrackPos] == currTick) {
		synth1TrackPos++;
		Serial.print("Note: ");
		Serial.println(partyRock1Note[synth1TrackPos]);
		synthOn(synthAddr[0]);
		synthSetFreq(synthAddr[0], partyRock1Note[synth1TrackPos]);
	}

}

void restartTrack() {
	Serial.println("Restarting track.");
	trackStartTime = millis();
	currTick = 0;
	synth1TrackPos = 0;
	synthOff(synthAddr[0]);
}

void changeTrack() {

	currTrack++;
	if (currTrack == numTracks) {
		currTrack = 0;
	}

	Serial.print("Track changed: ");
	Serial.println(currTrack);

	currBPM = bpmDB[currTrack];
	currTicksPerBeat = ticksPerBeatDB[currTrack];
	currMsPerTick = (60000 / currBPM) / currTicksPerBeat;
	
	restartTrack();

	updateTrackDisplay();

}

void updateTrackDisplay() {
	char* trackTitles[5] = {
		"PrtyRckAnthem",
		"HrdBetFstStrg",
		"Sabotage     ",
		"D.A.N.C.E.   ",
		"Trolololololo"
	};
	lcd.setCursor(0, 3);
	lcd.print("Track: ");
	lcd.print(trackTitles[currTrack]);
}