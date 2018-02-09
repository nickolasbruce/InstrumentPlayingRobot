#ifndef RUBBERROCKBAND_H
#define RUBBERROCKBAND_H

#include <Servo.h>
#include "instrument.h"

const int RUBB_PROP = 0.25;
const int RUBB_DERIV = 0.1;


class RubberRockBand : public Instrument {
public:
    RubberRockBand(int);
    ~RubberRockBand();
    virtual void begin();
    virtual void play(int);
    inline void stop_motor() {
        motor.write(90);
    }
    long loudness(int);
    void spindown();
    long test_freq();

private:
    int pin;
    int vel_low;
    int vel_high;
    int map[5];
    long avg;
    Servo motor;
};


RubberRockBand::RubberRockBand(int attach) {
    this.pin = attach;
    this.motor.attach(this.pin);
    this.vel_high = 180;
    this.vel_low = 130;
}

RubberRockBand::~RubberRockBand() {
    // protect from motor wear
    this.spindown();
}

void RubberRockBand::begin() {
    //calibration
    int velocity = 130;

    // spin up
    this.motor.write(130);
    delay(500);

    float average_loud = 0;
    long sum_loud = 0;
    long loud;
    int count = 0;
    int samples_between = 0;
    int struck_count = 0;
    int map_count = 0;
    unsigned long time;
    this.avg = 0;

    // map lowest and highest speeds
    while (velocity <= 180) {
        this.motor.write(velocity);
        time = millis();

        // get sound at this position for a 4 seconds
        while (millis() < time + 4000) {
            count++;
            loud = this.loudness();
            sum_loud += loud;
            average_loud = sum_loud / count;
            if (loud < average_loud) {
                samples_between++;
            } else {
                struck_count++;
            }
        }

        // average silence
        this.map[map_count] = samples_between / ((float)(struck_count));

        map_count += 5;
        velocity += 50;
        sum_loud = 0;
        this.avg += average_loud;
        average_loud = 0;
    }

    this.avg /= 2;

    // interpolate speeds
    int step = (map[4] - map[0]) / 5;
    for (int i=map[0], i<map[4]; i+=step) {
        map[i] = i;
    }

    // spin down
    this.spindown();
}

void RubberRockBand::spindown() {
    current = this.motor.read();
    for (int i=current; i>90; i-=20) {
        if (i < 120) i = 90;
        this.motor.write(i);
        delay(200);
    }
    this.stop_motor();
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

long RubberRockBand::test_freq() {
    // test frequency
    int samples_between = 0;
    while (this.loudness() < this.avg) {}
    while (this.loudness() < this.avg) {
        samples_between++;
    }
    return samples_between;
}

void RubberRockBand::play(int key) {
    float freq;
    int target;
    switch(key) {
        case 3:  target = this.map[0];  break; // C
        case 4:  target = this.map[1];  break; // D
        case 5:  target = this.map[2];  break; // E
        case 6:  target = this.map[3];  break; // F
        case 7:  target = this.map[4];  break; // G
        default:
            println("Invalid input.");
    }



    // approximate calibrated range in strike frequency
    int range = this.map[4] - this.map[0];
    int step = this.map[1] - this.map[0];
    int current_level;
    float target_proportion;
    float current_proportion;
    float error = 1;
    float derror = 0;
    int vel_range;
    int movement;

    while (error >= 0.20) { // normalized step size
        current_level = this.test_freq();

        // normalized levels
        target_proportion = (float) (target-map[0]) / range;
        current_proportion = (float) (current_level-map[0]) / range;
        error = target_proportion - current_proportion;
        if (error < 0.9) derror = error;

        // move motor as a proportion of level
        vel_range = this.vel_high - this.vel_low;
        movement = vel_range * (error * RUBB_PROP + derror * RUBB_DERIV);
        this.motor.write(this.motor.read() + movement);
        delay(50);
    }    
}

#endif