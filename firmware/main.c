
#include "main.h"

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
    DDRB &= ~( 1 << PB5 );

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

    // Timer1 initialisieren
    TCCR1A = 0x00;
    TCCR1B = 0x00;
 
    // Die PullUp-Widerst�nde einschalten.
    PORTC |= ( 1 << PC3 );
    PORTC |= ( 1 << PC2 );
    PORTC |= ( 1 << PC1 );
    PORTC |= ( 1 << PC0 );

    // Es soll eine Interrupt ausgel�st werden wenn ein Overflow auftritt oder
    // der Vergleichswert erreicht wurde.
    TIMSK1 = (1 << OCIE1A);

    OCR1A = 1451; // Vergleichswert festlegen

    TCNT1 = 0;

    lcd_init();

    sei(); // Interrupts aktivieren.

}

int main(void) {

    init();

    do {
        // Hier das wav ausgeben

        if ( mm_btnchk(PC0) )
            timerCtrl(1);
        if ( mm_btnchk(PC1) )
            timerCtrl(0);

        _delay_ms(100);

        mm_LEDCode(2); // toggle status led


    } while (1==1);

    return 0;

}

void timerCtl(unsigned stat)
{
        switch ( stat )
        {
            case 1:
                // Timer starten
                TCNT1 = 1;
                TCCR1A = 0x00;
                TCCR1A |= (1 << WGM11); // CTC enabled
                TCCR1B |= (1 << CS10); // Prescaler = CPU-Takt
            break;
            case 2: // Toggle des Timers
                if ( TCCR1B & (1 << CS10) )
                    timerCtl(0); // abschalten
                else
                    timerCtl(1); // anschalten
            break;
            default :
                // Timer stoppen
                TCCR1A = 0x00;
                TCCR1B = 0x00;
        }
}


ISR (TIMER1_COMPA_vect)
{
  // Hier das neue Sample laden und die Ausgabe einleiten

  // ?berlauf des Timers (TIMER1)
  //PORTD ^= (1 << PD5 ); // Toggeln des Summer Pin's.
}

