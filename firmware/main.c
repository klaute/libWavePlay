
#include "main.h"
#include "wavedata.c"

int main(void) {

    init();

    _delay_ms(0xff);
    lcd_clearDisplay();

    _delay_ms(0xff);
    lcd_gotoxy(0,0);
    lcd_putstr(soff);

    //uint16_t cnt = 0;

    do {

        if ( mm_btnchk(PC0) ) {

            lcd_gotoxy(0,0);
            lcd_putstr(son);

            timerCtrl(1);
            timerCtrl(4);

        } else if ( mm_btnchk(PC1) ) {

            lcd_gotoxy(0,0);
            lcd_putstr(soff);
            timerCtrl(0);

            timerCtrl(3);
        }

        /* Lesetest des wav im progmem. * /
        uint8_t val = pgm_read_byte(&data_wav[cnt]);

        char sval[10];
        lcd_gotoxy(0,1);
        utoa( val, sval, 16 );
        lcd_putstr(sval);
        lcd_gotoxy(5,1);
        utoa( cnt, sval, 10 );
        lcd_putstr(sval);

        cnt++;
        if (cnt >= data_wav_len)
            cnt = 0;
        */

        _delay_ms(50);

        mm_LEDCode(2); // toggle status led

    } while (1==1);

    return 0;

}

void init(void)
{

    // Alle Ports auf std. Werte setzen.
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;

    // Initialisieren von Pin5 an PortB als Ausgang,
    // zum ansteuern des Summers.
    DDRB &= ~( 1 << PB5 ); // OC0B
    //DDRB &= ~( 1 << PB6 ); // OC0A

    // LED einschalten
    PORTB |= ( 1 << PB2 ); 

    // Alle Taster als Eingang.
    DDRC &= ( ~( 1 << PC3 ) );
    DDRC &= ( ~( 1 << PC2 ) );
    DDRC &= ( ~( 1 << PC1 ) );
    DDRC &= ( ~( 1 << PC0 ) );
 
    // Die PullUp-Widerst�nde f�r die Taster einschalten.
    PORTC |= ( 1 << PC3 );
    PORTC |= ( 1 << PC2 );
    PORTC |= ( 1 << PC1 );
    PORTC |= ( 1 << PC0 );

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

    lcd_init();

    sei(); // Interrupts aktivieren.

}

void timerCtrl(unsigned stat)
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
                TCCR0A = 0x00;
                // Fast PWM, nicht invertierter Output an OC1A und OC1B
                TCCR0A |= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01);
                TCCR0B |= (1<<CS00); // Prescaler = 1
                OCR0A = 128;
                OCR0B = 128;
                break;
            case 2: // Toggle des Timers
                if ( TCCR0B & (1 << CS00) )
                    timerCtrl(0); // abschalten
                else
                    timerCtrl(1); // anschalten
                break;
            case 3 :
                // Timer stoppen
                TCCR2A = 0x00;
                TCCR2B = 0x00;
                break;
            case 4:
                // Timer starten
                TCCR2A = 0x00;
                TCCR2A |= (1<<WGM21); // CTC
                TCCR2B |= (1<<CS21); // Prescaler = 8
                break;
            case 5: // Toggle des Timers
                if ( TCCR2B & (1 << CS21) )
                    timerCtrl(3); // abschalten
                else
                    timerCtrl(4); // anschalten
                break;
            default:
                timerCtrl(0);
                timerCtrl(3);
        }
}



ISR (TIMER0_OVF_vect)
{
//    mm_BeepCode(0); // Summer Pin ein
    mm_LEDCode(1); // toggle status led
}

ISR (TIMER0_COMPA_vect)
{
    // ueberlauf des Timers (TIMER0)
//    mm_BeepCode(1);
    mm_LEDCode(0);
}

/*
ISR (TIMER0_COMPB_vect)
{
    // ueberlauf des Timers (TIMER0)
    //mm_BeepCode(2);
    //mm_LEDCode(2);
}
*/

ISR (TIMER2_COMPA_vect)
{
    // Hier das neue Sample laden und die Ausgabe einleiten
    wavepos++;

    if (wavepos >= data_wav_len) {

        wavepos = 0; // fuer die Endlosschleife und den neustart
        timerCtrl(0);
        timerCtrl(3);

        lcd_gotoxy(0,0);
        lcd_putstr(soff);
        
        lcd_gotoxy(0,0);
        lcd_putstr(soff);

    } else {

        uint8_t val = pgm_read_byte(&data_wav[wavepos]);
        OCR0A = val; // Neuen Vergleichswert festlegen
        OCR0B = val;
        
        lcd_gotoxy(0,1);
        char sval[10];
        utoa( val, sval, 10 );
        lcd_putstr(sval);

    }

    //mm_LEDCode(2);
    
}

