#include <ILI9163.h>

/*
 * ILI9163.c
 * Author: Harris Shallcross
 * Year: ~24/8/2014
 *
 *My Driver library, used for the ILI9163 128x128 LCD, easily available on eBay!
 *
 *More information can be found on my blog at:
 *www.hsel.co.uk
 *
 *Copyright (c) 2014 Harris Shallcross
 *
 *The code is released under the MIT License
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 */

extern volatile uint32_t MSec;
extern volatile uint32_t RTCC;
uint16_t XPix = 128, YPix = 160;
uint16_t BKGCol;

void Delay(uint32_t MS){
	volatile uint32_t MSS = MSec;
	while((MSec - MSS)<MS) asm volatile ("nop");
}

void SB(uint8_t Data, uint8_t DR){
	GPIO_ResetBits(G_LCDCS, P_LCS);
	if(DR == Dat) GPIO_SetBits(G_LCDSD, P_LDC);
	else GPIO_ResetBits(G_LCDSD, P_LDC);

	SPI_SendData8(S_LCD, Data);
	while(SPI_I2S_GetFlagStatus(S_LCD, SPI_I2S_FLAG_BSY) == SET);
	while(SPI_I2S_GetFlagStatus(S_LCD, SPI_I2S_FLAG_TXE) == RESET);
	GPIO_SetBits(G_LCDCS, P_LCS);
}

void SW(uint16_t Data){
	SPI_I2S_SendData16(S_LCD, Data);
	while(SPI_I2S_GetFlagStatus(S_LCD, SPI_I2S_FLAG_BSY) == SET);
	while(SPI_I2S_GetFlagStatus(S_LCD, SPI_I2S_FLAG_TXE) == RESET);
}

void SetAddr(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2){
	SB(0x2A, Reg);
	SB(0x00, Dat);
	SB(X1, Dat);
	SB(0x00, Dat);
	SB(X2, Dat);

	SB(0x2B, Reg);
	SB(0x00, Dat);
	SB(Y1, Dat);
	SB(0x00, Dat);
	SB(Y2, Dat);

	SB(0x2C, Reg);
}

void SetScrn(uint16_t Colour){
	uint32_t Cnt;

	SetAddr(0, 0, XPix-1, YPix-1);

	ConfigSPIDS(SPI_DataSize_16b);
	WriteAO(1);
	WriteCS(0);
	for(Cnt = 0; Cnt<XPix*YPix; Cnt++){
		SW(Colour);
	}
	WriteCS(1);
	ConfigSPIDS(SPI_DataSize_8b);
}

void ClrScrn(void){
	SetScrn(BKGCol);
}

void WritePix(uint16_t X, uint16_t Y, uint16_t Colour){
	SetAddr(X, Y, X, Y);
	PCol(Colour);
}

void PCol(uint16_t Colour){
	ConfigSPIDS(SPI_DataSize_16b);
	WriteAO(1);
	WriteCS(0);
	SW(Colour);
	WriteCS(1);
	ConfigSPIDS(SPI_DataSize_8b);
}

void SleepMode(uint8_t Mode){
	if(Mode == Sleep) SB(0x10, Reg);
	else SB(0x11, Reg);
	Delay(120);
}

void IdleMode(uint8_t Mode){
	if(Mode) SB(0x39, 0);
	else SB(0x38, 0);
}

void InvMode(uint8_t Mode){
	if(Mode==0) SB(0x20, Reg);
	else SB(0x21, Reg);
}

GPIO_InitTypeDef G;
SPI_InitTypeDef S;

void ILI9163Init(void){
	static uint8_t Init = 1;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	G.GPIO_Pin = P_LDC | P_LCS | P_RST;
	G.GPIO_Mode = GPIO_Mode_OUT;
	G.GPIO_OType = GPIO_OType_PP;
	G.GPIO_PuPd = GPIO_PuPd_NOPULL;
	G.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(G_LCDSD, &G);

	G.GPIO_Pin = P_MOSI | P_CLK;
	G.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(G_LCDSD, &G);

	GPIO_SetBits(GPIOA, P_RST);
	Delay(10);
	GPIO_ResetBits(GPIOA, P_RST);
	Delay(10);
	GPIO_SetBits(GPIOA, P_RST);

	GPIO_PinAFConfig(G_LCDSD, PS_MOSI, GA_LCD);
	GPIO_PinAFConfig(G_LCDSD, PS_CLK, GA_LCD);

	S.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	S.SPI_CPHA = SPI_CPHA_1Edge;
	S.SPI_CPOL = SPI_CPOL_Low;
	S.SPI_DataSize = SPI_DataSize_8b;
	S.SPI_FirstBit = SPI_FirstBit_MSB;
	S.SPI_Mode = SPI_Mode_Master;
	S.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(S_LCD, &S);
	SPI_Cmd(S_LCD, ENABLE);

	if(Init){
		Init = 0;
		Delay(100);
		GPIO_ResetBits(G_LCDCS, P_LCS);

		BKGCol = White;
		//BKGCol = Black;

		SB(0x01, Reg); //Software reset
		Delay(10);
		SB(0x11, Reg); //Exit Sleep
		Delay(20);

		SB(0x26, Reg); //Set default gamma
		SB(0x01, Dat);

		SB(0xC0, Reg); //Set Power Control 1
		SB(0x10, Dat);
		SB(0x04, Dat);

		SB(0xC1, Reg); //Set Power Control 2
		SB(0x04, Dat);
		//AVDD: 2xVCI1
		//VCL: -VCI1
		//VGH: 5xVCI1
		//VGL: 5xVCI1
		//VGH-VGL<=32v!

		SB(0xC2, Reg); //Set Power Control 3
		SB(0x03, Dat);
		SB(0x04, Dat);

		SB(0xC3, Reg); //Set Power Control 4 (Idle mode)
		SB(0x00, Dat);
		SB(0x07, Dat);

		SB(0xC5, Reg); //Set VCom Control 1
		SB(0x24, Dat); // VComH = 3v
		SB(0x7B, Dat); // VComL = -1v

		RotateScreen(DEG90, 0);

		SB(0x13, Reg); //Normal mode on

		SB(0x3A, Reg); //Set pixel mode
		SB(0x05, Dat);

		SB(0x29, Reg); //Display on

		InvMode(0);
		ClrScrn();
	}
}

void RotateScreen(S_Rotation R, uint8_t Mirror){
	uint8_t RGB = 0;

#ifdef RGB_BACKWARD
	RGB = (1<<3);
#endif

	if(!Mirror){
		switch(R){
		case DEG0:
			SB(0x36, Reg); //Set Memory access mode
			SB(0x00|RGB, Dat);
			XPix = 128;
			YPix = 160;
			break;

		case DEG90:
			SB(0x36, Reg); //Set Memory access mode
			SB(0x60|RGB, Dat);
			XPix = 160;
			YPix = 128;
			break;

		case DEG180:
			SB(0x36, Reg); //Set Memory access mode
			SB(0xC0|RGB, Dat);
			XPix = 128;
			YPix = 160;
			break;

		case DEG270:
			SB(0x36, Reg); //Set Memory access mode
			SB(0xA0|RGB, Dat);
			XPix = 160;
			YPix = 128;
			break;
		}
	}
	else{
		switch(R){
		case DEG0:
			SB(0x36, Reg); //Set Memory access mode
			SB(0x40|RGB, Dat);
			XPix = 128;
			YPix = 160;
			break;

		case DEG90:
			SB(0x36, Reg); //Set Memory access mode
			SB(0xF0|RGB, Dat);
			XPix = 160;
			YPix = 128;
			break;

		case DEG180:
			SB(0x36, Reg); //Set Memory access mode
			SB(0x80|RGB, Dat);
			XPix = 128;
			YPix = 160;
			break;

		case DEG270:
			SB(0x36, Reg); //Set Memory access mode
			SB(0x20|RGB, Dat);
			XPix = 160;
			YPix = 128;
			break;
		}
	}
}
