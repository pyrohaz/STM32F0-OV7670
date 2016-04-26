#ifndef ILI9163_H
#define ILI9163_H

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>

#ifndef NULL
#define NULL (void *)0
#endif

typedef enum WModesS{
	Dat = 1,
	Reg = 0
} WModes;

typedef enum SleepModesS{
	Sleep,
	Awake,
} SleepModes;

typedef enum SMd{
	SDmd = 0,
	LCDmd
} SMd;


#define Black 	0x0000
#define Blue 	0x0010
#define Red 	0x8000
#define Magenta 0x8010
#define Green 	0x0400
#define Cyan 	0x0410
#define Yellow 	0x8400
#define White 	0xFFFF

#define G_LCDCS		GPIOA
#define G_LCDSD		GPIOA

#define P_LDC		GPIO_Pin_4
#define P_RST		GPIO_Pin_3
#define P_LCS		GPIO_Pin_2

#define P_MOSI		GPIO_Pin_7
#define P_CLK		GPIO_Pin_5
#define PS_MOSI		GPIO_PinSource7
#define PS_CLK		GPIO_PinSource5
#define GA_LCD		GPIO_AF_0

#define S_LCD		SPI1

#define WriteCS(CS) GPIO_WriteBit(G_LCDCS, P_LCS, CS)
#define WriteAO(AO) GPIO_WriteBit(G_LCDSD, P_LDC, AO)
#define ConfigSPIDS(Size) SPI_DataSizeConfig(S_LCD, Size)

//#define RGB_BACKWARD

typedef enum{
	DEG0 = 0,
	DEG90 = 1,
	DEG180 = 2,
	DEG270 = 3
} S_Rotation;

extern uint16_t XPix, YPix;
extern uint16_t BKGCol;

void SPI_Mode(SMd Mode);

void Delay(uint32_t);

void ILI9163Init(void);

uint16_t EToS(uint8_t);

void SB(uint8_t, WModes);
void SW(uint16_t);

void SetAddr(uint8_t, uint8_t, uint8_t, uint8_t);
void SetScrn(uint16_t);
void ClrScrn(void);
void WritePix(uint16_t, uint16_t, uint16_t);
void PCol(uint16_t);
void RotateScreen(S_Rotation, uint8_t);

void SleepMode(uint8_t);
void InvMode(uint8_t);

#endif
