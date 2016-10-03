all: dude

choupibox.o: choupibox.c
	avr-gcc -g -Os -mmcu=atmega1284p -DF_CPU=8000000UL -c choupibox.c 

spi.o: spi.c spi.h
	avr-gcc -g -Os -mmcu=atmega1284p -DF_CPU=8000000UL -c spi.c 

choupibox.elf: spi.o choupibox.o
	avr-gcc -g -mmcu=atmega1284p -o choupibox.elf choupibox.o spi.o

dude: choupibox.elf
	avrdude -P /dev/ttyACM0 -b 19200 -c avrisp -p atmega1284p -U flash:w:choupibox.elf:a 

fuse: choupibox.elf
	avrdude -P /dev/ttyACM0 -b 19200 -c avrisp -p atmega1284p -U flash:w:test.elf:a -U lfuse:w:0xc2:m -U hfuse:w:0x99:m -U efuse:w:0xff:m
