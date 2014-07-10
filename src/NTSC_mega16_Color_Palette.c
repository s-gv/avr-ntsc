// #define F_CPU 14318180UL // May be needed in AVR Studio

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<inttypes.h>

/* 74LS195 Connections
PD7 - /LD

DDRA (0:3) - LUMA
DDRA ( 4:7 ) - CHROMA
*/

volatile uint16_t scanline = 0;

ISR(TIMER1_COMPB_vect)
{
	scanline++;
	if(scanline == 248)
	{
		OCR1A = 850;
	}
	else
	{
		OCR1A = 67;
	}
	if(scanline == 263)
		scanline = 0;
}
ISR(TIMER1_COMPA_vect)
{
	_delay_us(4);
	if(scanline > 40 && scanline < 220)
	{
		uint16_t i;
		for(i=(scanline - 40)/16;i<256;i+=16)
		{
			DDRA = i; // change color of 'pixels'
			_delay_us(2);
		}
	}
	DDRA = 0xE0; // return to throwing out color burst, even during sync.
}
int main(void)
{
	//Video Port
	DDRA = 0xE0; // Keep the color burst 'ON' at all times (including sync).
	PORTA = 0x0F;

	//74LS195
	DDRD |= _BV(7);
	PORTD &= ~(_BV(7));
	_delay_ms(10);
	PORTD |= _BV(7);

	//Timer
	DDRD |= _BV(5); // C-Sync pin

	TCCR1A |= _BV(7) | _BV(6) | _BV(1); // TOP-ICR1 , fast PWM, set on OCR1A match
	TCCR1B |= _BV(4) | _BV(3);
	TCNT1 = 0;
	OCR1A = 62; // short sync.
	OCR1B = 1; 
	ICR1 = 909; // H-line length = 63.55555uS
	TIMSK |= _BV(4) | _BV(3); // interrupt on COMPA and COMPB
	
	TCCR1B |= 1;//start timer, no prescale.
	sei();
		
	while(1)
	{
		asm("sleep");
	}
	return 0;
}
