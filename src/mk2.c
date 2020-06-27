#include "app_defs.h"

// STM32F10x Standard Peripherals Library
// http://stm32.kosyak.info/doc/index.html
extern void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);
extern void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup);

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
extern void RCC_APB1PeriphClockCmd(u32 RCC_APB1Periph, FunctionalState state);
extern void RCC_APB2PeriphClockCmd(u32 RCC_APB2Periph, FunctionalState state);
extern void RCC_AHBPeriphClockCmd(u32 RCC_AHBPeriph, FunctionalState state);
extern void RCC_APB1PeriphResetCmd(u32 RCC_APB1Periph, FunctionalState NewState);	
extern void RCC_APB2PeriphResetCmd(u32 RCC_APB2Periph, FunctionalState NewState);	

// Novation launchpad_pro.a
extern void init_gpios();
extern void init_device();
extern void init_exti();
// void init_surface();dr
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

typedef struct
{
  u32 DMA_PeripheralBaseAddr; 
  u32 DMA_MemoryBaseAddr;     
  u32 DMA_DIR;                
  u32 DMA_BufferSize;         
  u32 DMA_PeripheralInc;      
  u32 DMA_MemoryInc;          
  u32 DMA_PeripheralDataSize; 
  u32 DMA_MemoryDataSize;     
  u32 DMA_Mode;               
  u32 DMA_Priority;           
  u32 DMA_M2M;                
} DMA_InitTypeDef;
extern void DMA_DeInit(u32);
extern void DMA_Init(u32, DMA_InitTypeDef*);
extern void DMA_ITConfig(u32, u32, FunctionalState);
extern void DMA_Cmd	(u32 DMAy_Channelx, FunctionalState NewState);
typedef struct
{
  u16 SPI_Direction;           
  u16 SPI_Mode;                
  u16 SPI_DataSize;            
  u16 SPI_CPOL;                
  u16 SPI_CPHA;                
  u16 SPI_NSS;                 
  u16 SPI_BaudRatePrescaler;   
  u16 SPI_FirstBit;            
  u16 SPI_CRCPolynomial;       
} SPI_InitTypeDef;
extern void SPI_I2S_DeInit (u32 SPIx);
extern void SPI_I2S_DMACmd (u32 SPIx, u16 SPI_I2S_DMAReq, FunctionalState NewState);
extern void SPI_Init (u32 SPIx, SPI_InitTypeDef* SPI_InitStruct);
extern void SPI_Cmd (u32 SPIx, FunctionalState NewState);
extern void SPI_StructInit (SPI_InitTypeDef* SPI_InitStruct);	

typedef struct
{
  u16 TIM_Prescaler;         
  u16 TIM_CounterMode;       
  u16 TIM_Period;            
  u16 TIM_ClockDivision;     
  u8 TIM_RepetitionCounter;  
} TIM_TimeBaseInitTypeDef;
extern void TIM_ITConfig (u32 TIMx, u16 TIM_IT, FunctionalState NewState);
extern void TIM_PrescalerConfig (u32 TIMx, u16 Prescaler, u16 TIM_PSCReloadMode);
extern void TIM_SetCounter (u32 TIMx, u16 Counter);
extern void TIM_SetAutoreload (u32 TIMx, u16 Autoreload);
extern void TIM_UpdateRequestConfig (u32 TIMx, u16 TIM_UpdateSource);
extern void TIM_Cmd(u32 TIMx, FunctionalState NewState);
extern void GPIO_ResetBits(u32 GPIOx, u16 GPIO_Pin);
void custom_init_surface();

void custom_init_gpios();

extern volatile u8 LedStatus[10];

// Redefine main :b1:
int main() {
    NVIC_SetVectorTable(0x8000000,0x6400);  // base vector
    __asm__("cpsie i");    // enableIRQinterrupts();

    NVIC_PriorityGroupConfig(0x300);

    RCC_APB1PeriphClockCmd(0x18000000,1);
    
    init_device();  // this sets up device descriptor (0x0051 + device id from BL)
    custom_init_gpios();
    custom_init_surface();
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

void custom_init_gpios(){
    RCC_APB2PeriphClockCmd(0x33D, 1);
    *(u32*)0x4001080C = 0; // GPIOA_ODR
    *(u32*)0x40010804 = 0x44444111; // GPIOA_CRH
    *(u32*)0x40010800 = 0x14111111; // GPIOA_CRL
    *(u32*)0x40010C0C = 0x407; // GPIOB_ODR
    *(u32*)0x40010C04 = 0x94911111; // GPIOB_CRH
    *(u32*)0x40010C00 = 0x11144111; // GPIOB_CRL
    *(u32*)0x40011004 = 0x11111111; // GPIOC_CRH
    *(u32*)0x40011000 = 0x11111111; // GPIOC_CRL
}

// Register mappings: https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// + https://github.com/Focusrite-Novation/launchpad-firmware/blob/master/cortex/cxm3_registers.h
void custom_init_surface(){
    // Start SPI reset
    RCC_APB2PeriphResetCmd(0x1000, 1);
    RCC_APB1PeriphResetCmd(0x4000, 1);
    
    // Finish reset
    RCC_APB2PeriphResetCmd(0x1000, 0);
    RCC_APB1PeriphResetCmd(0x4000, 0);
    
    // Enable DMA1 timer
    RCC_AHBPeriphClockCmd(0x1, 1);
    
    DMA_DeInit(0x40020044);
    
    DMA_InitTypeDef DMA1_Channel4;
    
    DMA1_Channel4.DMA_BufferSize = 8; // 8 bit buffer
    DMA1_Channel4.DMA_DIR = 0; // Peripheral is source
    DMA1_Channel4.DMA_M2M = 0; // Not memory-to-memory transfer
    DMA1_Channel4.DMA_Priority = 0x2000; // High
    DMA1_Channel4.DMA_Mode = 0; // Non circular
    DMA1_Channel4.DMA_MemoryBaseAddr = (u32)&LedStatus;
    DMA1_Channel4.DMA_MemoryDataSize = 0; // 8 bit memory size
    DMA1_Channel4.DMA_MemoryInc = 0x80;
    DMA1_Channel4.DMA_PeripheralBaseAddr = 0x4000380C; // SPI2
    DMA1_Channel4.DMA_PeripheralDataSize = 0x100; // 16 bit peripheral size
    DMA1_Channel4.DMA_PeripheralInc = 0;
    
    DMA_Init(0x40020044, &DMA1_Channel4);
    DMA_ITConfig(0x40020044, 2, 1); // Enable transfer complete interrupts
    DMA_Cmd(0x40020044, 1); // Start DMA1 channel 4
    
    DMA_DeInit(0x40020058);
    
    DMA_InitTypeDef DMA1_Channel5;
    
    DMA1_Channel5.DMA_BufferSize = 8;
    DMA1_Channel5.DMA_DIR = 0x10; // Read from memory to peripheral, maybe lights?
    DMA1_Channel5.DMA_M2M = 0;
    DMA1_Channel5.DMA_Priority = 0x2000;
    DMA1_Channel5.DMA_Mode = 0;
    DMA1_Channel5.DMA_MemoryBaseAddr = (u32)&LedStatus + 0x20;
    DMA1_Channel5.DMA_MemoryDataSize = 0;
    DMA1_Channel5.DMA_MemoryInc = 0x80;
    DMA1_Channel5.DMA_PeripheralBaseAddr = 0x4000380C;
    DMA1_Channel5.DMA_PeripheralDataSize = 0;
    DMA1_Channel5.DMA_PeripheralInc = 0;
    
    DMA_Init(0x40020058, &DMA1_Channel5);
    DMA_ITConfig(0x40020058, 2, 1);
    DMA_Cmd(0x40020058, 1);

    // Enable SPI2 Clock
    RCC_APB1PeriphClockCmd(0x4000, 1);
    
    SPI_I2S_DeInit(0x40003800);
    
    SPI_InitTypeDef SPI2_Config;
    
    SPI2_Config.SPI_CPHA = 0;
    SPI2_Config.SPI_CPOL = 0;
    SPI2_Config.SPI_CRCPolynomial = 0;
    SPI2_Config.SPI_DataSize = 0;
    SPI2_Config.SPI_Direction = 0;
    SPI2_Config.SPI_BaudRatePrescaler = 0x10;
    SPI2_Config.SPI_FirstBit = 0x80; // LSB transmitted first;
    SPI2_Config.SPI_Mode = 0x104; // Master
    SPI2_Config.SPI_NSS = 0x200; // Slave controlled by software, not hardware
    
    SPI_Init(0x40003800, &SPI2_Config);
    SPI_I2S_DMACmd(0x40003800, 3, 1); // Enable send/receive over DMA
    SPI_Cmd(0x40003800, 1); // Enable SPI2
    
    RCC_APB1PeriphClockCmd(0x40000000, 1); // Enable TIM2 timer
    TIM_ITConfig(0x40000000, 1, 1); // Enable TIM2 Update interrupt
    *(u32*)0x40000010 = 0;// Reset Timer 2 interrupt status
    TIM_PrescalerConfig(0x40000000, 0x2F, 0);
    TIM_SetCounter(0x40000000, 0x13);
    TIM_SetAutoreload(0x40000000, 0x13);
    TIM_UpdateRequestConfig(0x40000000, 1);
    *(u32*)0xE000E100 |= 0x10000000; // Interrupt controller stuff for TIM2
    *(u32*)0xE000E40C |= 0x80;
    TIM_Cmd(0x40000000, 1); // Enable TIM2
    
    RCC_APB1PeriphClockCmd(0x40000400, 1);
    TIM_ITConfig(0x40000400, 1, 1);
    *(u32*)0x40000010 = 0;
    TIM_PrescalerConfig(0x40000400, 0x17, 0);
    TIM_SetCounter(0x40000400, 999);
    TIM_SetAutoreload(0x40000400, 999);
    TIM_UpdateRequestConfig(0x40000400, 1);
    *(u32*)0xE000E100 |= 0x20000000;
    *(u32*)0xE000E41C |= 0x6000;
    TIM_Cmd(0x40000400, 1);
    
    GPIO_ResetBits(0x40010800, 4);
}