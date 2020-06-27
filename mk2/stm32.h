#include "../include/app_defs.h" // TODO this needs to be done properly later

// STM32F10x Standard Peripherals Library
// http://stm32.kosyak.info/doc/index.html

// Register mappings: https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// + https://github.com/Focusrite-Novation/launchpad-firmware/blob/master/cortex/cxm3_registers.h

extern void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);
extern void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup);

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
extern void RCC_APB1PeriphClockCmd(u32 RCC_APB1Periph, FunctionalState state);
extern void RCC_APB2PeriphClockCmd(u32 RCC_APB2Periph, FunctionalState state);
extern void RCC_AHBPeriphClockCmd(u32 RCC_AHBPeriph, FunctionalState state);
extern void RCC_APB1PeriphResetCmd(u32 RCC_APB1Periph, FunctionalState NewState);	
extern void RCC_APB2PeriphResetCmd(u32 RCC_APB2Periph, FunctionalState NewState);

typedef struct {
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
extern void DMA_Cmd(u32 DMAy_Channelx, FunctionalState NewState);

typedef struct {
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

extern void SPI_I2S_DeInit(u32 SPIx);
extern void SPI_I2S_DMACmd(u32 SPIx, u16 SPI_I2S_DMAReq, FunctionalState NewState);
extern void SPI_Init(u32 SPIx, SPI_InitTypeDef* SPI_InitStruct);
extern void SPI_Cmd(u32 SPIx, FunctionalState NewState);
extern void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);	

typedef struct {
  u16 TIM_Prescaler;         
  u16 TIM_CounterMode;       
  u16 TIM_Period;            
  u16 TIM_ClockDivision;     
  u8 TIM_RepetitionCounter;  
} TIM_TimeBaseInitTypeDef;

extern void TIM_ITConfig(u32 TIMx, u16 TIM_IT, FunctionalState NewState);
extern void TIM_PrescalerConfig(u32 TIMx, u16 Prescaler, u16 TIM_PSCReloadMode);
extern void TIM_SetCounter(u32 TIMx, u16 Counter);
extern void TIM_SetAutoreload(u32 TIMx, u16 Autoreload);
extern void TIM_UpdateRequestConfig(u32 TIMx, u16 TIM_UpdateSource);
extern void TIM_Cmd(u32 TIMx, FunctionalState NewState);
extern void GPIO_ResetBits(u32 GPIOx, u16 GPIO_Pin);