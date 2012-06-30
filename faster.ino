// Friendly Automated Speaking Technological Expert Runner

#include "SoftwareSerial.h"     // for SpeakJet interface
#include "Bounce.h"             // for button (pedometer switch) handling
#include "LiquidCrystal.h"      // for lcd display
#include "Wire.h"               // must be included before synthControl.h
#include "synthControl.h"       // for controlling the ATtiny synths; requires Wire library
#include "pulseSensor.h"        // for reading the pulse sensor module
#include "digitalWriteFast.h"   // faster digital write library
#include "trackInfo.h"          // data for LMFAO - Party Rock Anthem

/* lcd display: 20 x 4
 * 4 lines: HR sliding bar display, HR numbers, steps, current track
 */
#define LCD_HORZ 20
#define LCD_VERT 4

const int pinLcdData = 11;
const int pinLcdClk = 12;
const int pinLcdLatch = 10;

// Connect LCD via SPI using the Adafruit library: https://github.com/adafruit/LiquidCrystal
LiquidCrystal lcd(pinLcdData, pinLcdClk, pinLcdLatch);

const int pinSpeakJetRX = 4;
const int pinSpeakJetTX = 5;
SoftwareSerial serialSpeakJet = SoftwareSerial(pinSpeakJetRX, pinSpeakJetTX);

const int pinPedometer = 3;
Bounce pedometer = Bounce(pinPedometer, 5); // 5ms debounce time

const int pinChipEnable = 2; // active low
bool chipEnable = true;

const int tempoArraySize = 5;
int lastTempo[tempoArraySize] = {0};
int tempoArrayPos = 0;

const int pinDebug = 13;

char testSJ[] = {20, 127, 21, 120, 23, 4, 22, 130, 199, 152, 174, 128, 78, 148, 135, 197};

void setup() {
	pinMode(pinSpeakJetRX, INPUT);
	pinMode(pinSpeakJetTX, OUTPUT);
	serialSpeakJet.begin(9600);

	pinMode(pinLcdData, OUTPUT);
	pinMode(pinLcdClk, OUTPUT);
	pinMode(pinLcdLatch, OUTPUT);

	pinMode(pinPedometer, INPUT);
	digitalWrite(pinPedometer, HIGH);

	pinMode(pinChipEnable, INPUT);
	digitalWrite(pinChipEnable, HIGH);

	pinMode(pinDebug, OUTPUT);

	setupTimersHeartMon();

	Wire.begin();

	synthOff(addrSyn1);
	synthLedOff(addrSyn1);
	synthOff(addrSyn2);
	synthLedOff(addrSyn2);

	lcd.begin(LCD_HORZ, LCD_VERT);
	lcd.setBacklight(LOW);
	lcd.setCursor(0, 1);
	lcd.print("90      ...      200");
}

void loop() {

	checkPulse();
	readButtons();
	if (chipEnable) {
		playNote();
	} else {
		synthOff(addrSyn1);
		synthOff(addrSyn2);
	}

}

void readButtons() {
	if (digitalRead(pinChipEnable) == LOW) { // 
		chipEnable = true;
	} else {
		chipEnable = false;
	}

	pedometer.update();
	if (pedometer.risingEdge()) {
		updateTempo();
	}
}

void updateTempo() {
	// nop
}

void checkPulse() {
	if (B) {                // B is true when a pulse is triggered
		B = false;          // Reset B for next time
	}
	if (QS) {               // QS is true when a heart rate is calculated
		updateHR();
		QS = false;	        // Reset QS for next time
	}
}

void updateHR() {
	lcd.setCursor(8, 1);
	lcd.print("    ");              // clear display HR section
	lcd.setCursor(8, 1);
	if (BPM < 0 || BPM > 999) {
		lcd.print("...");           // don't print bad (negative, >999) HRs
	} else if (BPM < 100) {		
		lcd.print(" ");             // insert spacing for 2-digit HRs
		lcd.print(BPM);             // print HR to display
	} else {
		lcd.print(BPM);             // print HR to display
	}
}

void playNote() {

	if (millis() > lastTickMs + msPerTick) {

		lastTickMs = millis();
		lastTick = currTick;
		currTick++;

		if (currTick == finalTick) {
			restartTrack();
		}

		// catch up to looped section
		while (pgm_read_word_near(pRock1Tick + pRock1Pos) < currTick) {
			pRock1Pos++;
		}

		while (pgm_read_word_near(pRock2Tick + pRock2Pos) < currTick) {
			pRock2Pos++;
		}

		while (pgm_read_word_near(speakTick + speakPos) < currTick) {
			speakPos++;
		}

		if (currTick == pRock1NoteOffTick) {
			synthOff(addrSyn1);
		}

		if (currTick == pRock2NoteOffTick) {
			synthOff(addrSyn2);
		}

		if (currTick == pgm_read_word_near(pRock1Tick + pRock1Pos)) {			
			synthSetFreq(addrSyn1, pianoFreq[pgm_read_word_near(pRock1Key + pRock1Pos)]);
			synthOn(addrSyn1);

			pRock1NoteOffTick = currTick + pgm_read_word_near(pRock1Dura + pRock1Pos);

			pRock1Pos++;
		}

		if (currTick == pgm_read_word_near(pRock2Tick + pRock2Pos)) {			
			synthSetFreq(addrSyn2, pianoFreq[pgm_read_word_near(pRock2Key + pRock2Pos)]);
			synthOn(addrSyn2);

			pRock2NoteOffTick = currTick + pgm_read_word_near(pRock2Dura + pRock2Pos);

			pRock2Pos++;
		}

		if (currTick == pgm_read_word_near(speakTick + speakPos)) {			
			int firstSpeakByte = pgm_read_word_near(speakIndex + speakPos);
			int lastSpeakByte = pgm_read_word_near(speakIndex + speakPos + 1) - 1;
			speakJetSay(firstSpeakByte, lastSpeakByte);
			speakPos++;
		}
	}
	
}

void speakJetSay(int firstByte, int lastByte) {
	cli();
	for (int i = firstByte; i <= lastByte; i++) {
		serialSpeakJet.write(pgm_read_byte_near(speakData + i));
	}
	sei();
}

void restartTrack() {
	synthOff(addrSyn1);
	synthOff(addrSyn2);
	currTick = lastTick = firstTick - 1;
	lastTickMs = 0;
	pRock1Pos = pRock2Pos = speakPos = 0;
}