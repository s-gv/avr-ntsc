#define F_CPU 14318180UL

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

inline void correctJitter()
{
	asm("in r16,0x2C"); // get TCNT1L to reg16

	asm("subi r16,0x8A");

	asm("cpi r16,1");
	asm("brlo .");
	asm("cpi r16,2");
	asm("brlo .");
	asm("cpi r16,3");
	asm("brlo .");
	asm("cpi r16,4");
	asm("brlo .");
	asm("cpi r16,5");
	asm("brlo .");
	asm("cpi r16,6");
	asm("brlo .");
	asm("cpi r16,7");
	asm("brlo .");
	asm("cpi r16,8");
	asm("brlo .");
	asm("cpi r16,9");
	asm("brlo .");
	asm("cpi r16,10");
	asm("brlo .");
	//asm("nop");
}

ISR(TIMER1_COMPB_vect)
{
	scanline++;
	if(scanline == 248)
	{
		OCR1A = 850;
		ICR1 = 907;
	}
	else
	{
		OCR1A = 67;
		ICR1 = 909;
	}
	if(scanline == 262)
		scanline = 0;
}
ISR(TIMER1_COMPA_vect)
{
	DDRA = 0xE0;
	_delay_us(3);

	correctJitter();

	if(scanline < 240)
	{
		uint16_t i;
		_delay_us(2);
	
		for(i=(scanline - 20)/16;i<256;i+=16)
		{
			DDRA = i;
			_delay_us(2);
		}
	}

	DDRA = 0xF0;
}
int main()
{
	//Video Port
	DDRA = 0xF0;
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
		asm("nop");
		asm("nop");
		asm("nop");
	}
	return 0;
}
