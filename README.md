# Arduino SPI Test
This project demonstrates bi-directional communication between two Arduinos over SPI. The slave device acts as a microcontroller responding to register peaks and pokes from the master device. The master pokes the slave registers, peaks the previously written registers and displays the received bytes in engineering units via serial.

The project was tested using an Arduino Nano as master and an Arduino Pro Mini as slave
