#include "mk2.h"

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

void init_gpios() {
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

void init_surface() {
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
}

void init_timers() {
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