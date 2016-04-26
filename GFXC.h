#ifndef GFXC_H
#define GFXC_H

#include "ILI9163.h"
#include "Fonts.h"
#include "HFunc.h"

#define FAST_CHAR
#define LetterSpace 1
#define LINES 4

#define ConvRGB565(R,G,B) ((R) | ((G)<<5) | ((B)<<11))
#define Conv332565(Px) ((((Px)&3)<<3) | ((((Px)>>2)&7)<<8) | (((Px)>>5)<<12))
#define Conv121565(Px) ((((Px)&1)<<4) | ((((Px)>>1)&3)<<9) | (((Px)>>3)<<15))
//#define Conv565332(Px) (((Px)>>13) | (((Px)>>8)&7) | (((Px)>>3)&3))
#define Conv565332(Px) (((((Px)>>3)&3)) | ((((Px)>>8)&7)<<2) | (((Px)>>13)<<5))

extern const uint16_t RGB332to565lookupTable[256];
typedef enum{
	B16 = 3,
	B8 = 2,
	B4 = 1,
	B1 = 4
} HPXBitDepth;

typedef struct HPX{
	uint16_t IX, IY;
	uint8_t BitDepth;
} HPXHeader;

uint16_t ColIP(uint16_t, uint8_t);
uint16_t ColP(uint8_t, uint8_t, uint8_t);

uint16_t Conv16C16BPix(uint16_t Px);
void Conv16C16BBuf(uint8_t *BufI, uint16_t *BufO);
void Conv565RGB(uint16_t C, uint8_t *R, uint8_t *G, uint8_t *B);
void Conv332RGB(uint8_t C, uint8_t *R, uint8_t *G, uint8_t *B);
uint16_t ConvRGB565T(uint8_t R, uint8_t G, uint8_t B);
uint8_t ConvRGB565_332(uint16_t Px);
uint16_t ConvRGB332_565(uint8_t Px);

int16_t PChar(uint16_t, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);
int16_t PStr(char*, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);
int16_t PStrL(char*, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);
int16_t PNum(int32_t, uint8_t, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);
int16_t PNumF(float, uint8_t, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);

uint8_t Circle(uint8_t, uint8_t, uint8_t, uint32_t);
uint8_t Ellipse(uint8_t, uint8_t, uint8_t, uint8_t, uint32_t);
uint8_t Semicircle(uint8_t, uint8_t, uint8_t, uint8_t, uint32_t);

uint8_t Rect(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint32_t);
uint8_t FillRec(uint8_t, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);
uint8_t LineP(uint8_t, uint8_t, uint8_t, int16_t, uint32_t);
uint8_t LineC(int32_t, int32_t, int32_t, int32_t, uint32_t, uint8_t);
uint8_t DrawSquare(uint16_t, uint16_t, uint16_t);

void DrawFill(uint8_t XS, uint8_t YS, uint8_t XE, uint8_t YE, uint16_t Col);

uint8_t Triangle(uint8_t, uint8_t, uint32_t, uint16_t, uint32_t);

uint8_t Row(uint8_t, uint8_t, uint8_t, uint32_t);
uint8_t Column(uint8_t, uint8_t, uint8_t, uint32_t);

uint8_t InBounds(int32_t, int32_t);
void RotateCo(int32_t *, int32_t *, int32_t);
uint32_t IAbs(int32_t);
int16_t qSin(int32_t);
/*uint32_t StrLen(char *);
uint8_t CheckNumLength(int32_t);
int32_t FPow(int32_t, uint32_t);*/

#endif
