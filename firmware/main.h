
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "lcd_lib.h"
#include "minimexle.h"

volatile uint16_t wavepos = 0;

char*       son = "playing ";
char*      soff = "stopped ";
char* btn_start = "[start] [    ]";
char* btn_stop = "[     ] [stop]";

void init(void);
void timerCtrl(unsigned);

