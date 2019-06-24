#include <SPI.h>
#include "pins_arduino.h"

#pragma pack(push,1)
struct reg_t {
	uint8_t		value1;
	uint16_t 	value2;
	float		value3;
};
#pragma pack(pop)

union register_t {
  reg_t eng;
  uint8_t raw[7];
};

#define SPI_PEAK	0
#define SPI_POKE	1

volatile register_t register_data[2];

void setup () {
	digitalWrite(SS, HIGH);
	SPI.begin();
	//SPI.setClockDivider(SPI_CLOCK_DIV8);

	Serial.begin (9600);   // Debugging
	Serial.println("Initializing...");

	// Configure Buffer A
	register_data[0].eng.value1 = 12;
	register_data[0].eng.value2 = 1234;
	register_data[0].eng.value3 = 1.234;

	// Configure Buffer B
	register_data[1].eng.value1 = 222;
	register_data[1].eng.value2 = 2345;
	register_data[1].eng.value3 = 2.345;

  delay(1000);

}

void loop () {

	Serial.println("Executing...");

	poke(0);
	peak(0);
	poke(1);
	peak(1);

	delay (1000);

}

void poke (int idx) {

	// Slave Select Enable
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	digitalWrite(SS, LOW);    		// SS is pin 10

	// Transmit
	for (uint8_t i=0; i<sizeof(reg_t); i++) {
	    delayMicroseconds(15);
	    SPI.transfer(SPI_POKE);		// Transaction Type
	    delayMicroseconds(15);
	    SPI.transfer(i);			// Register
	    delayMicroseconds(15);
		SPI.transfer(register_data[idx].raw[i]);
	}

	// Slave Select Disable
	digitalWrite(SS, HIGH);
	SPI.endTransaction();

}

void peak (int idx) {

	// Slave Select Enable
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	digitalWrite(SS, LOW);

	// Transmit
	for (uint8_t i=0; i<sizeof(reg_t); i++) {
	    delayMicroseconds(15);
	    SPI.transfer(SPI_PEAK);		// Transaction Type
	    delayMicroseconds(15);
	    SPI.transfer(i);			// Register
	    delayMicroseconds(15);
		register_data[idx].raw[i] = SPI.transfer(0);
	}

	// Slave Select Disable
	digitalWrite(SS, HIGH);
	SPI.endTransaction();

	// Debug Received Bytes in Engineering Units
	Serial.print("Value 1: "); Serial.println(register_data[idx].eng.value1);
	Serial.print("Value 2: "); Serial.println(register_data[idx].eng.value2);
	Serial.print("Value 3: "); Serial.println(register_data[idx].eng.value3);

}
