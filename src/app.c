/*----------------------------------------------------------------------------*/
/*  Performance-optimized Launchpad Pro Firmware modification by mat1jaczyyy  */
/*----------------------------------------------------------------------------*/

/******************************************************************************
 
  Copyright (c) 2015, Focusrite Audio Engineering Ltd.
  All rights reserved.
 
  Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of Focusrite Audio Engineering Ltd., nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 ******************************************************************************/

#include "app.h"

u32 global_timer = 0;

void app_timer_event() {
	global_timer++;

	tempo_timer++; tempo_tick();

	if (challenge_do) challenge_timer_event();

	(*mode_timer_event[mode])();
}

void app_surface_event(u8 t, u8 p, u8 v) {
	if (!vel_sensitive) {
		v = (v == 0)? 0 : 127;
	}

	(*mode_surface_event[mode])(p, v, p / 10, p % 10);
}

void app_midi_event(u8 port, u8 t, u8 p, u8 v) {
	u8 ch;

	switch (t) {
		case 0xFA:
			tempo_start();
			break;

		case 0xF8:
			tempo_midi();
			break;
		
		case 0xFC:
			tempo_stop();
			break;
		
		default:
			ch = t % 16;
			t >>= 4;

			if (mode != mode_ableton && mode_default == mode_ableton) {
				(*mode_midi_event[mode_default])(port, t, ch, p, v);
			}

			(*mode_midi_event[mode])(port, t, ch, p, v);
			break;
	}
}

void app_aftertouch_event(u8 p, u8 v) {
	u8 result = aftertouch_update(p, v);

	if (result != -1 && aftertouch_enabled == 1)
		(*mode_aftertouch_event[mode])(result);
	
	if (aftertouch_enabled == 2)
		(*mode_poly_event[mode])(p, v);
}

void app_cable_event(u8 t, u8 v) {} // Unused

void app_sysex_event(u8 port, u8 *d, u16 l) {
	handle_sysex(port, d, l);
}

void app_init(const u16 *adc_raw) {
	flash_read();
	mode_update(mode_boot);
}