#ifndef RUBBERROCKBAND_H
#define RUBBERROCKBAND_H

#include <Servo.h>
#include "instrument.h"

const int RUBB_PROP = 0.25;
const int RUBB_DERIV = 0.1;


class RubberRockBand : public Instrument {
public:
    RubberRockBand(int);
    virtual void begin();
    virtual void play(int);
    inline void stop_motor() {
        motor.write(90);
    }
    long loudness(int);

private:
    int pin;
    int vel_low;
    int vel_high;
    int map[5];
    Servo motor;
};


RubberRockBand::RubberRockBand(int attach) {
    this.pin = attach;
    this.motor.attach(this.pin);
    this.vel_high = 100;
    this.vel_low = 50;
}

void RubberRockBand::begin() {
    //calibration
    
}

long RubberRockBand::loudness() {
    // average loudness over 32 samples
    long sum = 0;
    for(int i=0; i<samples; i++)
    {
        sum += analogRead(pin);
    }

    sum >>= 5;

    return sum;
}

void RubberRockBand::play(int key) {
    float freq;
    int target;
    switch(key) {
        case 3:  freq = 1046.50; target = this.map[0];  break; // C
        case 4:  freq = 1174.66; target = this.map[2];  break; // D
        case 5:  freq = 1318.51; target = this.map[4];  break; // E
        case 6:  freq = 1396.91; target = this.map[5];  break; // F
        case 7:  freq = 1567.98; target = this.map[7];  break; // G
        default:
            println("Invalid input.");
    }
}

#endif