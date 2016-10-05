
#    Copyright (c) 2016 Manuel Sabban <manu@sabban.eu>
    
#    Permission is hereby granted, free of charge, to any person 
#    obtaining a copy of this software and associated documentation 
#    files (the "Software"), to deal in the Software without 
#    restriction, including without limitation the rights to use, copy, 
#    modify, merge, publish, distribute, sublicense, and/or sell copies 
#    of the Software, and to permit persons to whom the Software is 
#    furnished to do so, subject to the following conditions:
#    The above copyright notice and this permission notice shall be 
#    included in all copies or substantial portions of the Software.
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
#    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
#    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
#    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
#    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
#    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
#    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
#    DEALINGS IN THE SOFTWARE.
#    $Id$

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
