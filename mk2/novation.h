#include "../include/app_defs.h" // TODO this needs to be done properly later

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

extern u16 ADC_Direct[64];
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

extern void device_setuppid(u8 id);
extern u8 DeviceDescriptor[18];  // No need to override, PID is reset in device_setuppid
extern u8 USBDesc_StringSerial[20];

extern volatile u8 LedStatus[10];