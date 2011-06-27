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
 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BOARD_MINIMEXLE   1
#define BOARD_OTHER       2

#ifndef AVR_BOARD
#define AVR_BOARD BOARD_MINIMEXLE
#endif

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#if AVR_BOARD == BOARD_MINIMEXLE
#include "lcd_lib.h"
#include "minimexle.h"
#endif

#if AVR_BOARD == BOARD_MINIMEXLE
char       *son = "playing "; // sound on message
char      *soff = "stopped "; // sound off message
char *btn_start = "[start] [    ]";
char  *btn_stop = "[     ] [stop]";
#endif

void init(void);

extern volatile uint16_t _lwp_wavePos;
extern volatile unsigned _lwp_isPlaying;

extern void lwp_init(void);

extern unsigned lwp_isPlaying(void);
extern void lwp_Play(uint16_t);
extern void lwp_Pause(void);
extern void lwp_Stop(void);



