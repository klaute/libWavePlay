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
 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define LWP_START_ALL_TIMER 6
#define LWP_STOP_ALL_TIMER 7

volatile uint16_t _lwp_wavePos = 0;
volatile unsigned _lwp_isPlaying = 0;

void _lwp_timerCtrl(unsigned);
void lwp_init(void);

unsigned lwp_isPlaying(void);
void lwp_Play(uint16_t);
void lwp_Pause(void);
void lwp_Stop(void);


#ifndef _WIN32
#include "libWavePlay.c"
#endif
