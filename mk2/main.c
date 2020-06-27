#include "mk2.h"

// Redefine main :b1:
int main() {
    NVIC_SetVectorTable(0x8000000, 0x3400);  // base vector
    __asm__("cpsie i");    // enableIRQinterrupts();

    NVIC_PriorityGroupConfig(0x300);

    RCC_APB1PeriphClockCmd(0x18000000u, ENABLE);   // Verified to match MK2 stock
    
    init_device();  // this sets up device descriptor (0x0051 + device id from BL)
    init_gpios();
    init_surface();
    // init_timers();
    // init_adc();
    // init_midi();
    // midi_parsers_init();
    // usb_unconnected();
    // init_usbconfig();
    // adc_init();
    // pad_setcalibration(0x100,0x3ff,0x200,0x301);  // definitely dont need this
    
    app_init(ADC_Direct);
    // POGGGG

    /* we trying to get hal_plot_led to work right now, so yeah
    
    manager_handle_midijack_switches(0, PreviousMIDIIn);
    manager_handle_midijack_switches(1, PreviousMIDIOut);
    
    while (1) {
        if (last_configured_update != '\0') {
            last_configured_update = '\0';
            if (last_configured == '\0') {
                remember_USBUnconfigured();
            }
            else {
                remember_USBConfigured();
            }
        }

        usbmidi_poll();
        midi_parsers_poll();
        midi_din_poll();

        if (EventFlags & 1) {
            EventFlags = EventFlags & 0xfe;
            surface_1khz();
            surface_poll_power();
            app_timer_event();
        }

        if ( !(EventFlags & 2) )
            goto LABEL_11;

        EventFlags &= 0xFDu;

        surface_200hz();

        if (flush_msg_delay) {
            --flush_msg_delay;

            LABEL_11:
            if (!flush_msg_delay)
                goto LABEL_12;
        } else {
            LABEL_12:
            if (flush_msg_count) {
                midi_flush_windows_buffer();
                --flush_msg_count;
                flush_msg_delay = 10;
            }
        }
    }
    */
}