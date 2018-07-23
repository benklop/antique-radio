#include "config.h"
#if NORITAKE_VFD_INTERFACE==1

#include <stddef.h>
#include <avr/io.h>
#include <util/delay.h>
#include "interface.h"

void initPort() {
    RAISE(WR);
    RAISE(RD);
    DIRECTION(BUSY, 0);    
    DIRECTION(WR, 1);
    DIRECTION(RD, 1);
    DIRECTION(D0, 1);
    DIRECTION(D1, 1);
    DIRECTION(D2, 1);
    DIRECTION(D3, 1);
    DIRECTION(D4, 1);
    DIRECTION(D5, 1);
    DIRECTION(D6, 1);
    DIRECTION(D7, 1);
}

void writePort(uint8_t data) {
    #if NORITAKE_VFD_BUSY_CONNECTED==1
        while (CHECK(BUSY));
    #else
        bool ok;
        DIRECTION(D7,0);
        do {
            LOWER(RD);        
            _delay_us(.08);
            ok = CHECK(D7);
            RAISE(RD);
        } while (ok);
        DIRECTION(D7,1);        
    #endif

    if (data & 0x01) RAISE(D0); else LOWER(D0);
    if (data & 0x02) RAISE(D1); else LOWER(D1);
    if (data & 0x04) RAISE(D2); else LOWER(D2);
    if (data & 0x08) RAISE(D3); else LOWER(D3);
    if (data & 0x10) RAISE(D4); else LOWER(D4);
    if (data & 0x20) RAISE(D5); else LOWER(D5);
    if (data & 0x40) RAISE(D6); else LOWER(D6);
    if (data & 0x80) RAISE(D7); else LOWER(D7);
    LOWER(WR);
    _delay_us(0.11);
    RAISE(WR);
    
    _delay_us(1.6);
}

void hardReset() {
    initPort();
    #if NORITAKE_VFD_RESET_CONNECTED==1
        DIRECTION(RESET,1);
        LOWER(RESET);
        _delay_ms(1.2);
        RAISE(RESET);
        _delay_ms(100);
    #endif
}

#endif
