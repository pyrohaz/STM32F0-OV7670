#include "GFXC.h"

/*
 * GFXC.c
 * Author: Harris Shallcross
 * Year: ~24/8/2014
 *
 *The rough unfinished version of my colour LCD graphics
 *library. As of now, maximum supported resolution is 255x255
 *though this is limited by the function parameters being
 *passed as unsigned 8 bit integers. Improvements are always
 *being made!
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

const uint16_t RGB332to565lookupTable[256] = {
    0x0000, 0x000a, 0x0015, 0x001f, 0x0120, 0x012a, 0x0135, 0x013f,
    0x0240, 0x024a, 0x0255, 0x025f, 0x0360, 0x036a, 0x0375, 0x037f,
    0x0480, 0x048a, 0x0495, 0x049f, 0x05a0, 0x05aa, 0x05b5, 0x05bf,
    0x06c0, 0x06ca, 0x06d5, 0x06df, 0x07e0, 0x07ea, 0x07f5, 0x07ff,
    0x2000, 0x200a, 0x2015, 0x201f, 0x2120, 0x212a, 0x2135, 0x213f,
    0x2240, 0x224a, 0x2255, 0x225f, 0x2360, 0x236a, 0x2375, 0x237f,
    0x2480, 0x248a, 0x2495, 0x249f, 0x25a0, 0x25aa, 0x25b5, 0x25bf,
    0x26c0, 0x26ca, 0x26d5, 0x26df, 0x27e0, 0x27ea, 0x27f5, 0x27ff,
    0x4800, 0x480a, 0x4815, 0x481f, 0x4920, 0x492a, 0x4935, 0x493f,
    0x4a40, 0x4a4a, 0x4a55, 0x4a5f, 0x4b60, 0x4b6a, 0x4b75, 0x4b7f,
    0x4c80, 0x4c8a, 0x4c95, 0x4c9f, 0x4da0, 0x4daa, 0x4db5, 0x4dbf,
    0x4ec0, 0x4eca, 0x4ed5, 0x4edf, 0x4fe0, 0x4fea, 0x4ff5, 0x4fff,
    0x6800, 0x680a, 0x6815, 0x681f, 0x6920, 0x692a, 0x6935, 0x693f,
    0x6a40, 0x6a4a, 0x6a55, 0x6a5f, 0x6b60, 0x6b6a, 0x6b75, 0x6b7f,
    0x6c80, 0x6c8a, 0x6c95, 0x6c9f, 0x6da0, 0x6daa, 0x6db5, 0x6dbf,
    0x6ec0, 0x6eca, 0x6ed5, 0x6edf, 0x6fe0, 0x6fea, 0x6ff5, 0x6fff,
    0x9000, 0x900a, 0x9015, 0x901f, 0x9120, 0x912a, 0x9135, 0x913f,
    0x9240, 0x924a, 0x9255, 0x925f, 0x9360, 0x936a, 0x9375, 0x937f,
    0x9480, 0x948a, 0x9495, 0x949f, 0x95a0, 0x95aa, 0x95b5, 0x95bf,
    0x96c0, 0x96ca, 0x96d5, 0x96df, 0x97e0, 0x97ea, 0x97f5, 0x97ff,
    0xb000, 0xb00a, 0xb015, 0xb01f, 0xb120, 0xb12a, 0xb135, 0xb13f,
    0xb240, 0xb24a, 0xb255, 0xb25f, 0xb360, 0xb36a, 0xb375, 0xb37f,
    0xb480, 0xb48a, 0xb495, 0xb49f, 0xb5a0, 0xb5aa, 0xb5b5, 0xb5bf,
    0xb6c0, 0xb6ca, 0xb6d5, 0xb6df, 0xb7e0, 0xb7ea, 0xb7f5, 0xb7ff,
    0xd800, 0xd80a, 0xd815, 0xd81f, 0xd920, 0xd92a, 0xd935, 0xd93f,
    0xda40, 0xda4a, 0xda55, 0xda5f, 0xdb60, 0xdb6a, 0xdb75, 0xdb7f,
    0xdc80, 0xdc8a, 0xdc95, 0xdc9f, 0xdda0, 0xddaa, 0xddb5, 0xddbf,
    0xdec0, 0xdeca, 0xded5, 0xdedf, 0xdfe0, 0xdfea, 0xdff5, 0xdfff,
    0xf800, 0xf80a, 0xf815, 0xf81f, 0xf920, 0xf92a, 0xf935, 0xf93f,
    0xfa40, 0xfa4a, 0xfa55, 0xfa5f, 0xfb60, 0xfb6a, 0xfb75, 0xfb7f,
    0xfc80, 0xfc8a, 0xfc95, 0xfc9f, 0xfda0, 0xfdaa, 0xfdb5, 0xfdbf,
    0xfec0, 0xfeca, 0xfed5, 0xfedf, 0xffe0, 0xffea, 0xfff5, 0xffff
};

char *Chars = HFont1;
const char StockF[] =
{
		0x00, 0x00, 0x00, 0x00, 0x00 // 20
		,0x00, 0x00, 0x5f, 0x00, 0x00 // 21 !
		,0x00, 0x07, 0x00, 0x07, 0x00 // 22 "
		,0x14, 0x7f, 0x14, 0x7f, 0x14 // 23 #
		,0x24, 0x2a, 0x7f, 0x2a, 0x12 // 24 $
		,0x23, 0x13, 0x08, 0x64, 0x62 // 25 %
		,0x36, 0x49, 0x55, 0x22, 0x50 // 26 &
		,0x00, 0x05, 0x03, 0x00, 0x00 // 27 '
		,0x00, 0x1c, 0x22, 0x41, 0x00 // 28 (
		,0x00, 0x41, 0x22, 0x1c, 0x00 // 29 )
		,0x14, 0x08, 0x3e, 0x08, 0x14 // 2a *
		,0x08, 0x08, 0x3e, 0x08, 0x08 // 2b +
		,0x00, 0x50, 0x30, 0x00, 0x00 // 2c ,
		,0x08, 0x08, 0x08, 0x08, 0x08 // 2d -
		,0x00, 0x60, 0x60, 0x00, 0x00 // 2e .
		,0x20, 0x10, 0x08, 0x04, 0x02 // 2f /
		,0x3e, 0x51, 0x49, 0x45, 0x3e // 30 0
		,0x00, 0x42, 0x7f, 0x40, 0x00 // 31 1
		,0x42, 0x61, 0x51, 0x49, 0x46 // 32 2
		,0x21, 0x41, 0x45, 0x4b, 0x31 // 33 3
		,0x18, 0x14, 0x12, 0x7f, 0x10 // 34 4
		,0x27, 0x45, 0x45, 0x45, 0x39 // 35 5
		,0x3c, 0x4a, 0x49, 0x49, 0x30 // 36 6
		,0x01, 0x71, 0x09, 0x05, 0x03 // 37 7
		,0x36, 0x49, 0x49, 0x49, 0x36 // 38 8
		,0x06, 0x49, 0x49, 0x29, 0x1e // 39 9
		,0x00, 0x36, 0x36, 0x00, 0x00 // 3a :
		,0x00, 0x56, 0x36, 0x00, 0x00 // 3b ;
		,0x08, 0x14, 0x22, 0x41, 0x00 // 3c <
		,0x14, 0x14, 0x14, 0x14, 0x14 // 3d =
		,0x00, 0x41, 0x22, 0x14, 0x08 // 3e >
		,0x02, 0x01, 0x51, 0x09, 0x06 // 3f ?
		,0x32, 0x49, 0x79, 0x41, 0x3e // 40 @
		,0x7e, 0x11, 0x11, 0x11, 0x7e // 41 A
		,0x7f, 0x49, 0x49, 0x49, 0x36 // 42 B
		,0x3e, 0x41, 0x41, 0x41, 0x22 // 43 C
		,0x7f, 0x41, 0x41, 0x22, 0x1c // 44 D
		,0x7f, 0x49, 0x49, 0x49, 0x41 // 45 E
		,0x7f, 0x09, 0x09, 0x09, 0x01 // 46 F
		,0x3e, 0x41, 0x49, 0x49, 0x7a // 47 G
		,0x7f, 0x08, 0x08, 0x08, 0x7f // 48 H
		,0x00, 0x41, 0x7f, 0x41, 0x00 // 49 I
		,0x20, 0x40, 0x41, 0x3f, 0x01 // 4a J
		,0x7f, 0x08, 0x14, 0x22, 0x41 // 4b K
		,0x7f, 0x40, 0x40, 0x40, 0x40 // 4c L
		,0x7f, 0x02, 0x0c, 0x02, 0x7f // 4d M
		,0x7f, 0x04, 0x08, 0x10, 0x7f // 4e N
		,0x3e, 0x41, 0x41, 0x41, 0x3e // 4f O
		,0x7f, 0x09, 0x09, 0x09, 0x06 // 50 P
		,0x3e, 0x41, 0x51, 0x21, 0x5e // 51 Q
		,0x7f, 0x09, 0x19, 0x29, 0x46 // 52 R
		,0x46, 0x49, 0x49, 0x49, 0x31 // 53 S
		,0x01, 0x01, 0x7f, 0x01, 0x01 // 54 T
		,0x3f, 0x40, 0x40, 0x40, 0x3f // 55 U
		,0x1f, 0x20, 0x40, 0x20, 0x1f // 56 V
		,0x3f, 0x40, 0x38, 0x40, 0x3f // 57 W
		,0x63, 0x14, 0x08, 0x14, 0x63 // 58 X
		,0x07, 0x08, 0x70, 0x08, 0x07 // 59 Y
		,0x61, 0x51, 0x49, 0x45, 0x43 // 5a Z
		,0x00, 0x7f, 0x41, 0x41, 0x00 // 5b [
		,0x02, 0x04, 0x08, 0x10, 0x20 // 5c
		,0x00, 0x41, 0x41, 0x7f, 0x00 // 5d ]
		,0x04, 0x02, 0x01, 0x02, 0x04 // 5e ^
		,0x40, 0x40, 0x40, 0x40, 0x40 // 5f _
		,0x00, 0x01, 0x02, 0x04, 0x00 // 60 `
		,0x20, 0x54, 0x54, 0x54, 0x78 // 61 a
		,0x7f, 0x48, 0x44, 0x44, 0x38 // 62 b
		,0x38, 0x44, 0x44, 0x44, 0x20 // 63 c
		,0x38, 0x44, 0x44, 0x48, 0x7f // 64 d
		,0x38, 0x54, 0x54, 0x54, 0x18 // 65 e
		,0x08, 0x7e, 0x09, 0x01, 0x02 // 66 f
		,0x0c, 0x52, 0x52, 0x52, 0x3e // 67 g
		,0x7f, 0x08, 0x04, 0x04, 0x78 // 68 h
		,0x00, 0x44, 0x7d, 0x40, 0x00 // 69 i
		,0x20, 0x40, 0x44, 0x3d, 0x00 // 6a j
		,0x7f, 0x10, 0x28, 0x44, 0x00 // 6b k
		,0x00, 0x41, 0x7f, 0x40, 0x00 // 6c l
		,0x7c, 0x04, 0x18, 0x04, 0x78 // 6d m
		,0x7c, 0x08, 0x04, 0x04, 0x78 // 6e n
		,0x38, 0x44, 0x44, 0x44, 0x38 // 6f o
		,0x7c, 0x14, 0x14, 0x14, 0x08 // 70 p
		,0x08, 0x14, 0x14, 0x18, 0x7c // 71 q
		,0x7c, 0x08, 0x04, 0x04, 0x08 // 72 r
		,0x48, 0x54, 0x54, 0x54, 0x20 // 73 s
		,0x04, 0x3f, 0x44, 0x40, 0x20 // 74 t
		,0x3c, 0x40, 0x40, 0x20, 0x7c // 75 u
		,0x1c, 0x20, 0x40, 0x20, 0x1c // 76 v
		,0x3c, 0x40, 0x30, 0x40, 0x3c // 77 w
		,0x44, 0x28, 0x10, 0x28, 0x44 // 78 x
		,0x0c, 0x50, 0x50, 0x50, 0x3c // 79 y
		,0x44, 0x64, 0x54, 0x4c, 0x44 // 7a z
		,0x00, 0x08, 0x36, 0x41, 0x00 // 7b {
		,0x00, 0x00, 0x7f, 0x00, 0x00 // 7c |
		,0x00, 0x41, 0x36, 0x08, 0x00 // 7d }
		,0x10, 0x08, 0x08, 0x10, 0x08 // 7e
		,0x78, 0x46, 0x41, 0x46, 0x78 // 7f
		,0xFF, 0x00, 0x00, 0x00, 0x00 // Current char line
};



uint16_t ColIP(uint16_t Col, uint8_t Perc){
	uint16_t RCol = 0;
	uint32_t PercApr = (Perc*2622)>>10;

	switch(Col){
	case Blue: RCol = (PercApr>>3) >= 31 ? 31 : (PercApr>>3); break;
	case Green: RCol = (PercApr>>2)<<5 >= 63<<5 ? 63<<5 : (PercApr>>2)<<5; break;
	case Red: RCol = (PercApr>>3)<<11 >= 31<<11 ? 31<<11 : (PercApr>>3)<<11; break;
	}

	return RCol;
}

uint16_t ColP(uint8_t RedC, uint8_t BlueC, uint8_t GreenC){
	return ColIP(Red, RedC)|ColIP(Green, GreenC)|ColIP(Blue, BlueC);
}



//Convert 4bit RGB121 into 16bit RGB565
uint16_t Conv16C16BPix(uint16_t Px){
	uint16_t TPix = 0;

	TPix = 31*(Px&1);
	TPix |= ((21*(Px>>1))<<5);
	TPix |= ((31*(Px>>3))<<11);

	return TPix;
}

//Convert a buffer of 4bit packed colour (2bit/byte) into a 16bit buffer
void Conv16C16BBuf(uint8_t *BufI, uint16_t *BufO){
	uint32_t Cnt;

	for(Cnt = 0; Cnt<128; Cnt+=2){
		BufO[Cnt] = Conv16C16BPix(BufI[Cnt>>1]&15);
		BufO[Cnt+1] = Conv16C16BPix(BufI[Cnt>>1]>>4);
	}
}

void Conv565RGB(uint16_t C, uint8_t *R, uint8_t *G, uint8_t *B){
	*R = C&31;
	*G = (C>>5)&63;
	*B = (C>>11);
}

void Conv332RGB(uint8_t C, uint8_t *R, uint8_t *G, uint8_t *B){
	*R = C&7;
	*G = (C>>3)&7;
	*B = (C>>6);
}

uint16_t ConvRGB565T(uint8_t R, uint8_t G, uint8_t B){
	uint16_t Pix;

	Pix = B>>3;
	Pix |= (G>>2)<<5;
	Pix |= (R>>3)<<11;

	return Pix;
}

uint8_t ConvRGB565_332(uint16_t Px){
	uint8_t r, g, b, p;

	b = Px>>11;
	g = (Px>>5)&63;
	r = Px&31;

	p = (r>>2)<<5;
	p |= (g>>3)<<2;
	p |= (b>>3);

	return p;
}

uint16_t ConvRGB332_565(uint8_t Px){
	uint8_t r, g, b;
	uint16_t p;

	r = Px>>5;
	g = (Px>>2)&7;
	b = Px&3;

	p = r<<13;
	p |= g<<8;
	p |= b<<3;

	return p;
}

int16_t PChar(uint16_t Ch, uint8_t X, uint8_t Y, uint8_t Size, uint32_t Col, uint32_t BKCol){
	uint16_t XCnt, YCnt;
	uint16_t CharAddr, YPos;

	if(Ch<0x20) return X;
	Ch-=0x20;
	Ch*=5;

	SetAddr(X, Y, X + 4 +(Size*5) + LetterSpace, Y + 7 + (Size<<3));

	ConfigSPIDS(SPI_DataSize_16b);
	WriteAO(1);
	WriteCS(0);

	for(YCnt = 0; YCnt<8*(Size+1); YCnt++){
		for(XCnt = 0; XCnt<(5*(1+Size)) + LetterSpace; XCnt++){

#ifdef FAST_CHAR
			switch(Size){
			case 0:
				CharAddr = XCnt;
				YPos = YCnt;
				break;
			case 1:
				CharAddr = XCnt>>1;
				YPos = YCnt>>1;
				break;
			case 2:
				CharAddr = (XCnt*86)>>8;
				YPos = (YCnt*86)>>8;
				break;
			case 3:
				CharAddr = XCnt>>2;
				YPos = YCnt>>2;
				break;
			case 4:
				CharAddr = (51*XCnt)>>8;
				YPos = (51*YCnt)>>8;
				break;
			}
#else
			//Slow, supports all sizes up to the screen max!
			CharAddr = XCnt/(Size+1);
			YPos = YCnt/(Size+1);
#endif

			if(XCnt<(5*(Size+1)) && (Chars[Ch+CharAddr] & 1<<YPos)) SW(Col);
			else SW(BKCol);
		}
	}

	WriteCS(1);
	ConfigSPIDS(SPI_DataSize_8b);

	return X+(1+Size)*5+LetterSpace;
}
/*
int16_t PChar(uint16_t Ch, uint8_t X, uint8_t Y, uint8_t Size, uint32_t Col, uint32_t BKCol){
	uint16_t XCnt, YCnt;
	uint16_t CharAddr, YPos;

	Ch-=0x20;
	Ch*=5;

	SetAddr(X, Y, X + 4 +(Size*5) + LetterSpace, Y + 7 + (Size<<3));

	for(YCnt = 0; YCnt<8*(Size+1); YCnt++){
		for(XCnt = 0; XCnt<(5*(1+Size)) + LetterSpace; XCnt++){

#ifdef FAST_CHAR
			switch(Size){
			case 0:
				CharAddr = XCnt;
				YPos = YCnt;
				break;
			case 1:
				CharAddr = XCnt>>1;
				YPos = YCnt>>1;
				break;
			case 2:
				CharAddr = (XCnt*86)>>8;
				YPos = (YCnt*86)>>8;
				break;
			case 3:
				CharAddr = XCnt>>2;
				YPos = YCnt>>2;
				break;
			case 4:
				CharAddr = (51*XCnt)>>8;
				YPos = (51*YCnt)>>8;
				break;
			}
#else
			//Slow, supports all sizes up to the screen max!
			CharAddr = XCnt/(Size+1);
			YPos = YCnt/(Size+1);
#endif

			if(XCnt<(5*(Size+1)) && (Chars[Ch+CharAddr] & 1<<YPos)) PCol(Col);
			else PCol(BKCol);
		}
	}

	return X+(1+Size)*5+LetterSpace;
}*/

int16_t PStr(char* Str, uint8_t X, uint8_t Y, uint8_t Size, uint32_t Col, uint32_t BKCol){
	uint8_t SCnt, StrL;

	StrL = StrLen(Str);

	for(SCnt = 0; SCnt<StrL; SCnt++){
		X = PChar(Str[SCnt], X, Y, Size, Col, BKCol);
	}

	return X;
}

int16_t PStrL(char* Str, uint8_t X, uint8_t Y, uint8_t Size, uint32_t Col, uint32_t BKCol){
	uint8_t SCnt, StrL;

	StrL = StrLen(Str);

	for(SCnt = 0; SCnt<StrL; SCnt++){
		X = PChar(Str[SCnt], X, Y, Size, Col, BKCol);
		if(X>XPix-5*(Size+1)){
			Y+=8*(Size+1);
			X = 0;
		}
	}

	return X;
}

int16_t PNum(int32_t Num, uint8_t X, uint8_t Y, uint8_t Pad, uint8_t Size, uint32_t Col, uint32_t BKCol){
	int8_t Cnt, Len, NegNum = 0, XPos = X;

	if(Num == 0 && Pad == 0){
		Pad = 1;
	}

	if(Num<0){
		Num = -Num;
		NegNum = 1;
	}

	if(NegNum){
		XPos = PChar('-', XPos, Y, Size, Col, BKCol);
	}

	Len = CheckNumLength(Num);

	if(Len<Pad){
		for(Cnt = 0; Cnt<(Pad-Len); Cnt++){
			XPos = PChar('0', XPos, Y, Size, Col, BKCol);
		}
	}

	for(Cnt = Len-1; Cnt>=0; Cnt--){
		XPos = PChar('0' + (Num/FPow(10, Cnt))%10, XPos, Y, Size, Col, BKCol);
	}

	return XPos;
}

int16_t PNumF(float Num, uint8_t X, uint8_t Y, uint8_t Prec, uint8_t Size, uint32_t Col, uint32_t BKCol){
	char NBuf[17], PadBuf[5] = "%.0f";
	uint8_t SCnt, Len, XPos = X;
	int32_t ZPads;

	//PadBuf[2] = Prec + '0';
	//Len = sprintf(NBuf, &PadBuf[0], Num);

	int32_t IPart = Num;
	int32_t DPart = (AbsF(Num)-(float)Abs(IPart))*FPow(10, Prec);

	XPos = PNum(IPart, X, Y, 0, Size, Col, BKCol);
	XPos = PChar('.', XPos, Y, Size, Col, BKCol);

	ZPads = (CheckNumLength(DPart)-Prec);
	if(ZPads>0){
		for(SCnt = 0; SCnt<ZPads; SCnt++){
			XPos = PChar('0', XPos, Y, Size, Col, BKCol);
		}
	}

	XPos = PNum(DPart, XPos, Y, 0, Size, Col, BKCol);

	/*Flt2Str(Num, NBuf, Prec);
	XPos = PStr(NBuf, X, Y, Size, Col, BKCol);*/

	/*for(SCnt = 0; SCnt<Len; SCnt++){
		XPos = PChar(NBuf[SCnt], XPos, Y, Size, Col, BKCol);
	}*/

	return XPos;
}

uint8_t Circle(uint8_t XS, uint8_t YS, uint8_t R, uint32_t Col){
	return Ellipse(XS, YS, R, R, Col);
}

uint8_t Ellipse(uint8_t XS, uint8_t YS, uint8_t XR, uint8_t YR, uint32_t Col){
	const uint16_t Res = 360;
	uint16_t Cnt;
	uint8_t X, Y;

	for(Cnt = 0; Cnt<Res; Cnt++){
		X = XS+((XR*qSin(Cnt+90))>>10);
		Y = YS+((YR*qSin(Cnt))>>10);
		WritePix(X, Y, Col);
	}
	return 0;
}

uint8_t Semicircle(uint8_t XS, uint8_t YS, uint8_t R, uint8_t UD, uint32_t Col){
	const uint16_t Res = 360;
	uint16_t Cnt;
	uint8_t X, Y;

	for(Cnt = 0; Cnt<Res/2; Cnt++){
		if(UD){
			X = XS - ((R*qSin(Cnt+90))>>10);
			Y = YS - ((R*qSin(Cnt))>>10);
		}
		else{
			X = XS + ((R*qSin(Cnt+90))>>10);
			Y = YS + ((R*qSin(Cnt))>>10);
		}
		WritePix(X, Y, Col);
	}

	LineC(XS-(R-1), YS, XS+(R), YS, Col, 0);
	return 0;
}

//TODO: Change from XS to XE and YS to YE and instead do origin, xlen and ylen
uint8_t Rect(uint8_t XS, uint8_t YS, uint8_t XE, uint8_t YE, uint16_t Angle, uint32_t Col){
	int16_t XLM = - (IAbs(XE-XS)>>1), XRM = -XLM;
	int16_t YTM = (IAbs(YE-YS)>>1), YBM = -YTM;
	int16_t OrX = XS+(IAbs(XE-XS)>>1), OrY = YS+(IAbs(YE-YS)>>1);
	int32_t NTLCoX, NTRCoX, NBLCoX, NBRCoX;
	int32_t NTLCoY, NTRCoY, NBLCoY, NBRCoY;

	//Rotated co-ordinates
	NTLCoX = ((XLM*qSin(Angle+90))>>10) - ((YTM*qSin(Angle))>>10);
	NTLCoY = ((XLM*qSin(Angle))>>10) + ((YTM*qSin(Angle+90))>>10);

	NTRCoX = ((XRM*qSin(Angle+90))>>10) - ((YTM*qSin(Angle))>>10);
	NTRCoY = ((XRM*qSin(Angle))>>10) + ((YTM*qSin(Angle+90))>>10);

	NBLCoX = ((XLM*qSin(Angle+90))>>10) - ((YBM*qSin(Angle))>>10);
	NBLCoY = ((XLM*qSin(Angle))>>10) + ((YBM*qSin(Angle+90))>>10);

	NBRCoX = ((XRM*qSin(Angle+90))>>10) - ((YBM*qSin(Angle))>>10);
	NBRCoY = ((XRM*qSin(Angle))>>10) + ((YBM*qSin(Angle+90))>>10);

	LineC(NTLCoX+OrX, NTLCoY+OrY, NTRCoX+OrX, NTRCoY+OrY, Col, 0); //Top left to top right
	LineC(NTLCoX+OrX, NTLCoY+OrY, NBLCoX+OrX, NBLCoY+OrY, Col, 0); //Top left to bottom left
	LineC(NTRCoX+OrX, NTRCoY+OrY, NBRCoX+OrX, NBRCoY+OrY, Col, 0); //Top right to bottom right
	LineC(NBLCoX+OrX, NBLCoY+OrY, NBRCoX+OrX, NBRCoY+OrY, Col, 0); //Bottom left to bottom right

	return 0;
}

uint8_t FillRec(uint8_t XS, uint8_t YS, uint8_t XE, uint8_t YE, uint32_t LCol, uint32_t FCol){
	uint8_t XCnt, YCnt, LCnt;

	uint8_t FRowLen = IAbs(XE-XS)-1, FColLen = IAbs(YE-YS)-1;
	uint32_t Cnt, Area = FRowLen*FColLen;

	LineC(XS, YS, XE, YS, LCol, 1);
	LineC(XS, YE, XE, YE, LCol, 1);
	LineC(XS, YS, XS, YE, LCol, 1);
	LineC(XE, YS, XE, YE, LCol, 1);

	SetAddr(XS+1, YS+1, XE-1, YE-1);
	for(Cnt = 0; Cnt<=Area; Cnt++){
		PCol(FCol);
	}

	return 0;
}

uint8_t LineP(uint8_t XS, uint8_t YS, uint8_t R, int16_t Angle, uint32_t Col){
	uint8_t XE, YE;

	XE = XS+((R*qSin(Angle+90))>>10);
	YE = YS+((R*qSin(Angle))>>10);

	return LineC(XS, YS, XE, YE, Col, 0);
}

uint8_t DrawSquare(uint16_t X, uint16_t Y, uint16_t Col){
	uint16_t Cnt;

	if(X>XPix-2 || Y>YPix-2 || X<2 || Y<2) return 1;

	SPI_Mode(LCDmd);
	SetAddr(X-2, Y-2, X+2, Y+2);
	WriteAO(1);
	WriteCS(0);
	for(Cnt = 0; Cnt<25; Cnt++){
		SW(Col);
	}
	WriteCS(1);

	return 0;
}


void DrawFill(uint8_t XS, uint8_t YS, uint8_t XE, uint8_t YE, uint16_t Col){
	uint32_t End;

	SPI_Mode(LCDmd);

	if(YE==YS){
		SetAddr(XS, YS, XE-1, YE);
		End = (XE-XS);
	}
	else if(XE==XS){
		End = (YE-YS);
		SetAddr(XS, YS, XE, YE-1);
	}
	else{
		SetAddr(XS, YS, XE-1, YE-1);
		End = (XE-XS)*(YE-YS);
	}
	ConfigSPIDS(SPI_DataSize_16b);
	WriteAO(1);
	WriteCS(0);

	uint32_t C;
	for(C = 0; C<End; C++){
		SW(Col);
	}

	WriteCS(1);
	ConfigSPIDS(SPI_DataSize_8b);
}

uint8_t LineC(int32_t XS, int32_t YS, int32_t XE, int32_t YE, uint32_t Col, uint8_t ColInc){
	int dx = IAbs(XE-XS), sx = XS<XE ? 1 : -1;
	int dy = IAbs(YE-YS), sy = YS<YE ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;


	uint8_t Ri, Gi, Bi, ColI = 4;
	uint16_t R, G, B;
	if(ColInc){
		Conv565RGB(Col, &Ri, &Gi, &Bi);
		R = Ri;
		G = Gi;
		B = Bi;

		if(R>G){
			if(R>B) ColI = 0;
		}
		else if(G>R){
			if(G>B) ColI = 1;
		}
		else if(B>R){
			if(B>G) ColI = 2;
		}
	}

	const uint8_t COLINC = 3;

	for(;;){
		if(ColInc){
			if(R>255) R = 255;
			if(G>255) G = 255;
			if(B>255) B = 255;
			WritePix(XS, YS, ConvRGB565T(R, G, B));
		}
		else{
			WritePix(XS, YS, Col);
		}

		switch(ColI){
		case 0: R+=COLINC; break;
		case 1: G+=COLINC; break;
		case 2: B+=COLINC; break;
		case 3:
			R+=COLINC;
			G+=COLINC;
			B+=COLINC;
			break;
		}

		if (XS==XE && YS==YE) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; XS += sx; }
		if (e2 < dy) { err += dx; YS += sy; }
	}
	return 0;
}

uint8_t Row(uint8_t Y, uint8_t Amount, uint8_t Rows, uint32_t Col){
	uint8_t YCnt, XCnt;

	if(InBounds(Y, Amount) && InBounds(Y+Rows, Amount)){
		SetAddr(0, Y, Amount-1, Y+Rows-1);
		for(XCnt = Y; XCnt<(Rows+Y); XCnt++){
			for(YCnt = 0; YCnt<Amount; YCnt++){
				PCol(Col);
			}
		}

		return 0;
	}
	else{
		return 1;
	}
}

uint8_t Column(uint8_t X, uint8_t Amount, uint8_t Columns, uint32_t Col){
	uint8_t YCnt, XCnt;

	if(InBounds(X, Amount) && InBounds(X+Columns, Amount)){
		SetAddr(X, 0, X+Columns-1, Amount-1);
		for(XCnt = X; XCnt<(Columns+X); XCnt++){
			for(YCnt = 0; YCnt<Amount; YCnt++){
				PCol(Col);
			}
		}

		return 0;
	}
	else{
		return 1;
	}
}

uint8_t Triangle(uint8_t X, uint8_t Y, uint32_t SideLen, uint16_t Angle, uint32_t Col){
	int32_t TCoX, TCoY, LCoX, LCoY, RCoX, RCoY;
	int32_t NTCoX, NTCoY, NLCoX, NLCoY, NRCoX, NRCoY;
	int16_t SLST = SideLen>>1, HST, Height;

	Angle+=180;

	Height = (SideLen*887)>>10;
	HST = Height>>1;

	TCoX = 0;
	TCoY = HST;

	LCoX = -SLST;
	LCoY = -HST;

	RCoX = SLST;
	RCoY = LCoY;

	RotateCo(&TCoX, &TCoY, Angle);
	RotateCo(&RCoX, &RCoY, Angle);
	RotateCo(&LCoX, &LCoY, Angle);

	if(InBounds(TCoX, TCoY) && InBounds(LCoX, LCoY) && InBounds(RCoX, RCoY)){
		LineC(TCoX+X, TCoY+Y, LCoX+X, LCoY+Y, Col, 0);
		LineC(TCoX+X, TCoY+Y, RCoX+X, RCoY+Y, Col, 0);
		LineC(RCoX+X, RCoY+Y, LCoX+X, LCoY+Y, Col, 0);

		return 0;
	}
	else{
		return 1;
	}
}

//Check if given co-ordinates are within range.
uint8_t InBounds(int32_t X, int32_t Y){
	if(X<0 || X>XPix) return 0;
	if(Y<0 || Y>YPix) return 0;

	return 1;
}

//Rotate co-ordinates around the origin
void RotateCo(int32_t *X, int32_t *Y, int32_t Angle){
	int32_t XT;

	XT = ((*X*qSin(Angle+90))>>10) - ((*Y*qSin(Angle))>>10);
	*Y = ((*X*qSin(Angle))>>10) + ((*Y*qSin(Angle+90))>>10);

	*X = XT;
}

//Returns -1024 to 1024!
int16_t qSin(int32_t Ph){
	int32_t XSqr;

	Ph%=360;

	if(Ph<180){
		XSqr = (8100-(Ph-90)*(Ph-90));
	}
	else{
		Ph-=180;
		XSqr = ((Ph-90)*(Ph-90)-8100);
	}
	XSqr = (XSqr*517)>>12;

	return XSqr;
}

uint32_t IAbs(int32_t N){
	if(N>0) return N;
	else return -N;
}
/*
uint32_t StrLen(char *S){
	uint32_t SCnt = 0;
	uint8_t CChar = 1;

	while(CChar!= '\0'){
		CChar = S[SCnt];
		SCnt++;
	}

	return SCnt-1;
}

uint8_t CheckNumLength(int32_t Num){
	uint8_t Len = 0, Cnt;

	for(Cnt = 0; Cnt<10; Cnt++){
		if(Num>=FPow(10, Cnt)){
			Len = Cnt;
		}
		else{
			Len = Cnt;
			break;
		}
	}

	return Len;
}

int32_t FPow(int32_t Num, uint32_t Pow){
	int32_t NumO = Num;
	uint32_t Cnt;
	if(Pow>1){
		for(Cnt = 0; Cnt<Pow-1; Cnt++){
			Num*=NumO;
		}
		return Num;
	}

	if(Pow==1) return Num;
	if(Pow==0) return 1;
	else return 0;
}*/
