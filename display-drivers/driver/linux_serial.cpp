#include <stdint.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <unistd.h>

#include <wiringPi.h>
#include <queue>

#include "config.h"
static volatile int vfd;
static volatile queue<uint8_t> buffer;

void ISRWritePort() {
	if(buffer.size()) //if there is data to write
  	    write(vfd, buffer.pop_front(), 1); //pop it off and write it
}

void writePort(uint8_t data) {
	buffer.push_back(data);
	if(digitalRead(16) == LOW && buffer.size()) //if the on-display buffer is empty but we have data in our queue still
	    ISRWritePort(); //trigger the ISR since there won't be a falling edge
}

void initPort() {
	#if NORITAKE_VFD_BAUD == 9600
		#define baud B9600
	#elif NORITAKE_VFD_BAUD == 19200
		#define baud B19200
	#elif NORITAKE_VFD_BAUD == 38400
		#define baud B38400
	#elif NORITAKE_VFD_BAUD == 115200
		#define baud B115200
	#else
		#error "Illegal baud rate selected"
	#endif
	
	struct termios t;
	static const char *fn = NORITAKE_VFD_FILE;

	wiringPiSetup();

	vfd = open(fn, O_RDWR | O_NOCTTY);
	if (vfd < 0)
		return;
	
	tcgetattr(vfd, &t);
	t.c_cflag &= ~CSIZE; // clear character size mask
	t.c_cflag |= CS8 | CLOCAL | CREAD | CRTSCTS; // 8n1, non-modem, read-enabled, hardware flow control
	#if NORITAKE_VFD_STOP==2
		t.c_cflag |= CSTOPB; // 2 stop bits
	#endif
	t.c_iflag = IGNPAR; // ignore parity errors
	t.c_oflag = 0; // raw output
	t.c_lflag = 0; // non-canonical; no echo
	t.c_cc[VTIME] = 1; // 1 * 0.1s between characters times out
	t.c_cc[VMIN] = 0; // minimum number of characters from fread
	cfsetospeed(&t, baud);
	cfsetispeed(&t, baud);
	tcsetattr(vfd, TCSANOW, &t);

	//set up the rx pin using wiringpi to receive buffer status
	pinMode(16, INPUT);
	pullUpDnControl(16, PUD_OFF);
	wiringPiISR(16, INT_EDGE_FALLING, &ISRWritePort);
}

void hardReset() {
}

