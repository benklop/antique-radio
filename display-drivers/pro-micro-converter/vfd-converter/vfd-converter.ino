#include "GU7000_Interface.h"
#include "GU7000_Serial_Async.h"
#include <SoftwareSerial.h>
#include "GU7000_SoftwareSerial.h"
#include "Noritake_VFD_GU7000.h"

//GU7000_Serial_Async interface(115200, 1, 0, 4); // BAUD RATE,SIN,BUSY,RESET
GU7000_SoftwareSerial interface(115200, 1, 0, 4);
Noritake_VFD_GU7000 vfd;

char c;

/* 140x16 image "westinghouse" */
static const uint8_t image_westinghouse[] = {
    0xff,0xff,0xff,0xff,0xfc,0x3f,0xf3,0xcf,0xe7,0xe7,0xef,0xf7,0xd3,0xfb,0xd1,0xfb,
    0xb2,0x7d,0xbf,0xb5,0xbf,0xd5,0x7f,0x36,0x72,0xf6,0x71,0xf6,0x71,0xf6,0x72,0xf6,
    0x7f,0x36,0xbf,0xd5,0xbf,0xb5,0xb2,0x7d,0xd1,0xfb,0xd3,0xfb,0xef,0xf7,0xe7,0xe7,
    0xf3,0xcf,0xfc,0x3f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xdf,0xff,0xc3,0xff,0xfc,0x3f,0xff,0xc7,0xff,0xc7,0xfc,0x3f,0xc3,0xff,
    0xc3,0xff,0xfc,0x3f,0xff,0xc7,0xff,0xc7,0xfc,0x3f,0xc3,0xff,0xdf,0xff,0xff,0xff,
    0xfe,0x1f,0xfd,0x6f,0xfb,0x77,0xfb,0x77,0xfb,0x77,0xfd,0x6f,0xfe,0x5f,0xff,0xff,
    0xff,0xff,0xfc,0xcf,0xfb,0x77,0xfb,0x77,0xfb,0x77,0xfb,0xb7,0xfb,0xcf,0xfb,0xff,
    0xfb,0xff,0xfb,0xff,0xc0,0x0f,0xfb,0xf7,0xfb,0xf7,0xfb,0xf7,0xff,0xff,0x98,0x07,
    0xff,0xff,0xff,0xff,0xff,0xff,0xf8,0x07,0xfc,0xff,0xfb,0xff,0xfb,0xff,0xfb,0xff,
    0xfd,0xff,0xfe,0x07,0xff,0xff,0xff,0xff,0xfe,0x1d,0xfd,0xee,0xfb,0xf6,0xfb,0xf6,
    0xfb,0xf6,0xfd,0xed,0xf8,0x03,0xff,0xff,0xff,0xff,0x80,0x07,0xfc,0xff,0xfb,0xff,
    0xfb,0xff,0xfb,0xff,0xfd,0xff,0xfe,0x07,0xff,0xff,0xff,0xff,0xfe,0x1f,0xfd,0xef,
    0xfb,0xf7,0xfb,0xf7,0xfb,0xf7,0xfd,0xef,0xfe,0x1f,0xff,0xff,0xff,0xff,0xf8,0x0f,
    0xff,0xe7,0xff,0xf7,0xff,0xf7,0xff,0xf7,0xff,0xef,0xf8,0x07,0xff,0xff,0xff,0xff,
    0xfc,0xcf,0xfb,0x77,0xfb,0x77,0xfb,0x77,0xfb,0xb7,0xfd,0xcf,0xff,0xff,0xff,0xff,
    0xfe,0x1f,0xfd,0x6f,0xfb,0x77,0xfb,0x77,0xfb,0x77,0xfd,0x6f,0xfe,0x5f,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};

void setup() {
/*  _delay_ms(500);           // wait for device to power up
  vfd.begin(140, 16);       // 140x16 module
  vfd.interface(interface); // select which interface to use
  // Enter the 4-digit model class number
  // E.g. 7040 for GU140X16G-7040A
  vfd.isModelClass(7002);
  //vfd.isGeneration('J');
  vfd.GU7000_reset();       // reset module
  vfd.GU7000_init();        // initialize module
  
  // Display image
  vfd.GU7000_drawImage_p(140, 16, image_westinghouse);
*/
}


void loop() {
  
  // copy from virtual serial line to uart and vice versa
  if (Serial.available()) {
    c = (char)Serial.read();
    Serial1.write(c);
  }
  
}
