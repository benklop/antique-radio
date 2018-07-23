// CPU frequency in Hertz
#define F_CPU                           16000000UL

#ifdef __AVR
	#include <avr/pgmspace.h>
	#include <util/delay.h>
#else
	#include <unistd.h>
	#define _delay_us(x)	usleep(x)
	#define _delay_ms(x)	usleep(x*1000)
	#define pgm_read_byte(p) (*(uint8_t*)p)
#endif

#define NORITAKE_VFD_WIDTH              140
#define NORITAKE_VFD_HEIGHT             16
#define NORITAKE_VFD_LINES              (NORITAKE_VFD_HEIGHT/8)

//Different classes have different features. In order for the
//library to use these capabilities, the correct class number
//must be given.
//The model class number is the 4 digits after the dash in the
//model number:
//Example
//    Module                Class Number
//    GU140X32F-7000        7000
//    GU140X16G-7040A       7040
//    GU140X16G-7903        7903
#define NORITAKE_VFD_MODEL_CLASS        7002

//The generation of the GU-7000 series module. This is the last
//letter on the model number if present. If this letter is not
//present or 'A', the generation is 0.
//Example
//    Module                Generation
//    GU140X16G-7003           0
//    GU140X16G-7040A          0
//    GU140X16G-7003B         'B'
#define NORITAKE_VFD_GENERATION         '0'

//Delay time between the Atmel starting and the VFD module being
//initialized. This is necessary to allow the module's controller
//to initialize. This value will vary depend on the power supply
//and hardware setup. 500 ms is more than enough time for the
//module to start up. This delay will only occur the first time
//the CUY_init() or CUY_reset() method is called.
#define NORITAKE_VFD_RESET_DELAY        500

// SELECT THE INTERFACE TO THE VFD MODULE
// NORITAKE_VFD_INTERFACE:
//  0 = Serial
//  1 = Parallel
//  2 = Linux serial device interface
#define NORITAKE_VFD_INTERFACE          2

// SELECT SERIAL INTERFACE MODE
//  0 = Asynchronous
//  1 = Synchronous
//  2 = SPI (Serial Peripheral Interface)
#define NORITAKE_VFD_SERIAL             0

#if NORITAKE_VFD_INTERFACE==0
    #if NORITAKE_VFD_SERIAL==0
        // SELECT RS-232 OR CMOS FOR SERIAL INTERFACE
        // Please check the documentation for the target module to determine
        // the signal levels.
        // 0 = CMOS
        // 1 = RS-232
        #define NORITAKE_VFD_RS232              1
        
        //
        // ASYNCHRONOUS SERIAL INTERFACE OPTIONS
        //
        //  NORITAKE_VFD_BAUD           Baud rate
        //  NORITAKE_VFD_RS232          RS-232/CMOS switch
        //  OUT_PORT & OUT_PIN          Data line port & pin
        //  BUSY_PORT & BUSY_PIN        Busy line port & pin
        //  RESET_PORT & RESET_PIN      Reset line port & pin
        //
        #define NORITAKE_VFD_BAUD       38400
        #define OUT_PIN                 4
        #define OUT_PORT                PORTG
        #define BUSY_PIN                7
        #define BUSY_PORT               PORTB
        #define RESET_PIN               6
        #define RESET_PORT              PORTA
    #elif NORITAKE_VFD_SERIAL==1
        //
        // SYNCHRONOUS SERIAL INTERFACE OPTIONS
        //
        //  OUT_PORT & OUT_PIN          Data line port & pin
        //  SCK_PORT & SCK_PIN          Clock line port & pin
        //  BUSY_PORT & BUSY_PIN        Busy line port & pin
        //  RESET_PORT & RESET_PIN      Reset line port & pin
        //
        #define OUT_PIN                 4
        #define OUT_PORT                PORTG
        #define SCK_PIN                 3
        #define SCK_PORT                PORTG
        #define BUSY_PIN                7
        #define BUSY_PORT               PORTB
        #define RESET_PIN               6
        #define RESET_PORT              PORTA        
    #elif NORITAKE_VFD_SERIAL==2
        //
        // SPI INTERFACE OPTIONS
        //
        //  OUT_PORT & OUT_PIN          Data line port & pin
        //  BUSY_PORT & BUSY_PIN        Busy line port & pin
        //  RESET_PORT & RESET_PIN      Reset line port & pin
        //  CS_PORT & CS_PIN            Chip select line port & pin
        //
        #define OUT_PIN                 4
        #define OUT_PORT                PORTG
        #define SCK_PIN                 3
        #define SCK_PORT                PORTG
        #define BUSY_PIN                7
        #define BUSY_PORT               PORTB
        #define RESET_PIN               6
        #define RESET_PORT              PORTA
        #define CS_PIN                  0
        #define CS_PORT                 PORTA
    #endif
#elif NORITAKE_VFD_INTERFACE==1
    // 0=share the BUSY flag with D7 on the parallel interface
    // 1=BUSY flag is connected to pin #3
    // Please check the documentation for your target module to determine
    // the jumper settings for this option. This option cannot be 1 when
    // NORITAKE_VFD_RESET_CONNECTED==1.
    #define NORITAKE_VFD_BUSY_CONNECTED     0
    
    // 0=no reset function available on the parallel interface
    // 1=reset is connected on pin #3
    // Please check the documentation for your target module to determine
    // the jumper settings for this option. This option cannot be 1 when
    // NORITAKE_VFD_BUSY_CONNECTED==1.
    #define NORITAKE_VFD_RESET_CONNECTED    0
    
    #if NORITAKE_VFD_BUSY_CONNECTED==1 && NORITAKE_VFD_RESET_CONNECTED==1
    #error "NORITAKE_VFD_BUSY_CONNECTED and NORITAKE_VFD_RESET_CONNECTED cannot both be set to 1"
    #endif
    
    //
    // I80 PARALLEL INTERFACE OPTIONS
    //  WR_PORT & WR_PIN        WR line port & pin
    //  RD_PORT & RD_PIN        RD line port & pin
    //  BUSY_PORT & BUSY_PIN    Busy line port & pin
    //  RESET_PORT & RESET_PIN    Reset line port & pin
    //  D0_PORT & D0_PIN        Data bit 0 port & pin
    //  D1_PORT & D1_PIN        Data bit 1 port & pin
    //  D2_PORT & D2_PIN        Data bit 2 port & pin
    //  D3_PORT & D3_PIN        Data bit 3 port & pin
    //  D4_PORT & D4_PIN        Data bit 4 port & pin
    //  D5_PORT & D5_PIN        Data bit 5 port & pin
    //  D6_PORT & D6_PIN        Data bit 6 port & pin
    //  D7_PORT & D7_PIN        Data bit 7 port & pin
    //  Please refer to VFD module specification for pin assignments.
    
    #define BUSY_PIN                7
    #define BUSY_PORT               PORTA
    #define RESET_PIN               7
    #define RESET_PORT              PORTA

    #define WR_PIN                  0
    #define WR_PORT                 PORTG
    #define RD_PIN                  1
    #define RD_PORT                 PORTG
    #define D0_PORT                 PORTC
    #define D0_PIN                  0
    #define D1_PORT                 PORTC
    #define D1_PIN                  1
    #define D2_PORT                 PORTC
    #define D2_PIN                  2
    #define D3_PORT                 PORTC
    #define D3_PIN                  3
    #define D4_PORT                 PORTC
    #define D4_PIN                  4
    #define D5_PORT                 PORTC
    #define D5_PIN                  5
    #define D6_PORT                 PORTC
    #define D6_PIN                  6
    #define D7_PORT                 PORTC
    #define D7_PIN                  7
#elif NORITAKE_VFD_INTERFACE==2
	#define NORITAKE_VFD_FILE		"/dev/ttyAMA0"
    #define NORITAKE_VFD_BAUD       38400
#endif
