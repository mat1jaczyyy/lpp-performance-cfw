#include "app_defs.h"

// STM32F10x Standard Peripherals Library
// http://stm32.kosyak.info/doc/index.html
extern void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);
extern void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup);

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
extern void RCC_APB1PeriphClockCmd(u32 RCC_APB1Periph, FunctionalState state);

// Novation launchpad_pro.a
extern void init_gpios();
// extern void init_device();
extern void init_exti();
// extern void init_surface();
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

// extern void device_setuppid(u8 id);
extern u8 DeviceDescriptor[18];  // No need to override
u8 USBDesc_StringSerial[20] = {'L', 'a', 'u', 'n', 'c', 'h', 'p', 'a', 'd', ' ', 'M', 'K', '2', 'O'};

void device_setuppid(u8 id) {
    id = 0; // TEMPORARY: HARDCODE TO 0 TO IDENTIFY AS 0x0069 REGULAR LP MK2
    
    if (id > 0xF) id = 0;  // protect against invalid value

    DeviceDescriptor[10] = id + 0x69;

    if (id) {   // Add number to USB Device Name
        USBDesc_StringSerial[14] = ' ';

        if (id >= 9) {
            USBDesc_StringSerial[15] = '1';
            USBDesc_StringSerial[16] = '1' + id - 10;

        } else USBDesc_StringSerial[15] = '1' + id;
    }
}

void init_device() {
    /* This code was previously pretty bloated...
    
        u8 TempMemory[64]; // [sp+0h] [bp-48h]

        memset(TempMemory, 0, 0x40u);
        init_copymemory(0x801FC00u, TempMemory, 0x40u);
        device_setuppid(TempMemory[1]);
        
    I'll just directly read the u8 from flash.
    Changed 0x0801FC00 to 0x0800FC00 because Novation uses that in MK2 stock
    Although the C00 storage regions don't seem to match... not sure
    TODO when done: see what changing this to 1FC does */

    device_setuppid(*(u8*)0x0800FC01);
}

// Redefine main :b1:
int main() {
    NVIC_SetVectorTable(0x8000000, 0x3400);  // base vector
    __asm__("cpsie i");    // enableIRQinterrupts();

    NVIC_PriorityGroupConfig(0x300);
    init_gpios();

    RCC_APB1PeriphClockCmd(0x18000000u, ENABLE);   // Verified to match MK2 stock
    
    init_device();  // this sets up device descriptor (0x0051 + device id from BL)
    init_exti();    // might be for midi jacks, idk
    custom_init_surface();    // we have buttons not sensitive pads!
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

// Register mappings: https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// + https://github.com/Focusrite-Novation/launchpad-firmware/blob/master/cortex/cxm3_registers.h
void custom_init_surface(){
    // Set SPI timers to reset
    // RCC_APB2RSTR     SPI1 timer flag
    *(u32*)0x4002100C = 0x1000;
    // RCC_APB1ENR      SPI2 reset flag
    *(u32*)0x40021010 = 0x4000;
    
    // Clear reset registers
    *(u32*)0x4002100C = 0;
    *(u32*)0x40021010 = 0;
    
    // Enable DMA1 timer
    *(u32*)0x40021014 = *(u32*)0x40021014 | 1;
    
    // Connect DMA1 channel 4 to SPI2 (Some peripheral)
    //     CPAR4        Peripheral address
    *(u32*)0x4002004C = 0x4000380C;
    //     CMAR4        Memory address 
    *(u32*)0x40020050 = 0x20000B9F;
    
    // Reference manual pg 278 'Channel configuration procedure'
    // Describes configuring DMA channels
    // Theres probably a struct + STM HAL function that does this nicer
    *(u32*)0x40020048 = 8;
    /*
      2: High Priority
      1: 16 bit
      8: Memory Increment Mode Enabled
      2: Transfer Complete Interrupt Enabled
    */
    *(u32*)0x40020044 = 0x2182;
    
    // ¯\_(ツ)_/¯ Something to do with interrupt addresses
    *(u32*)0xE000E100 = *(u32*)0xE000E100 | 0x4000;
    
    // Set DMA1 channel 4 enabled
    *(u32*)0x40020044 = *(u32*)0x40020044 | 1;
    
    // Connect DMA1 channel 5 to SPI2 (Idk why it gets 2 channels, maybe lots of data?)
    //     CPAR5        Peripheral address
    *(u32*)0x40020060 = 0x4000380C;
    //     CMAR5        Memory address - note that it's 4 bytes from CMAR4
    *(u32*)0x40020064 = 0x20000BBF;
    // Configure channel, see stuff about reference manual above
    *(u32*)0x4002005C = 8;
    
}