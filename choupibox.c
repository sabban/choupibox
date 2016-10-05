/*
    Copyright (c) 2016 Manuel Sabban <manu@sabban.eu>
    
    Permission is hereby granted, free of charge, to any person 
    obtaining a copy of this software and associated documentation 
    files (the "Software"), to deal in the Software without 
    restriction, including without limitation the rights to use, copy, 
    modify, merge, publish, distribute, sublicense, and/or sell copies 
    of the Software, and to permit persons to whom the Software is 
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be 
    included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.

    $Id$
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <util/atomic.h>
#include "spi.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_LEDS 15 
volatile bool update = false;
uint8_t*  led_array;

void shift(uint8_t* array);

#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator 
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1
volatile uint8_t portAhistory = 0xFF;
/* _BV(a) is a mcro which returns the value corresponding to 2 to the power 'a'. Thus _BV(PX3) would be 0x08 or 0b00001000 */



ISR(PCINT0_vect) {
  //  if (PINA & _BV(PA0)) update = true;
  // _delay_ms(500); // Giant friggin' debounce delay

    uint8_t changedbits;

    changedbits = (PINA ^ portAhistory) ;
    changedbits &= portAhistory; // rising edge
    portAhistory = PINA;
     
    if(changedbits & (1 << PA0)) { //detect which bit changed 
	  shift(led_array);
	  led_array[0] = 0x00;
	  led_array[1] = 0x00;
	  led_array[2] = 0xFF;
	  spi_transmit_sync (led_array, 3 * NUM_LEDS);
	  /* PCINT0 changed */
    }
    

    if(changedbits & (1 << PA1)){
	  shift(led_array);
	  led_array[0] = 0x00;
	  led_array[1] = 0xFF;
	  led_array[2] = 0xFF;
	  spi_transmit_sync (led_array, 3 * NUM_LEDS);
	/* PCINT1 changed */
    }

    if(changedbits & (1 << PA2)) { 
      shift(led_array);
      led_array[0] = 0x00;
      led_array[1] = 0xFF;
      led_array[2] = 0x00;
      spi_transmit_sync (led_array, 3 * NUM_LEDS);
      /* PCINT2 changed */
    }

    if(changedbits & (1 << PA3)) {
      shift(led_array);
      led_array[0] = 0xFF;
      led_array[1] = 0xFF;
      led_array[2] = 0xFF;
      spi_transmit_sync (led_array, 3 * NUM_LEDS);
      /* PCINT3 changed */
    }

    if(changedbits & (1 << PA4)) {
      shift(led_array);
      led_array[0] = 0xFF;
      led_array[1] = 0x00;
      led_array[2] = 0x00;
      spi_transmit_sync (led_array, 3 * NUM_LEDS);
    }
    /* PCINT4 changed */


 }

void shift(uint8_t* array) {
  uint8_t i;
  for (i=NUM_LEDS *3; i>0; i--)  {
    array[i]= array[i-3];
  }
}

void main(void)
{
  unsigned int i;
  led_array=(uint8_t *) malloc( 3 * NUM_LEDS);
  DDRB=0xff; //PORTB as OUTPUT
  PORTB=0x00; //All pins of PORTB LOW
  sbi(PORTB,PB0);
  sbi(PORTB,PB1);
  sbi(PORTB,PB2);
  sbi(PORTB,PB3);
  sbi(PORTB,PB4);


  DDRA &= ~((1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3)| (1 << DDA4)); // Clear the PA0, PA1, PA2 pin
  // PA0,PA1,PA2 (PCINT0, PCINT1, PCINT2 pin) are now inputs
  
  PCICR |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT0);

  PORTA |= ((1 << PORTA0) | (1 << PORTA1) | (1 << PORTA2) | (1 << PORTA3) | (1 << PORTA4)); // turn On the Pull-up
  // PA0, PA1, PA2, PA3and PA4 are now inputs with pull-up enabled
  
  PCICR |= (1 << PCIE0);     // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) |  (1 << PCINT3) |  (1 << PCINT4) ;
  // set PCINT0 to trigger an interrupt on state change 

  EICRA |= ( (1 << ISC01) | (1 << ISC00) );
  
  // Allocate buffer array
  uint8_t data_buf[1];
  // Initialize SPI
  spi_init();
  // Send 0xFF to spi slave and read 1 byte back to the same array
  //  data_buf[0] = 0xFF;
  //spi_transfer_sync(data_buf,data_buf,1);
  sei();                     // turn on interrupts
  
  while(1)
    {
      /*main program loop here */
    }  
 

}
