#include <stdlib.h>
#include <string.h>
#include <alloca.h>

#include "config.h"
#include "Noritake_VFD_GU7000.h"
#include "interface.h"

void Noritake_VFD_GU7000::GU7000_back() {
    command(0x08);
}

void Noritake_VFD_GU7000::GU7000_forward() {
    command(0x09);
}

void Noritake_VFD_GU7000::GU7000_lineFeed() {
    command(0x0a);
}

void Noritake_VFD_GU7000::GU7000_home() {
    command(0x0b);
}

void Noritake_VFD_GU7000::GU7000_carriageReturn() {
    command(0x0d);
}

void Noritake_VFD_GU7000::GU7000_setCursor(unsigned x, unsigned y) {
    command(0x1f);
    command('$');
    command_xy(x, y);
}

void Noritake_VFD_GU7000::GU7000_clearScreen() {
    command(0x0c);
}

void Noritake_VFD_GU7000::GU7000_cursorOn() {
    command(0x1f, 'C', 1);
}

void Noritake_VFD_GU7000::GU7000_cursorOff() {
    command(0x1f, 'C', 0);
}

void Noritake_VFD_GU7000::GU7000_init() {
    _delay_ms(NORITAKE_VFD_RESET_DELAY);
    initPort();
    command(0x1b);
    command('@');
}

void Noritake_VFD_GU7000::GU7000_reset() {
    hardReset();
}

void Noritake_VFD_GU7000::GU7000_useMultibyteChars(bool enable) {
    #if (NORITAKE_VFD_MODEL_CLASS-7000)/100==9 || (NORITAKE_VFD_MODEL_CLASS-7000)/100==1
        us_command('g', 0x02);
        command(enable);
    #endif
}

void Noritake_VFD_GU7000::GU7000_setMultibyteCharset(uint8_t code) {
    #if (NORITAKE_VFD_MODEL_CLASS-7000)/100==9 || (NORITAKE_VFD_MODEL_CLASS-7000)/100==1
        us_command('g', 0x0f);
        command(code);
    #endif
}

void Noritake_VFD_GU7000::GU7000_useCustomChars(bool enable) {
    command(0x1b, '%', enable);
}

static inline uint8_t getColumn(const uint8_t *src, int col) {
    uint8_t out = 0;
    for (int i=0; i<8; i++)
        if (src[i] & (1<<(4-col))) out += 1<<(7-i);
    return out;
}

void Noritake_VFD_GU7000::GU7000_defineCustomChar(uint8_t code, FontFormat format, const uint8_t *data) {
    command(0x1b, '&', 0x01);
    command(code);
    command(code);

    switch (format) {
    case CUUFormat:
        command(5);
        for (uint8_t i=0; i<5; i++)
            command(getColumn(data, i));
        break;

    case GU70005x7Format:
        command(5);
        print((const char*)data, 5);
        break;
    case GU70007x8Format:
        command(7);
        print((const char*)data, 7);
        break;
    }
}

void Noritake_VFD_GU7000::GU7000_deleteCustomChar(uint8_t code) {
    command(0x01b, '?', 0x01);
    command(code);
}

void Noritake_VFD_GU7000::GU7000_setAsciiVariant(AsciiVariant code) {
    command(0x1b, 'R', code);
}

void Noritake_VFD_GU7000::GU7000_setCharset(Charset code) {
    command(0x1b, 't', code);
}

void Noritake_VFD_GU7000::GU7000_setScrollMode(ScrollMode mode) {
    command(0x1f);
    command(mode);
}

void Noritake_VFD_GU7000::GU7000_setHorizScrollSpeed(uint8_t speed) {
    command(0x1f, 's', speed);
}

void Noritake_VFD_GU7000::GU7000_invertOff() {
    command(0x1f, 'r', 0);
}

void Noritake_VFD_GU7000::GU7000_invertOn() {
    command(0x1f, 'r', 1);
}

void Noritake_VFD_GU7000::GU7000_setCompositionMode(CompositionMode mode) {
    command(0x1f, 'w', mode);
}

void Noritake_VFD_GU7000::GU7000_setScreenBrightness(unsigned level) {
    if (level == 0)
        GU7000_displayOff();
    else if (level <= 100) {
        GU7000_displayOn();
        command(0x1f, 'X', (level*10 + 120)/125);
    }
}

void Noritake_VFD_GU7000::GU7000_wait(uint8_t time) {
    us_command('a', 0x01);
    command(time);
}

void Noritake_VFD_GU7000::GU7000_scrollScreen(unsigned x, unsigned y, unsigned times, uint8_t speed) {
    unsigned pos = (x*NORITAKE_VFD_LINES)+(y/8);
    us_command('a', 0x10);
    command(pos);
    command(pos>>8);
    command(times);
    command(times>>8);
    command(speed);
}

void Noritake_VFD_GU7000::GU7000_blinkScreen() {
    us_command('a', 0x11);
    command(0);
    command(0);
    command(0);
    command(0);
}

void Noritake_VFD_GU7000::GU7000_blinkScreen(bool enable, bool reverse, uint8_t onTime, uint8_t offTime, uint8_t times) {
    us_command('a', 0x11);
    command(enable? (reverse? 2: 1): 0);
    command(onTime);
    command(offTime);
    command(times);
}

void Noritake_VFD_GU7000::GU7000_displayOff() {
    us_command('a', 0x40);
    command(0);
}

void Noritake_VFD_GU7000::GU7000_displayOn() {
    us_command('a', 0x40);
    command(0x01);
}

void Noritake_VFD_GU7000::GU7000_screenSaver(ScreenSaver mode) {
    us_command('a', 0x40);
    command(mode);
}

void Noritake_VFD_GU7000::GU7000_setFontStyle(bool proportional, bool evenSpacing) {
    us_command('g', 0x03);
    command(proportional*2 + evenSpacing);
}

void Noritake_VFD_GU7000::GU7000_setFontSize(uint8_t x, uint8_t y, bool tall) {
    if (x<=4 && y<=2) {
        us_command('g', 0x40);
        command(x);
        command(y);
        #if (NORITAKE_VFD_MODEL_CLASS-7000)/100==9 || (NORITAKE_VFD_MODEL_CLASS-7000)/100==1
            us_command('g', 0x01);
            command(tall+1);
        #endif
    }
}

void Noritake_VFD_GU7000::GU7000_selectWindow(uint8_t window) {
    if (window <= 4)
        command(0x10 + window);
}

void Noritake_VFD_GU7000::GU7000_defineWindow(uint8_t window, unsigned x, unsigned y, unsigned width, unsigned height) {
    us_command('w', 0x02);
    command(window);
    command(0x01);
    command_xy(x, y);
    command_xy(width, height);
}

void Noritake_VFD_GU7000::GU7000_deleteWindow(uint8_t window) {
    us_command('w', 0x02);
    command(window);
    command(0);
    command_xy(0, 0);
    command_xy(0, 0);
}

void Noritake_VFD_GU7000::GU7000_joinScreens() {
    us_command('w', 0x10);
    command(0x01);
}

void Noritake_VFD_GU7000::GU7000_separateScreens() {
    us_command('w', 0x10);
    command(0);
}

void Noritake_VFD_GU7000::print(char c) {
    command(c);
}

void Noritake_VFD_GU7000::print(const char *str) {
    while (*str)
        writePort(*str++);
}

void Noritake_VFD_GU7000::print(const char *buffer, size_t size) {
    while (size--)
        print(*buffer++);
}

void Noritake_VFD_GU7000::print(long number, uint8_t base) {
    if (number < 0) {
        print('-');
        number = -number;
    }
    printNumber(number, base);
}

void Noritake_VFD_GU7000::print(int number, uint8_t base) {
    print((long)number, base);
}

void Noritake_VFD_GU7000::print(unsigned long number, uint8_t base) {
    printNumber(number, base);
}

void Noritake_VFD_GU7000::print(unsigned number, uint8_t base) {
    print((unsigned long)number, base);
}

void Noritake_VFD_GU7000::crlf() {
    GU7000_carriageReturn();
    GU7000_lineFeed();
}

void Noritake_VFD_GU7000::println(char c) {
    print(c);
    crlf();
}

void Noritake_VFD_GU7000::println(const char *str) {
    print(str);
    crlf();
}

void Noritake_VFD_GU7000::println(const char *buffer, size_t size) {
    print(buffer, size);
    crlf();
}

void Noritake_VFD_GU7000::println(long number, uint8_t base) {
    print(number, base);
    crlf();
}

void Noritake_VFD_GU7000::println(int number, uint8_t base) {
    println((long) number, base);
}

void Noritake_VFD_GU7000::println(unsigned long number, uint8_t base) {
    print(number, base);
    crlf();
}

void Noritake_VFD_GU7000::println(unsigned number, uint8_t base) {
    println((unsigned long) number, base);
}

void Noritake_VFD_GU7000::printNumber(unsigned long number, uint8_t base) {
    if (number/base)
        printNumber(number/base, base);
    number %= base;
    print(number + (number < 10? '0': 'A' - 10));
}

void Noritake_VFD_GU7000::GU7000_setBacklightColor(uint8_t r, uint8_t g, uint8_t b) {
    #if NORITAKE_VFD_MODEL_CLASS==7040
        command(0x1f, 'L', 0x10);
        command(b & 0xf0);
        command(g & 0xf0);
        command(r & 0xf0);
    #endif
}

void Noritake_VFD_GU7000::GU7000_setBacklightColor(unsigned rgb) {
    GU7000_setBacklightColor((rgb>>8 & 0x0f)*16, (rgb>>4 & 0x0f)*16, (rgb & 0x0f)*16);
}

void Noritake_VFD_GU7000::GU7000_drawImage(unsigned width, uint8_t height, const uint8_t *data) {
    if (height > NORITAKE_VFD_HEIGHT) return;
    us_command('f', 0x11);
    command_xy(width, height);
    command((uint8_t) 1);
    for (unsigned i = 0; i<(height/8)*width; i++)
        command(data[i]);
}

void Noritake_VFD_GU7000::GU7000_drawFROMImage(unsigned long address, uint8_t srcHeight, unsigned width, uint8_t height) {
    #if (NORITAKE_VFD_MODEL_CLASS-7000)/100==9 || (NORITAKE_VFD_MODEL_CLASS-7000)/100==1
        if (height > NORITAKE_VFD_HEIGHT) return;
        us_command('f', 0x10);
        command(0x01);
        command(address);
        command(address>>8);
        command(address>>16);
        command(srcHeight/8);
        command((srcHeight/8)>>8);
        command_xy(width, height);
        command((uint8_t) 1);
    #endif
}

static unsigned min(unsigned x, unsigned y) { return x<y? x: y; }

void Noritake_VFD_GU7000::GU7000_fillRect(unsigned x0, unsigned y0, unsigned x1, unsigned y1, bool on) {
    x0 = min(NORITAKE_VFD_WIDTH, x0);
    x1 = min(NORITAKE_VFD_WIDTH, x1);
    y0 = min(NORITAKE_VFD_HEIGHT, y0);
    y1 = min(NORITAKE_VFD_HEIGHT, y1);
    if (y1<=y0 || x1<=x0) return;
    uint8_t bufw = 8, bufh = (y1-y0+7)/8*8;
    uint8_t *buf = (uint8_t*)alloca(bufh/8 * bufw);
    for (unsigned x = 0; x < x1-x0; x += bufw) {
        uint8_t part = (x + bufw < x1-x0)? bufw: (x1-x0) - x;
        memset(buf, 0, bufh/8 * bufw);
        if (on)
            for (uint8_t col = 0; col < part; col++) {
                for (uint8_t py = y0 % 8; py < y0 % 8 + min(y1-y0, 8); py++)
                    buf[col*bufh/8] |= 1 << (7-py);
                for (uint8_t row = (y0+7)/8; row < y1/8; row++)
                    buf[row - y0/8 + col*bufh/8] = 0xff;
                if (y0/8 != y1/8)
                    for (uint8_t py = 0; py < y1 % 8; py++)
                        buf[(y1-y0)/8 + col*bufh/8] |= 1 << (7-py);
            }
        GU7000_setCursor(x + x0, y0);
        GU7000_drawImage(bufw, bufh, buf);
    }
}

void Noritake_VFD_GU7000::command(uint8_t data) {
    writePort(data);
}
void Noritake_VFD_GU7000::command_xy(unsigned x, unsigned y) {
    command(x);
    command(x>>8);
    y /= 8;
    command(y);
    command(y>>8);
}
void Noritake_VFD_GU7000::command_xy1(unsigned x, unsigned y) {
    command(x);
    command(x>>8);
    command(y);
    command(y>>8);
}

void Noritake_VFD_GU7000::us_command(uint8_t group, uint8_t cmd) {
   command(0x1f);
   command(0x28);
   command(group);
   command(cmd);
}

void Noritake_VFD_GU7000::command(uint8_t prefix, uint8_t group, uint8_t cmd) {
   command(prefix);
   command(group);
   command(cmd);
}

void Noritake_VFD_GU7000::print(unsigned x, uint8_t y, const char *buffer, uint8_t len) {
    #if NORITAKE_VFD_GENERATION == 'B'
        us_command('d', 0x30);
        command_xy1(x, y);
        command(0);
        command(len);
        while (len--)
            command(*buffer++);
    #endif
}

void Noritake_VFD_GU7000::print(unsigned x, uint8_t y, const char *str) {
    #if NORITAKE_VFD_GENERATION == 'B'
        print(x, y, str, strlen(str));
    #endif
}
void Noritake_VFD_GU7000::print(unsigned x, uint8_t y, char c) {
    #if NORITAKE_VFD_GENERATION == 'B'
        print(x, y, &c, 1);
    #endif
}
void Noritake_VFD_GU7000::print(unsigned x, uint8_t y, int number, uint8_t base) {
    #if NORITAKE_VFD_GENERATION == 'B'
        if (number < 0) {
            print(x, y, '-');
            print(-1, y, (unsigned)-number, base);
        } else
            print(x, y, (unsigned)number, base);
    #endif
}
void Noritake_VFD_GU7000::print(unsigned x, uint8_t y, unsigned number, uint8_t base) {
    #if NORITAKE_VFD_GENERATION == 'B'
        char buf[16], *p = buf + sizeof buf;
        do
            *--p = number % base + (number % base < 10? '0': 'A' - 10);
        while (number /= base);
        print(x, y, p, buf + sizeof buf - p);
    #endif
}
void Noritake_VFD_GU7000::GU7000_drawImage(unsigned x, uint8_t y, unsigned width, uint8_t height, const uint8_t *data) {
    #if NORITAKE_VFD_GENERATION == 'B'
        us_command('d', 0x21);
        command_xy1(x, y);
        command_xy1(width, height);
        command(0x01);
        for (unsigned i = 0; i<(height/8)*width; i++)
            command(data[i]);
    #endif
}
void Noritake_VFD_GU7000::GU7000_drawImage(unsigned x, uint8_t y, ImageMemoryArea area, unsigned long address, uint8_t srcHeight, unsigned width, uint8_t height, unsigned offsetx, unsigned offsety) {
    #if NORITAKE_VFD_GENERATION == 'B'
        if (height > NORITAKE_VFD_HEIGHT) return;
        us_command('d', 0x20);
        command_xy1(x, y);
        command(area);
        command(address);
        command(address>>8);
        command(address>>16);
        command(srcHeight/8);
        command(srcHeight/8>>8);
        command_xy1(offsetx, offsety);
        command_xy1(width, height);
        command(0x01);
    #endif
}

void Noritake_VFD_GU7000::GU7000_drawImage(unsigned x, uint8_t y, ImageMemoryArea area, unsigned long address, unsigned width, uint8_t height) {
    #if NORITAKE_VFD_GENERATION == 'B'
        GU7000_drawImage(x, y, area, address, (height + 7) & ~7, width, height, 0, 0);
    #endif
}

void Noritake_VFD_GU7000::print_p(const char *str) {
    while (pgm_read_byte(str))
        writePort(pgm_read_byte(str++));
}
void Noritake_VFD_GU7000::print_p(unsigned x, uint8_t y, const char *buffer, uint8_t len) {
    #if NORITAKE_VFD_GENERATION == 'B'
        us_command('d', 0x30);
        command_xy1(x, y);
        command(0);
        command(len);
        while (len--)
            command(pgm_read_byte(buffer++));
    #endif
}

void Noritake_VFD_GU7000::print_p(unsigned x, uint8_t y, const char *str) {
    #if NORITAKE_VFD_GENERATION == 'B'
        const char *end = str;
        while (pgm_read_byte(end)) end++;
        print_p(x, y, str, end - str);
    #endif
}

void Noritake_VFD_GU7000::GU7000_drawImage_p(unsigned width, uint8_t height, const uint8_t *data) {
    if (height > NORITAKE_VFD_HEIGHT) return;
    us_command('f', 0x11);
    command_xy(width, height);
    command((uint8_t) 1);
    for (unsigned i = 0; i<(height/8)*width; i++)
        command(pgm_read_byte(data+i));
}

void Noritake_VFD_GU7000::GU7000_drawImage_p(unsigned x, uint8_t y, unsigned width, uint8_t height, const uint8_t *data) {
    #if NORITAKE_VFD_GENERATION == 'B'
        us_command('d', 0x21);
        command_xy1(x, y);
        command_xy1(width, height);
        command(0x01);
        for (unsigned i = 0; i<(height/8)*width; i++)
            command(pgm_read_byte(data+i));
    #endif
}

void Noritake_VFD_GU7000::buffer_wait() {
    while ( !bufferEmpty() ) {
      _delay_us(10);
    }
}
