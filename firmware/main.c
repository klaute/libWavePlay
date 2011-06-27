/* (C) Copyright 2011 Kai Lauterbach klaute at web.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include "main.h"

int main(void) {

    init(); // init hardware

#if AVR_BOARD == BOARD_MINIMEXLE
    // clear display
    _delay_ms(0xff);
    lcd_clearDisplay();

    // print out the "user interface"
    _delay_ms(0xff);
    lcd_gotoxy(0,0);
    lcd_putstr(soff);

    lcd_gotoxy(0,1);
    lcd_putstr(btn_start);
#endif
    
    //uint16_t cnt = 0;

    unsigned tmpPlaying = 0;
    
    do {
    
        if ( lwp_isPlaying() == 1 && lwp_isPlaying() != tmpPlaying ) {
#if AVR_BOARD == BOARD_MINIMEXLE
            lcd_gotoxy(0,1);
            lcd_putstr(btn_stop);

            _delay_ms(10);
            
            lcd_gotoxy(0,0);
            lcd_putstr(son);
#endif
            
            tmpPlaying = lwp_isPlaying();
            
        } else if ( lwp_isPlaying() == 0 && lwp_isPlaying() != tmpPlaying ) {
#if AVR_BOARD == BOARD_MINIMEXLE
            lcd_gotoxy(0,1);
            lcd_putstr(btn_start);

            _delay_ms(10);
        
            lcd_gotoxy(0,0);
            lcd_putstr(soff);
#endif
            
            tmpPlaying = lwp_isPlaying();
            
        }
    
#if AVR_BOARD == BOARD_MINIMEXLE
        // check if button 1 is pressed
        if ( mm_btnchk(PC0) ) {

            lwp_Play(0);
            
        } // check if button 2 is pressed
        else if ( mm_btnchk(PC1) ) {

            lwp_Stop();

        }
#else
    if ( tmpPlaying == 0 ) {
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        _delay_ms(100);
        lwp_Play(0); // start playing
    }
#endif

    } while (1==1);

    return 0;

}

void init(void)
{

#if AVR_BOARD == BOARD_MINIMEXLE

    // Alle Ports auf std. Werte setzen.
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;

    // LED einschalten
    PORTB |= ( 1 << PB2 ); 

    // Alle Taster als Eingang.
    DDRC &= ( ~( 1 << PC3 ) );
    DDRC &= ( ~( 1 << PC2 ) );
    DDRC &= ( ~( 1 << PC1 ) );
    DDRC &= ( ~( 1 << PC0 ) );
 
    // Enable all PullUp-Resistors on all Ports which are connected to a button.
    PORTC |= ( 1 << PC3 );
    PORTC |= ( 1 << PC2 );
    PORTC |= ( 1 << PC1 );
    PORTC |= ( 1 << PC0 );

    lcd_init();
#endif

    lwp_init();
    
    sei(); // Interrupts aktivieren.

}


