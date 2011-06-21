
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "lcd_lib.h"
#include "minimexle.h"

volatile uint16_t wavepos = 0;

char* son  = "on ";
char* soff = "off";

void init(void);
void timerCtrl(unsigned);

