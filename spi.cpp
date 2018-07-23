#include "config.h"

#if NORITAKE_VFD_GENERATION == 'B' && NORITAKE_VFD_INTERFACE == 0 && NORITAKE_VFD_SERIAL == 2

#include <stddef.h>
#include <avr/io.h>
#include <util/delay.h>
#include "interface.h"

void initPort() {
    RAISE(RESET);
    RAISE(OUT);
    RAISE(SCK);
    RAISE(CS);
    DIRECTION(OUT,1);
    DIRECTION(SCK,1);
    DIRECTION(CS,1);
    DIRECTION(RESET,1);
    DIRECTION(BUSY,0);
}

void writePort(uint8_t data) {
    while (CHECK(BUSY));
    LOWER(CS);
	for (uint8_t i=128; i; i >>= 1) {
		LOWER(SCK);
		_delay_us(1);
		if (data & i) RAISE(OUT); else LOWER(OUT);
		RAISE(SCK);
		_delay_us(1);
	}
	_delay_us(17);
    RAISE(CS);
}

void hardReset() {
    initPort();
    LOWER(RESET);
    _delay_ms(2);
    RAISE(RESET);
    _delay_ms(120);
}

#endif
