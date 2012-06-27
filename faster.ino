// Friendly Automated Speaking Technological Expert Runner

#include "Button.h"				// for button (next track, pedometer) handling
#include "LiquidCrystal.h"		// for lcd display
#include "Wire.h"				// must be included before synthControl.h
#include "synthControl.h"		// for controlling the ATtiny synths; requires Wire library
#include "pulseSensor.h"		// for reading the pulse sensor module
#include "digitalWriteFast.h"	// faster digital write library

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

// Connect via SPI. Data pin is #3, Clock is #2 and Latch is #4
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
		synthNoiseOn(synthAddr[i]);
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
		Serial.print(" B");
		B = false; // Reset B for next time
	}
	if (QS) { // QS is true when a heart rate is calculated
		Serial.println();
		Serial.print(BPM);
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

	int currTick = (millis() - trackStartTime) / currMsPerTick;
	if (currTick > lastTick) {
		/*Serial.print("Tick changed: ");
		Serial.println(currTick); */
		lastTick = currTick;
	}
	if (currTick % currTicksPerBeat == 0) {
		digitalWrite(pinDebug, HIGH);
	} else {
		digitalWrite(pinDebug, LOW);
	}

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
	int trackStartTime = millis();

	updateTrackDisplay();

}

void updateTrackDisplay() {
	lcd.setCursor(0, 3);
	lcd.print("Track: ");
	switch(currTrack) {
		case 0:
			lcd.print("PrtyRckAnthem");
			break;

		case 1:
			lcd.print("HrdBetFstStrg");
			break;

		case 2:
			lcd.print("Sabotage     ");
			break;

		case 3:
			lcd.print("D.A.N.C.E.   ");
			break;

		case 4:
			lcd.print("Trolololololo");
			break;


		default:
			lcd.print("Error        ");
			break;
	}
}