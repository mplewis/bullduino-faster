// Friendly Automated Speaking Technological Expert Runner

#include "Bounce.h"				// for button (pedometer switch) handling
#include "LiquidCrystal.h"		// for lcd display
#include "Wire.h"				// must be included before synthControl.h
#include "synthControl.h"		// for controlling the ATtiny synths; requires Wire library
#include "pulseSensor.h"		// for reading the pulse sensor module
#include "digitalWriteFast.h"	// faster digital write library
#include "trackInfo.h"			// data for LMFAO - Party Rock Anthem

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
Bounce pedometer = Bounce(pinPedometer, 5); // 5ms debounce time
const int tempoArraySize = 5;
int lastTempo[tempoArraySize] = {0};
int tempoArrayPos = 0;

const int pinDebug = 13;



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
	pedometer.update();
	if (pedometer.risingEdge()) {
		updateTempo();
	}
}

void updateTempo() {
	// nop
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

	if (millis() > lastTickMs + msPerTick) {
		lastTickMs = millis();
		lastTick = currTick;
		currTick++;

		if (currTick == resetTick) {
			restartTrack();
		} else if (/* currTick % ticksPerBeat == 0 */ true) {
			Serial.print("Time: ");
			Serial.print(millis());
			Serial.print("\tTick: ");
			Serial.print(currTick);
		}

		if (currTick == pRock1NoteOffTick) {
			synthOff(addrSyn1);
		}

		if (currTick == pRock1Tick[pRock1Pos]) {
			Serial.print("\tNote: ");
			Serial.print(pRock1Key[pRock1Pos]);
			Serial.print("\tFreq: ");
			Serial.print(pianoFreq[pRock1Key[pRock1Pos]]);

			synthSetFreq(addrSyn1, pianoFreq[pRock1Key[pRock1Pos]]);
			synthOn(addrSyn1);

			pRock1NoteOffTick = currTick + pRock1Dura[pRock1Pos];

			pRock1Pos++;
		}
		if (currTick == pRock2Tick[pRock2Pos]) {
			Serial.print("\tNote: ");
			Serial.print(pRock2Key[pRock2Pos]);
			Serial.print("\tFreq: ");
			Serial.print(pianoFreq[pRock2Key[pRock2Pos]]);
			pRock2Pos++;
		}

		Serial.println();
	}
	
}

void restartTrack() {
	Serial.println("Restarting track.");
	currTick = lastTick = -1;
	lastTickMs = 0;
	pRock1Pos = pRock2Pos = 0;
	synthOff(synthAddr[0]);
}