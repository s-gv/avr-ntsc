/*
   Copyright (c) 2014 Sagar G V (sagar.writeme@gmail.com)

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

// #define F_CPU 14318180UL

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<inttypes.h>
#include<avr/sleep.h>

volatile uint16_t scanline = 0; // scanline will be accessed from ISR. So, make it volatile.

ISR(TIMER1_COMPB_vect)
{
    // Enter interrupt on every scanline.
    uint8_t luma, chroma;

    // Start drawing pixels if scanline is visible.
    if(scanline > 40 && scanline < 220)
    {
        luma = (((uint8_t)scanline) >> 4) & 0x0F; // Luma increases from top to bottom of the screen.
        for (chroma = 0; chroma < 16; chroma++) // Chroma changes from left to right on every scanline.
        {
            DDRA = (chroma << 4) | luma; // change color of 'pixels'.
            _delay_us(2);
        }
    }

    // Drawing done for this scanline. Return to throwing out color burst (even during sync).
    DDRA = 0xE0; 

    // Should a long-sync be given to signal V-Sync ?
    scanline++;
    if(scanline == 247)
    {
        OCR1A = 850; // OCR1A is double buffered. So, the next scanline will be a long-sync.
    }
    else
    {
        OCR1A = 67;
    }

    // Draw 263 lines per frame. 
    if(scanline == 263)
    {
        scanline = 0;
    }
}
int main(void)
{
    // Make all unused pins as output.
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    //Video Port: (DDRA (0:3) - LUMA), (DDRA (4:7) - CHROMA).
    DDRA = 0xE0; // Keep the color burst 'ON' at all times (including sync).
    PORTA = 0x0F; // Luma bits are kept high while chroma bits are kept low.

    //Shift register 
    DDRD |= (1 << 7); // PD7 is connected to the SHIFT/LD pin. Make PD7 output pin.
    PORTD &= ~(1 << 7); // put register into load mode to parallel load "1100".
    _delay_ms(10); // wait for a while.
    PORTD |= (1 << 7); // put register back to shift mode.

    // Setup sleep mode
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable(); // Enable sleep mode (this does not put the MCU to sleep right now).

    //Timer/PWM config
    DDRD |= (1 << 5); // C-Sync pin direction should be output.
    TCCR1A |= (1 << 7) | (1 << 6) | (1 << 1); // TOP-ICR1 , fast PWM, set PWM pin on OCR1A match.
    TCCR1B |= (1 << 4) | (1 << 3);
    TCNT1 = 0; // timer value made 0.
    OCR1A = 67; // short sync for about 4.7 uS.
    OCR1B = 150; // Enter interrupt after sync to start drawing pixels.
    ICR1 = 909; // Timer Top Value = H-line length = 63.55555uS.
    TIMSK |= (1 << 3); // interrupt on COMPB.
    TCCR1B |= 1;//start timer, no prescale.

    // Global interrupt enable
    sei();

    while(1)
    {
        sleep_cpu(); // To avoid ISR entry time jitter because of variable interrupt service latency, always enter ISR from sleep mode.

        /* If some work has to be done here (rather than sleeping), then
         * the ISR entry jitter has to be cancelled in the ISR. The idea is
         * to read the timer in the ISR, and if the ISR has been entered
         * early, some cycles are wasted so that line drawing always starts
         * at the same time with respect to the sync tip on every scanline 
         * regardless of the ISR entry latency.
         *
         * Example ISR code to do this:
        void correctJitter()
        {
            asm("in r16,0x2C"); // get TCNT1L to reg16
            asm("subi r16,MIN_POSSIBLE_TCNT1L_AT_THIS_POINT");
            
            // If TCNT1L is small enough (ISR entered early), waste a cycle by 
            // branching to the next instruction (which takes 2 cycles) instead of 
            // sliding to the next instruction without a branch (which takes 1 cycle).

            asm("cpi r16,1"); 
            asm("brlo .");  
            
            // Repeat this process to account for the worst-case jitter.
            
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
        }
        */
    }
    return 0;
}
