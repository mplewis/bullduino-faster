// Friendly Automated System To Enhance Running

#include "Button.h"
#include "Wire.h"
#include "LiquidCrystal.h"

#define LCD_HORZ 20
#define LCD_VERT 4

LiquidCrystal lcd(0);

// tracks: Party Rock Anthem, Harder Better Faster Stronger, Sabotage, D.A.N.C.E., Trololo
int pinNextTrack = 2;
Button nextTrack = Button(pinNextTrack, BUTTON_PULLUP);
int currTrack = 0; // -1 because the first uniquePress increments the track to 0
int numTracks = 5;

int pinPedometer = 3;
Button pedometer = Button(pinPedometer, BUTTON_PULLUP);
const int tempoArraySize = 5;
int lastTempo[tempoArraySize] = {0};
int tempoArrayPos = 0;

int debug = 13;

void setup() {
	Serial.begin(115200);
	lcd.begin(LCD_HORZ, LCD_VERT);
	lcd.setBacklight(LOW);

	pinMode(pinNextTrack, INPUT);
	digitalWrite(pinNextTrack, HIGH);
	pinMode(pinPedometer, INPUT);
	digitalWrite(pinPedometer, HIGH);
	pinMode(debug, OUTPUT);

	setupTimersHeartMon();

	Serial.println("Ready to go!");
}

void loop() {

	readButtons();

	updateDisplay();

}

void readButtons() {
	if (pedometer.uniquePress()) {
		updateTempo();
	}
	if (nextTrack.uniquePress()) {
		changeTrack();
	}
	digitalWrite(debug, nextTrack.isPressed());
}

void updateTempo() {

}

void changeTrack() {
	currTrack++;
	if (currTrack == numTracks) {
		currTrack = 0;
	}
	Serial.print("Track changed: ");
	Serial.println(currTrack);
}

/* lcd display: 20 x 4
 * HR sliding bar display, HR numbers, steps, current track
 */

void updateDisplay() {
	lcd.setCursor(0, 3);
	lcd.print("Track: ");
	switch(currTrack) {
		case 0:
			lcd.print("Party Rock   ");
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

void setupTimersHeartMon() {
	TCCR1A = 0x00; // DISABLE OUTPUTS AND BREAK PWM ON DIGITAL PINS 9 & 10
	TCCR1B = 0x11; // GO INTO 'PHASE AND FREQUENCY CORRECT' MODE, NO PRESCALER
	TCCR1C = 0x00; // DON'T FORCE COMPARE
	TIMSK1 = 0x01; // ENABLE OVERFLOW INTERRUPT (TOIE1)
	ICR1 = 8000;   // TRIGGER TIMER INTERRUPT EVERY 1mS  
	sei();         // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}