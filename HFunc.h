#ifndef HFUNC_H
#define HFUNC_H

#include <stm32f0xx.h>

typedef struct{
	uint8_t Hours, Minutes, Seconds;
	uint8_t Day, Month, Year;
} ChronoStruct;

#define AbsF(x) (((x)<0.0f)?-(x):(x))

#define MPI		3.14159265358979323846
#define TPI		(MPI*2)

#define TO_RAD(x) (((x)*MPI/180.0f))
#define TO_DEG(x) (((x)*180.0f/MPI))

void StrCpy(char *Dst, const char *Src, uint32_t Amnt);
uint32_t StrCpyC(char *Dst, const char *Src, char C);
int32_t Abs(int32_t);
//uint32_t StrLen(const char *);

uint8_t StrCmp(char *, char*, uint32_t);
int32_t StrFind(const char*, const char*);
int32_t FPow(int32_t, uint32_t);
int32_t Str2Num(const char *);
int32_t Str2NumH(const char *);
int32_t Str2NumC(const char *, char);
float Str2FltC(const char *, const char);
int32_t Num2Str(int32_t Num, char *, uint8_t);
void Flt2Str(float N, char *Str, uint8_t Prec);
float RPrec(float N, uint8_t P);
float FAbs(float N);
uint8_t CheckNumLength(int32_t Num);
float wSin(float P);
float wCos(float P);
float fSqrt(float V);

float CalcDist(float La1, float Lo1, float La2, float Lo2);


#endif
