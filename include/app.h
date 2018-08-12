#ifndef APP_H
#define APP_H

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
 
 *****************************************************************************/

#include "app_defs.h"

#include "modes/mode.h"
#include "other/tempo.h"
#include "sysex/sysex.h"

void hal_plot_led(u8 type, u8 index, u8 red, u8 green, u8 blue);
void hal_send_midi(u8 port, u8 status, u8 data1, u8 data2);
void hal_send_sysex(u8 port, const u8* data, u16 length);
void hal_read_flash(u32 offset, u8 *data, u32 length);
void hal_write_flash(u32 offset,const u8 *data, u32 length);

void app_timer_event();
void app_surface_event(u8 type, u8 index, u8 value);
void app_midi_event(u8 port, u8 status, u8 d1, u8 d2);
void app_aftertouch_event(u8 index, u8 value);
void app_cable_event(u8 type, u8 value);
void app_sysex_event(u8 port, u8 * data, u16 count);

void app_init(const u16 *adc_buffer);

#endif