// ATtiny I2C synth control requires Wire and Arduino library
#include "Arduino.h"	// for byte type
#include "Wire.h"		// for I2C messaging

union bytesFloat_t {
	float asFloat;
	byte asBytes[4];
} bytesFloat;

union bytesInt_t {
	int asInt;
	byte asBytes[2];
} bytesInt;

void synthOn(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('1');
	Wire.endTransmission();
}

void synthOff(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('0');
	Wire.endTransmission();
}

void synthLedOn(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('L');
	Wire.write(1);
	Wire.endTransmission();
}

void synthLedOff(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('L');
	Wire.write(0);
	Wire.endTransmission();
}

void synthNoiseOn(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('N');
	Wire.write(1);
	Wire.endTransmission();
}

void synthNoiseOff(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('N');
	Wire.write(0);
	Wire.endTransmission();
}

void synthAtariOn(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('N');
	Wire.write(1);
	Wire.endTransmission();
}

void synthAtariOff(byte addr) {
	Wire.beginTransmission(addr);
	Wire.write('N');
	Wire.write(0);
	Wire.endTransmission();
}

void synthSetFreq(byte addr, float freq) {
	byte msg[5] = {0};
	msg[0] = 'F';
	bytesFloat.asFloat = freq;
	for (int i = 0; i <= 3; i++) {
		msg[i + 1] = bytesFloat.asBytes[i];
	}
	Wire.beginTransmission(addr);
	for (int i = 0; i <= 4; i++) {
		Wire.write(msg[i]);
	}
	Wire.endTransmission();
}

void synthDoSweep(byte addr, float startFreq, float endFreq, int sweepStep, int sweepDelay) {
	byte msg[13] = {0};
	msg[0] = 'S';

	bytesFloat.asFloat = startFreq;
	for (int i = 0; i <= 3; i++) {
		msg[i + 1] = bytesFloat.asBytes[i];
	}
	bytesFloat.asFloat = endFreq;
	for (int i = 0; i <= 3; i++) {
		msg[i + 5] = bytesFloat.asBytes[i];
	}
	bytesInt.asInt = sweepStep;
	for (int i = 0; i <= 1; i++) {
		msg[i + 9] = bytesInt.asBytes[i];
	}
	bytesInt.asInt = sweepDelay;
	for (int i = 0; i <= 1; i++) {
		msg[i + 11] = bytesInt.asBytes[i];
	}

	Wire.beginTransmission(addr);
	for (int i = 0; i <= 12; i++) {
		Wire.write(msg[i]);
	}
	Wire.endTransmission();
}

void synthSetWaveform(byte addr, uint8_t waveform) {
	Wire.beginTransmission(addr);
	Wire.write(waveform);
	Wire.endTransmission();
}