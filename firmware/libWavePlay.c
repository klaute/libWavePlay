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
 
#ifdef _WIN32
#include "libWavePlay.c"
#endif

#include "wavedata.c" // include the wavefile data

unsigned lwp_isPlaying() {
  return _lwp_isPlaying;
}
 
void lwp_Play(uint16_t pos) {

  _lwp_wavePos = pos;

  if (_lwp_wavePos >= data_wav_len) {
    _lwp_wavePos = 0;
  }
  
  _lwp_timerCtrl(LWP_START_ALL_TIMER);

}
 
void lwp_Pause() {
  if (_lwp_isPlaying == 1) {
    _lwp_timerCtrl(LWP_STOP_ALL_TIMER);
    _lwp_isPlaying = 0;
  } else {
    _lwp_timerCtrl(LWP_START_ALL_TIMER);
    _lwp_isPlaying = 1;
  }
}

void lwp_Stop() {

  _lwp_timerCtrl(LWP_STOP_ALL_TIMER);
  _lwp_wavePos = 0;
  _lwp_isPlaying = 0;

}
           
void _lwp_timerCtrl(unsigned stat)
{
        switch ( stat )
        {
            case 0 :
                // Timer stoppen
                TCCR0A = 0x00;
                TCCR0B = 0x00;
                break;
            case 1:
                // Timer starten
                // Fast PWM, nicht invertierter Output an OC1A und OC1B
                TCCR0A |= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01);
                TCCR0B |= (1<<CS00);
                OCR0A = 128;
                OCR0B = 128;
                break;
            case 2: // Toggle des Timers
                if ( TCCR0B & (1 << CS00) )
                    _lwp_timerCtrl(0); // abschalten
                else
                    _lwp_timerCtrl(1); // anschalten
                break;
            case 3 :
                // Timer stoppen
                TCCR2A = 0x00;
                TCCR2B = 0x00;
                break;
            case 4:
                // Timer starten
                TCCR2A |= (1<<WGM21); // CTC
                TCCR2B |= (1<<CS21); // Prescaler = 8
                break;
            case 5: // Toggle des Timers
                if ( TCCR2B & (1 << CS21) )
                    _lwp_timerCtrl(3); // abschalten
                else
                    _lwp_timerCtrl(4); // anschalten
                break;
            case LWP_START_ALL_TIMER: // start all
                _lwp_timerCtrl(1);
                _lwp_timerCtrl(4);
                break;
            case LWP_STOP_ALL_TIMER: // stop all
                _lwp_timerCtrl(0);
                _lwp_timerCtrl(3);
                break;
            default:
                _lwp_timerCtrl(0);
                _lwp_timerCtrl(3);
        }
}

void lwp_init() {

    // Initialisieren von Pin5 an PortB als Ausgang,
    // zum ansteuern des Summers.
    DDRB &= ~( 1 << PB5 ); // OC0B
    DDRB &= ~( 1 << PB6 ); // OC0A

    // Timer0 initialisieren
    TCCR0A = 0x00;
    TCCR0B = 0x00;
 
    // Timer2 initialisieren
    TCCR2A = 0x00;
    TCCR2B = 0x00;
 
    // Es soll eine Interrupt ausgeloest werden wenn ein Overflow auftritt oder
    // der Vergleichswert erreicht wurde.
    TIMSK0 = 0x00; // Normale Operation des Timer1
    //TIMSK0 = (1 << OCIE0A) | (1 << TOIE0); // Timer für die Tonausgabe, ISR zum debuggen
    TIMSK2 = (1 << OCIE2A); // Timer für das auslesen der Samples

    OCR0A = 128;
    OCR0B = 128;
    OCR2A = 250; // Samplingrate = 8kHz; Prescaler = 8; F_CPU = 16 MHz

    TCNT0 = 1;
    TCNT2 = 1;
    
    sei();

}

ISR (TIMER2_COMPA_vect)
{

    if (_lwp_wavePos >= data_wav_len) {

        _lwp_wavePos = 0; // fuer die Endlosschleife und den neustart
        
        _lwp_timerCtrl(LWP_STOP_ALL_TIMER);

        _lwp_isPlaying = 0;
        
    } else {
    
        // Ein Byte nach dem anderen aus dem FLASH auslesen.
        uint8_t val = pgm_read_byte(&data_wav[_lwp_wavePos]);
        
        OCR0A = val; // Neuen Vergleichswert festlegen
        OCR0B = val;

    }
    
    _lwp_wavePos++; // index des aktuallen samples inkrementieren

}

