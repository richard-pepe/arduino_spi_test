#include "pins_arduino.h"

#define STATE_WAIT_FOR_TYPE   0
#define STATE_WAIT_FOR_REG    1
#define STATE_PEAK            2
#define STATE_POKE            3

uint8_t  buf [7];
uint8_t  state;
uint8_t  reg;
uint8_t  type;
int      count;

void setup (void) {

   Serial.begin (9600);       // debugging

   pinMode(MISO, OUTPUT);     // have to send on master in, *slave out*
   SPCR |= _BV(SPE);          // turn on SPI in slave mode
   SPCR |= _BV(SPIE);         // turn on interrupts

   count = 0;
   state = STATE_WAIT_FOR_TYPE;

}

// SPI Interrupt Routine
ISR (SPI_STC_vect) {
   uint8_t c = SPDR;
   count++;
   switch (state) {
      case STATE_WAIT_FOR_TYPE:
         type = c;
         SPDR = 0;
         state = STATE_WAIT_FOR_REG;
         break;

      case STATE_WAIT_FOR_REG:
         reg = c;
         state = type+2;
         SPDR = buf[reg];
         break;

      case STATE_PEAK:
         //SPDR = buf[reg];
         state = STATE_WAIT_FOR_TYPE;
         break;

      case STATE_POKE:
         buf[reg] = c;
         state = STATE_WAIT_FOR_TYPE;
         SPDR = 0;
         break;
   }
}

void loop() {
   Serial.print("Register 1: "); Serial.println(buf[0]);
   Serial.print("Register 2: "); Serial.println(buf[1]);
   Serial.print("Register 3: "); Serial.println(buf[2]);
   Serial.print("Register 4: "); Serial.println(buf[3]);
   Serial.print("Register 5: "); Serial.println(buf[4]);
   Serial.print("Register 6: "); Serial.println(buf[5]);
   Serial.print("Register 7: "); Serial.println(buf[6]);
   Serial.print("Count     : "); Serial.println(count);
   delay(1000);
}