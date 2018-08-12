#include "other/challenge.h"

u8 challenge_do = 0;
u16 challenge_counter = 0;

void challenge_timer_event() {
	for (u8 i = 0; i < challenge_speed; i++) {
		syx_challenge_response[7] = challenge_counter >> 7;
		syx_challenge_response[8] = challenge_counter & 127;
		
		hal_send_sysex(USBMIDI, &syx_challenge_response[0], syx_challenge_response_length);
		
		if (++challenge_counter >= challenge_max) {
			challenge_do = 0;
			return;
		}
	}
}