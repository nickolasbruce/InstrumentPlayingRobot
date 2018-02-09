#ifndef PHOTONICWHISTLE_H
#define PHOTONICWHISTLE_H

#include <Servo.h>
#include "instrument.h"

const int PHOT_PROP = 0.25;
const int PHOT_DERIV = 0.1;

class PhotonicWhistle : public Instrument {
public:
	PhotonicWhistle(int);
	virtual void begin();
	virtual void play(int);
	inline int light_level() {
		return analogRead(pin);
	}
	inline float rvalue() {
		int sensorValue = light_level();
		return (float)(1023 - sensorValue) * 10 / sensorValue;
	}

private:
	int pin;
	int pos_low;
	int pos_high;
	int map[13];
	Servo motor;
};

PhotonicWhistle::PhotonicWhistle(int attach) {
	this.pin = attach;
	this.motor.attach(this.pin);
	this.pos_high = 0;
	this.pos_low = 0;
}

void PhotonicWhistle::begin() {
	// calibration
	int position = 0;
	unsigned long time;
	int level_high = this.light_level();
	int level_low = this.light_level(); // light level boundaries
	int level;

	// try various positions
	while (position <= 180) {
		this.motor.write(position);
		time = millis();

		// get light level at this position for a second
		while (millis() < time + 1000) {
			level = this.light_level();
			if (level > level_high) {
				level_high = level;
				this.pos_high = position;
			}
			if (level < level_low) {
				level_low = level;
				this.pos_low = position;
			}
		}
		position += 45;
	}

	// start at position away from sensor
	this.motor.write(pos_high);

	// map light levels to notes
	float step = (level_high - level_low) / 13;
	int map_index = 0;
	for (int i=level_low; i<level_high; i+=step) {
		this.map[map_index] = i;
		map_index++
	}
}

void PhotonicWhistle::play(int key) {
	float freq;
	int target;
	switch(key) {
		case 3:  freq = 1046.50; target = this.map[0];  break; // C6
		case 22: freq = 1108.73; target = this.map[1];  break; // C#6
		case 4:  freq = 1174.66; target = this.map[2];  break; // D6
		case 23: freq = 1244.51; target = this.map[3];  break; // D#6
		case 5:  freq = 1318.51; target = this.map[4];  break; // E6
		case 6:  freq = 1396.91; target = this.map[5];  break; // F6
		case 25: freq = 1479.98; target = this.map[6];  break; // F#6
		case 7:  freq = 1567.98; target = this.map[7];  break; // G6
		case 26: freq = 1661.22; target = this.map[8];  break; // G#6
		case 1:  freq = 1760.00; target = this.map[9];  break; // A6
		case 20: freq = 1864.66; target = this.map[10]; break; // A#6
		case 2:  freq = 1975.53; target = this.map[11]; break; // B
		case 11: freq = 2093.00; target = this.map[12]; break; // C7
		default:
			println("Invalid input.");
	}

	// approximate calibrated range in light levels
	int range = this.map[12] - this.map[0];
	int step = this.map[1] - this.map[0];
	int current_level;
	float target_proportion;
	float current_proportion;
	float error = 1;
	float derror = 0;
	int pos_range;
	int movement;

	while (error >= 0.077) { // normalized step size
		current_level = this.light_level();

		// normalized levels
		target_proportion = (float) (target-map[0]) / range;
		current_proportion = (float) (target-map[0]) / range;
		error = target_proportion - current_proportion;
		if (error < 0.9) derror = error;

		// move motor as a proportion of level
		pos_range = this.pos_high - this.pow_low;
		movement = pos_range * (error * PHOT_PROP + derror * PHOT_DERIV);
		this.motor.write(this.motor.read() + movement);
		delay(50);
	}

}

#endif