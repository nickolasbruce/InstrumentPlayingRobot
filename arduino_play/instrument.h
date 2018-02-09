#ifndef INSTRUMENT_H
#define INSTRUMENT_H

class Instrument {
public:
	virtual void begin() = 0;
	virtual void play(int) = 0;

};

#endif


