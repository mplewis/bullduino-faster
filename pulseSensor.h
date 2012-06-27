#include "Arduino.h"	// for boolean type

/*--------------------*/
/* Pulse Sensor setup */
/*--------------------*/

// uses pulse sensor library and code from pulsesensor.com

// preset variables below. don't touch 'em!

long Hxv[4]; // these arrays are used in the digital filter
long Hyv[4]; // H for highpass, L for lowpass
long Lxv[4];
long Lyv[4];

unsigned long readings;			// used to help normalize the signal
unsigned long peakTime;			// used to time the start of the heart pulse
unsigned long lastPeakTime = 0;	// used to find the time between beats
volatile int Peak;				// used to locate the highest point in positive phase of heart beat waveform
int rate;						// used to help determine pulse rate
volatile int BPM;				// used to hold the pulse rate
int offset = 0;					// used to normalize the raw data
int sampleCounter;				// used to determine pulse timing
int beatCounter = 1;			// used to keep track of pulses
volatile int Signal;			// holds the incoming raw data
int NSignal;					// holds the normalized signal 
volatile int FSignal;			// holds result of the bandpass filter
volatile int HRV;				// holds the time between beats
volatile int Fade;				// used for dimming the pinFade LED
boolean first = true;			// reminds us to seed the filter on the first go
volatile boolean Pulse = false;	// becomes true when there is a heart pulse
volatile boolean B = false;		// becomes true when there is a heart pulse
volatile boolean QS = false;	// becomes true when pulse rate is determined. every 20 pulses

// user-set variables below
// analog input pin for Pulse Sensor
int pinPulse = 0;
// used to scale the result of the digital filter.
// range 12 ... 20: high ... low amplification
volatile int Scale = 20;

void setupTimersHeartMon() {
	TCCR1A = 0x00; // DISABLE OUTPUTS AND BREAK PWM ON DIGITAL PINS 9 & 10
	TCCR1B = 0x11; // GO INTO 'PHASE AND FREQUENCY CORRECT' MODE, NO PRESCALER
	TCCR1C = 0x00; // DON'T FORCE COMPARE
	TIMSK1 = 0x01; // ENABLE OVERFLOW INTERRUPT (TOIE1)
	ICR1 = 8000;   // TRIGGER TIMER INTERRUPT EVERY 1mS  
	sei();         // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}

/*--------------------------*/
/* Pulse Sensor Subroutines */
/*--------------------------*/

ISR(TIMER1_OVF_vect) { // triggered every time Timer 1 overflows

	// Timer 1 makes sure that we take a reading every milisecond
	Signal = analogRead(pinPulse);

	// First normailize the waveform around 0
	readings += Signal; // take a running total
	sampleCounter++;     // we do this every milisecond. this timer is used as a clock
	if ((sampleCounter % 300) == 0) {   // adjust as needed
		offset = readings / 300;        // average the running total
		readings = 0;                   // reset running total
	}
	NSignal = Signal - offset;        // normalizing here

	// IF IT'S THE FIRST TIME THROUGH THE SKETCH, SEED THE FILTER WITH CURRENT DATA
	if (first = true) { // FIXME assignment operator? really?
		for (int i=0; i<4; i++) {
			Lxv[i] = Lyv[i] = NSignal <<10;  // seed the lowpass filter
			Hxv[i] = Hyv[i] = NSignal <<10;  // seed the highpass filter
		}
		first = false;      // only seed once please
	}
	// THIS IS THE BANDPASS FILTER. GENERATED AT www-users.cs.york.ac.uk/~fisher/mkfilter/trad.html
	// BUTTERWORTH LOWPASS ORDER = 3; SAMPLERATE = 1mS; CORNER = 5Hz
			Lxv[0] = Lxv[1]; Lxv[1] = Lxv[2]; Lxv[2] = Lxv[3];
			Lxv[3] = NSignal<<10;    // insert the normalized data into the lowpass filter
			Lyv[0] = Lyv[1]; Lyv[1] = Lyv[2]; Lyv[2] = Lyv[3];
			Lyv[3] = (Lxv[0] + Lxv[3]) + 3 * (Lxv[1] + Lxv[2])
						+ (3846 * Lyv[0]) + (-11781 * Lyv[1]) + (12031 * Lyv[2]);
	// Butterworth; Highpass; Order = 3; Sample Rate = 1mS; Corner = .8Hz
			Hxv[0] = Hxv[1]; Hxv[1] = Hxv[2]; Hxv[2] = Hxv[3];
			Hxv[3] = Lyv[3] / 4116; // insert lowpass result into highpass filter
			Hyv[0] = Hyv[1]; Hyv[1] = Hyv[2]; Hyv[2] = Hyv[3];
			Hyv[3] = (Hxv[3]-Hxv[0]) + 3 * (Hxv[1] - Hxv[2])
						+ (8110 * Hyv[0]) + (-12206 * Hyv[1]) + (12031 * Hyv[2]);
	FSignal = Hyv[3] >> Scale;  // result of highpass shift-scaled

	//PLAY AROUND WITH THE SHIFT VALUE TO SCALE THE OUTPUT ~12 <> ~20 = High <> Low Amplification.

	if (FSignal >= Peak && Pulse == false) {	//heart beat causes ADC readings to surge down in value.  
		Peak = FSignal;							// finding the moment when the downward pulse starts
		peakTime = sampleCounter;				// record the time to derive HRV. 
	}
	//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
	if ((sampleCounter % 20) == 0) {				// only look for the beat every 20mS. This clears out a lot of high frequency noise.
		if (FSignal < 0 && Pulse == false) {	// signal surges down in value every time there is a pulse
			Pulse = true;						// Pulse will stay true as long as pulse signal < 0
			HRV = peakTime - lastPeakTime;		// measure time between beats
			lastPeakTime = peakTime;			// keep track of time for next pulse
			B = true;							// set the Quantified Self flag when HRV gets updated. NOT cleared inside this ISR     
			rate += HRV;						// add to the running total of HRV used to determine heart rate
			beatCounter++;						// beatCounter times when to calculate bpm by averaging the beat time values
			if (beatCounter == 10) {			// derive heart rate every 10 beats. adjust as needed
				rate /= beatCounter;			// averaging time between beats
				BPM = 60000/rate;				// how many beats can fit into a minute?
				beatCounter = 0;				// reset counter
				rate = 0;						// reset running total
				QS = true;						// set Beat flag when BPM gets updated. NOT cleared inside this ISR
			}
		}
		if (FSignal > 0 && Pulse == true) {		// when the values are going up, it's the time between beats
			Pulse = false;						// reset these variables so we can do it again!
			Peak = 0;
		}
	}
}