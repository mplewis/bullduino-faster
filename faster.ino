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
}

void checkPulse() {
	if (B) { 				// B is true when a pulse is triggered
		B = false; 			// Reset B for next time
	}
	if (QS) {				// QS is true when a heart rate is calculated
		updateHR();
		QS = false;			// Reset QS for next time
	}
}

void updateHR() {
	lcd.setCursor(8, 1);
	lcd.print("    ");				// clear display HR section
	lcd.setCursor(8, 1);
	if (BPM < 0 || BPM > 999) {
		lcd.print("...");			// don't print bad (negative, >999) HRs
	} else if (BPM < 100) {		
		lcd.print(" ");				// insert spacing for 2-digit HRs
		lcd.print(BPM);				// print HR to display
	} else {
		lcd.print(BPM);				// print HR to display
	}
	
}

void playNote() {

	currTick = ((millis() - trackStartTime) / msPerTick) - 1; // -1 to start indexing at 0
	if (currTick > lastTick) {
		Serial.print(currTick);
		Serial.print('\t');
		Serial.print(pRock1Tick[synth1TrackPos]);
		Serial.print('\t');
		Serial.println(synth1TrackPos);
		lastTick = currTick;
	}
	
}

void restartTrack() {
	Serial.println("Restarting track.");
	trackStartTime = millis();
	currTick = -1;
	lastTick = -1;
	synth1TrackPos = -1;
	synthOff(synthAddr[0]);
}