#include "app_defs.h"

// STM32F10x Standard Peripherals Library
// http://stm32.kosyak.info/doc/index.html
extern void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);
extern void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup);

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
extern void RCC_APB1PeriphClockCmd(u32 RCC_APB1Periph, FunctionalState state);

// Novation launchpad_pro.a
extern void init_gpios();
extern void init_device();
extern void init_exti();
extern void init_surface();
extern void init_timers();
extern void init_adc();
extern void init_midi();
extern void midi_parsers_init();
extern void usb_unconnected();
extern void init_usbconfig();   // might have (int a1, int a2, int a3), idk
extern void adc_init();
extern void pad_setcalibration(u16 padmin, u16 padmax, u16 aftertouchmin, u16 aftertouchmax);

extern u16 ADC_Direct[64];   // might not need to specify size, idk
extern void app_init(const u16 *adc_buffer);

extern u8 PreviousMIDIIn, PreviousMIDIOut;
extern void manager_handle_midijack_switches(u8 jacksocket, u8 value);

extern u8 last_configured_update, last_configured;
extern void remember_USBUnconfigured();
extern void remember_USBConfigured();

extern void usbmidi_poll();
extern void midi_parsers_poll();
extern void midi_din_poll();

extern volatile u8 EventFlags;
extern void surface_1khz();
extern void surface_poll_power();
extern void app_timer_event();

extern void surface_200hz();

extern u8 flush_msg_count;
extern u16 flush_msg_delay;

extern void midi_flush_windows_buffer();

// Redefine main :b1:
int main() {
    NVIC_SetVectorTable(0x8000000,0x6400);  // base vector
    __asm__("cpsie i");    // enableIRQinterrupts();

    NVIC_PriorityGroupConfig(0x300);
    init_gpios();

    RCC_APB1PeriphClockCmd(0x18000000,1);
    
    init_device();  // this sets up device descriptor (0x0051 + device id from BL)
    init_exti();    // might be for midi jacks, idk
    init_surface();    // we have buttons not sensitive pads!
    init_timers();
    init_adc();
    init_midi();
    midi_parsers_init();
    usb_unconnected();
    init_usbconfig();
    adc_init();
    pad_setcalibration(0x100,0x3ff,0x200,0x301);  // definitely dont need this
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