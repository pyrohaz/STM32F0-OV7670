#include <stm32f0xx_spi.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_exti.h>
#include <stm32f0xx_misc.h>
#include <stm32f0xx_syscfg.h>
#include "Sensor_config.h"
#include "ov7670reg.h"
#include "ILI9163.h"
#include "GFXC.h"

//GPIOB
#define C_D0		GPIO_Pin_0
#define C_D1		GPIO_Pin_1
#define C_D2		GPIO_Pin_2
#define C_D3		GPIO_Pin_3
#define C_D4		GPIO_Pin_4
#define C_D5		GPIO_Pin_5
#define C_D6		GPIO_Pin_6
#define C_D7		GPIO_Pin_7

//GPIOC
#define C_VSYNC		GPIO_Pin_0
#define C_RCK		GPIO_Pin_2
#define C_WR		GPIO_Pin_1
#define C_RRST		GPIO_Pin_5
#define C_WRST		GPIO_Pin_4

#define C_IGPIO		GPIOA
#define C_SCL		GPIO_Pin_0
#define C_SDA		GPIO_Pin_1

#define C_OVADDR	(0x42)

#define I_DLYL	6
#define I_DLYS	2

volatile uint32_t MSec = 0;

void SysTick_Handler(void){
	MSec++;
}

void Delayus(uint32_t D){
	D*=50;
	while(D--) asm volatile("nop");
}

void I_Start(void){
	GPIO_SetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
	GPIO_SetBits(C_IGPIO, C_SCL);
	Delayus(I_DLYL);
	GPIO_ResetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
	GPIO_ResetBits(C_IGPIO, C_SCL);
	Delayus(I_DLYL);
}

void I_Stop(void){
	GPIO_ResetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
	GPIO_SetBits(C_IGPIO, C_SCL);
	Delayus(I_DLYL);
	GPIO_SetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
}

void I_NoAck(void){
	GPIO_SetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
	GPIO_SetBits(C_IGPIO, C_SCL);
	Delayus(I_DLYL);
	GPIO_ResetBits(C_IGPIO, C_SCL);
	Delayus(I_DLYL);
	GPIO_ResetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
}

uint8_t I_WB(uint8_t D){
	uint8_t n, a;

	for(n = 0; n<8; n++){
		if((D<<n)&0x80) GPIO_SetBits(C_IGPIO, C_SDA);
		else GPIO_ResetBits(C_IGPIO, C_SDA);
		Delayus(I_DLYL);
		GPIO_SetBits(C_IGPIO, C_SCL);
		Delayus(I_DLYL);
		GPIO_ResetBits(C_IGPIO, C_SCL);
		Delayus(I_DLYL);
	}

	Delayus(I_DLYS);
	GPIO_SetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
	GPIO_SetBits(C_IGPIO, C_SCL);
	Delayus(I_DLYS);

	if(GPIO_ReadInputDataBit(C_IGPIO, C_SDA)) a = 0;
	else a = 1;
	GPIO_ResetBits(C_IGPIO, C_SCL);
	Delayus(I_DLYL);

	return a;
}

uint8_t I_RB(void){
	uint8_t n, D = 0;

	GPIO_SetBits(C_IGPIO, C_SDA);
	Delayus(I_DLYL);
	for(n = 0; n<8; n++){
		Delayus(I_DLYL);
		GPIO_SetBits(C_IGPIO, C_SCL);
		Delayus(I_DLYL);
		D<<=1;
		if(GPIO_ReadInputDataBit(C_IGPIO, C_SDA)) D++;
		GPIO_ResetBits(C_IGPIO, C_SCL);
		Delayus(I_DLYL);
	}

	return D;
}

uint8_t C_Write(uint8_t Reg, uint8_t Dat){
	I_Start();
	if(!I_WB(C_OVADDR)){
		I_Stop();
		return 0;
	}

	Delayus(I_DLYS);
	if(!I_WB(Reg)){
		I_Stop();
		return 0;
	}

	Delayus(I_DLYS);
	if(!I_WB(Dat)){
		I_Stop();
		return 0;
	}
	I_Stop();

	return 1;
}

uint8_t C_Read(uint8_t Reg, uint8_t *Dat){
	I_Start();
	if(!I_WB(C_OVADDR)){
		I_Stop();
		return 0;
	}

	Delayus(I_DLYS);
	if(!I_WB(Reg)){
		I_Stop();
		return 0;
	}
	I_Stop();

	Delayus(I_DLYS);

	I_Start();
	if(!I_WB(C_OVADDR|1)){
		I_Stop();
		return 0;
	}

	Delayus(I_DLYS);
	*Dat = I_RB();
	I_NoAck();
	I_Stop();

	return 1;
}

uint8_t C_Init(void){
	int32_t n;
	uint8_t V;

	if(!C_Write(0x12, 0x80)) return 0;
	Delay(10);
	if(!C_Read(0x0B, &V)) return 0;

	if(V == 0x73){
		for(n = 0; n<OV7670_REG_NUM; n++){
			if(!C_Write(OV7670_reg[n][0], OV7670_reg[n][1])) return 0;
		}

		//AEC AGC AWB Banding
		if(!C_Read(REG_COM8, &V)) return 0;
		//if(!C_Write(REG_COM8, V & ~((1<<5)|0))) return 0;

		//Night mode
		if(!C_Read(REG_COM11, &V)) return 0;
		//if(!C_Write(REG_COM11, V & ~(0x90))) return 0;

		//Gamma
		if(!C_Read(REG_COM13, &V)) return 0;
		//if(!C_Write(REG_COM13, V & ~(0x80))) return 0;

		if(!C_Read(0x6B, &V)) return 0;
		if(!C_Write(0x6B, V|(1<<7))) return 0;

		//if(!C_Read(REG_CLKRC, &V)) return 0;
		//if(!C_Write(REG_CLKRC, (V&~(0x3F)) | 0x2)) return 0;
	}

	return 1;
}

volatile uint8_t GetFrame = 0;
void EXTI0_1_IRQHandler(void){
	static uint8_t GrabFrame = 0;
	if(EXTI_GetITStatus(EXTI_Line0)){
		EXTI_ClearITPendingBit(EXTI_Line0);

		//Well synchronized
		/*if(GPIO_ReadInputDataBit(GPIOC, C_VSYNC)){ //Rising edge
			if(GrabFrame){
				GrabFrame = 0;
				GetFrame = 0;
			}
			if(GetFrame){
				GrabFrame = 1;
				GPIO_ResetBits(GPIOC, C_WRST | C_WR);
			}
		}
		else{ //Falling edge
			if(GrabFrame){
				GPIO_SetBits(GPIOC, C_WRST | C_WR);
			}
		}*/

		//Fast and sloppy, can overrun!
		if(GPIO_ReadInputDataBit(GPIOC, C_VSYNC)){ //Rising edge
			if(GetFrame){
				GetFrame = 0;
				GPIO_ResetBits(GPIOC, C_WRST | C_WR);
			}
		}
		else{ //Falling edge
			if(!GetFrame){
				GPIO_SetBits(GPIOC, C_WRST | C_WR);
			}
		}
	}
}

//#define NO_SCALE
#define PIXEL_SKIP
//#define PIXMEM

GPIO_InitTypeDef G;
NVIC_InitTypeDef N;
EXTI_InitTypeDef E;

uint16_t pbuf[32][40];

int main(void)
{
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_PLLCmd(DISABLE);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
	RCC_PLLCmd(ENABLE);
	while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	SysTick_Config(SystemCoreClock/1000);

	ILI9163Init();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	G.GPIO_Pin = C_SCL | C_SDA;
	G.GPIO_Mode = GPIO_Mode_OUT;
	G.GPIO_OType = GPIO_OType_OD;
	G.GPIO_PuPd = GPIO_PuPd_UP;
	G.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(C_IGPIO, &G);

	G.GPIO_Pin = C_D0|C_D1|C_D2|C_D3|C_D4|C_D5|C_D6|C_D7;
	G.GPIO_Mode = GPIO_Mode_IN;
	G.GPIO_OType = GPIO_OType_PP;
	G.GPIO_PuPd = GPIO_PuPd_NOPULL;
	G.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &G);

	G.GPIO_Pin = C_VSYNC;
	G.GPIO_Mode = GPIO_Mode_IN;
	G.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &G);

	G.GPIO_Pin = C_RCK|C_WR|C_RRST|C_WRST | GPIO_Pin_12;
	G.GPIO_Mode = GPIO_Mode_OUT;
	G.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &G);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);

	N.NVIC_IRQChannel = EXTI0_1_IRQn;
	N.NVIC_IRQChannelCmd = ENABLE;
	N.NVIC_IRQChannelPriority = 0;
	NVIC_Init(&N);

	E.EXTI_Line = EXTI_Line0;
	E.EXTI_LineCmd = ENABLE;
	E.EXTI_Mode = EXTI_Mode_Interrupt;
	E.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&E);

	while(!C_Init());

	int32_t x, y, n;
	uint16_t pa, pb, px, pxb;
	uint32_t mss = 0, mse, fr, xp, yi, frl;
	int32_t yc = 0, yco = 0;
	float ycs = 0;
	uint8_t xt = 0, c = 0, r, g, b, rb, gb, bb;
	int32_t rd, gd, bd;

	while(1)
	{

		mss = MSec;


		GetFrame = 1;
		GPIO_ResetBits(GPIOC, GPIO_Pin_12);
		while(GetFrame);
		GPIO_SetBits(GPIOC, GPIO_Pin_12);

		//Reset read pointer
		GPIO_ResetBits(GPIOC, C_RRST);
		GPIO_ResetBits(GPIOC, C_RCK);
		GPIO_SetBits(GPIOC, C_RCK);
		GPIO_SetBits(GPIOC, C_RCK);
		GPIO_SetBits(GPIOC, C_RRST);

		//Initialize LCD for data stream
		SetAddr(0, 0, XPix-1, YPix-1);
		WriteCS(0);
		WriteAO(1);

#ifdef NO_SCALE
		for(y = 0; y<128; y++){
			for(x = 0; x<320; x++){

				GPIOC->BRR = C_RCK;
				GPIOC->BSRR = C_RCK;
				asm volatile("nop");
				pa = GPIOB->IDR;

				GPIOC->BRR = C_RCK;
				GPIOC->BSRR = C_RCK;
				asm volatile("nop");
				pb = GPIOB->IDR;

				if(x<160){
					while(SPI_I2S_GetFlagStatus(S_LCD, SPI_I2S_FLAG_BSY) == SET);
					SPI_I2S_SendData16(S_LCD, (pa&255)|(pb<<8));
				}
			}
		}
#endif
#ifdef PIXEL_SKIP
		yco = -1;
		px = 0;
		ycs = 0;

		for(y = 0; y<240; y++){
			ycs = (float)y*128.0f/240.0f;
			if((int)ycs != yco){
				yco = ycs;
				xt = 1;

				for(x = 0; x<320; x++){
					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
					asm volatile("nop");
					pa = GPIOB->IDR;

					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
					asm volatile("nop");
					pb = GPIOB->IDR;

					if(xt){
						while(SPI_I2S_GetFlagStatus(S_LCD, SPI_I2S_FLAG_TXE) == RESET);
						SPI_I2S_SendData16(S_LCD, (pa&255)|(pb<<8));
					}
					xt^=1;
				}
			}
			else{
				//Scrap line
				for(x = 0; x<320; x++){
					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
				}
			}
		}
#endif
#ifdef PIXMEM
		yc = 0;
		yco = -1;
		yi = 0;
		for(y = 0; y<240; y++){
			yc += 273;
			ycs = yc>>11;
			if(ycs != yco){
				yco = ycs;
				xt = 0;

				for(x = 0; x<320; x++){
					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
					asm volatile("nop");
					pa = GPIOB->IDR;

					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
					asm volatile("nop");
					pb = GPIOB->IDR;

					if(xt == 7){
						px = (pa&255)|(pb<<8);
						pbuf[(y<<5)/240][x>>3] = px;
						xt = 0;
					}
					xt++;
				}
			}
			else{
				//Scrap a line
				for(x = 0; x<320; x++){
					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
					GPIOC->BRR = C_RCK;
					GPIOC->BSRR = C_RCK;
				}
			}
		}

		for(y = 0; y<YPix; y++){
			for(x = 0; x<XPix; x++){
				SW(pbuf[(y<<5)/YPix][x>>3]);
			}
		}
#endif

		/*mse = MSec;
		xp = PStr("Frame Rate: ", 0, 0, 0, White, Black);
		xp = PNum(frl, xp, 0, 0, 0, White, Black);
		frl = 1000/(mse-mss);*/
		WriteCS(1);
	}
}
