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

#ifndef _LIB_WAVE_PLAY_H_
#define _LIB_WAVE_PLAY_H_

#if F_CPU != 16000000
#error 16MHz supported only!
#endif

/*
 * 1 := Flash
 * 2 := EEPROM
 * 3 := MMC/SD ohne FAT
 * 4 := MMC/SD mit FAT
 * 5 := ?
 */
#define SRC_FLASH           1
#define SRC_EEPROM          2

#define WAVE_SOURCE SRC_FLASH

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#if WAVE_SOURCE == SRC_EEPROM

#include <avr/eeprom.h>

#ifndef EEMEM
// alle Textstellen EEMEM im Quellcode durch __attribute__ ... ersetzen
#define EEMEM  __attribute__ ((section (".eeprom")))
#endif

#define EEP_WAVE_DATA_SIZE 1024 // size in bytes of the wave in eeprom

#endif

#define LWP_STOP_TIMER0     0
#define LWP_STOP_TIMER2     3
#define LWP_START_TIMER0    1
#define LWP_START_TIMER2    4
#define LWP_TOGGLE_TIMER0   2
#define LWP_TOGGLE_TIMER2   5
#define LWP_START_ALL_TIMER 6
#define LWP_STOP_ALL_TIMER  7

volatile uint16_t _lwp_wavePos   = 0;
volatile unsigned _lwp_isPlaying = 0;

#if WAVE_SOURCE == SRC_EEPROM
volatile uint16_t data_wave_len;
EEMEM uint8_t data_wav[EEP_WAVE_DATA_SIZE];
#endif

void _lwp_timerCtrl(unsigned);
void lwp_init(void);

unsigned lwp_isPlaying(void);
void lwp_Play(uint16_t);
void lwp_Pause(void);
void lwp_Stop(void);

#endif

