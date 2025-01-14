
///  DDR 1=out, 0 =in                TRIS 0=out,1=in
///  PORT=val same as LATCH=val      PORT=val same as LATCH=val
///  val=PORT, reads LATCH           val=PORT reads PIN state
///  val=PIN,  reads PIN state       val=LATCH reads latch

///  Notes:
///   "SENSOR" gets convereted into the required DDR and BIT definitions.
///   We use the comma operator to return the PIN test.
///   We do not use {} around the statements so they behave like functions.
///   Consider what would happen if you used breaces (should be obviious)
///      if ( GPIB_IO_RD(SENSOR) )
///      {
///       printf"SENSOR");
///      }
///      else
///      {
///  do stuff
///      }

#define AVR_DIR_IN(a)       BIT_CLR( a ## _DDR,  a ## _BIT)
#define AVR_DIR_OUT(a)      BIT_SET( a ## _DDR,  a ## _BIT)

#define AVR_LATCH_LOW(a)    BIT_CLR( a ## _PORT, a ## _BIT)
#define AVR_LATCH_HI(a)     BIT_SET( a ## _PORT, a ## _BIT)

#define AVR_LATCH_RD(a)     BIT_TST( a ## _PORT, a ## _BIT)

#define AVR_PIN_RD(a)       BIT_TST( a ## _PIN,  a ## _BIT)

#define AVR_PULLUP(a)       AVR_LATCH_HI(a)
#define AVR_NO_PULLUP(a)    AVR_LATCH_LOW(a)

#define IO_HI(a)                AVR_LATCH_HI(a), AVR_DIR_OUT(a)
#define IO_LOW(a)               AVR_LATCH_LOW(a), AVR_DIR_OUT(a)

#define IO_RD(a)                ( AVR_DIR_IN(a), AVR_PIN_RD(a))

#define IO_FLOAT(a)             AVR_DIR_IN(a)

#define IO_LATCH_RD(a)          ( AVR_LATCH_RD(a) )

#define IO_JUST_RD(a)           ( PIN_RD(a))
#define IO_JUST_LOW(a)          ( AVR_LATCH_LOW(a) )
#define IO_JUST_HI(a)           ( AVR_LATCH_HI(a) )
/// @todo  Add pullup options
///  WARNING not all CPUs have pullups so avoide these
///  If you have a pull up always on or off just alter the
///  base macros. If you MUST switch it that other will have
///  to add hardware if thier CPU does not support it.

#ifdef atmega1284p

#define SS_DDR   DDRB
#define SS_PORT  PORTB
#define SS_PIN   PINB
#define SS_BIT   4

#define SCK_DDR   DDRB
#define SCK_PORT  PORTB
#define SCK_PIN   PINB
#define SCK_BIT   7

#define MISO_DDR   DDRB
#define MISO_PORT  PORTB
#define MISO_PIN   PINB
#define MISO_BIT   6

#define MOSI_DDR   DDRB
#define MOSI_PORT  PORTB
#define MOSI_PIN   PINB
#define MOSI_BIT   5

#if 0
#define MMC_CS_DDR   DDRB
#define MMC_CS_PORT  PORTB
#define MMC_CS_PIN   PINB
#define MMC_CS_BIT   3
#endif

#define SCL_DDR   DDRC
#define SCL_PORT  PORTC
#define SCL_PIN   PINC
#define SCL_BIT   0

#define SDA_DDR   DDRC
#define SDA_PORT  PORTC
#define SDA_PIN   PINC
#define SDA_BIT   1

#define RX0_DDR   DDRD
#define RX0_PORT  PORTD
#define RX0_PIN   PIND
#define RX0_BIT   0

#define TX0_DDR   DDRD
#define TX0_PORT  PORTD
#define TX0_PIN   PIND
#define TX0_BIT   1

#else
#error define I2C, RS232, SPI, MMC pins
#endif
