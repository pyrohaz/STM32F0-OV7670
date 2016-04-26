#include "HFunc.h"
#include "HFunc.h"

void StrCpy(char *Dst, const char *Src, uint32_t Amnt){
	uint32_t Cnt;

	for(Cnt = 0; Cnt<Amnt && Src[Cnt]; Cnt++){
		Dst[Cnt] = Src[Cnt];
	}
	Dst[Amnt-1] = 0;
}

uint32_t StrCpyC(char *Dst, const char *Src, char C){
	uint32_t Cnt;

	for(Cnt = 0; Src[Cnt] != C; Cnt++){
		Dst[Cnt] = Src[Cnt];
	}

	return Cnt;
}

int32_t Abs(int32_t Num){
	if(Num<0) return -Num;
	else return Num;
}

uint32_t StrLen(const char* Str){
	uint32_t Cnt = 0;

	while(*Str != '\0'){
		Str++;
		Cnt++;
	}
	return Cnt;
}


int32_t FPow(int32_t Num, uint32_t Pow){
	uint32_t Cnt;
	uint32_t N = 1;

	if(Pow == 0){
		return 1;
	}
	if(Pow == 1){
		return Num;
	}
	else{
		for(Cnt = 0; Cnt<Pow; Cnt++){
			N*=Num;
		}

		return N;
	}
}

uint8_t CheckNumLength(int32_t Num){
	uint8_t Len = 0, Cnt, Sign = 0;;

	if(Num<0){
		Sign = 1;
		Num = -Num;
	}
	else if(Num == 0) return 1;

	for(Cnt = 0; Cnt<9; Cnt++){
		if(Num>=FPow(10, Cnt)){
			Len = Cnt;
		}
		else{
			Len = Cnt;
			break;
		}
	}

	if(!Sign) return Len;
	else return Len+1;
}

int32_t Num2Str(int32_t Num, char *S, uint8_t Pad){
	int32_t Cnt, Len, NegNum = 0;

	if(Num!=0){
		if(Num<0){
			Num = -Num;
			NegNum = 1;
		}

		if(NegNum){
			S[0] = '-';
			S++;
		}

		Len = CheckNumLength(Num);

		if(Len<Pad){
			for(Cnt = 0; Cnt<(Pad-Len); Cnt++){
				S[Cnt] = '0';
			}
		}

		for(Cnt = Len-1; Cnt>=0; Cnt--){
			S[Len-(Cnt+1)] = '0' + (Num/FPow(10, Cnt))%10;
		}
		return Len;
	}
	else{
		S[0] = '0';
		S[1] = '\0';
		return 2;
	}
}


float FAbs(float N) {
	if(N<0.0f) return -N;
	return N;
}

//Reduce N to P precision
float RPrec(float N, uint8_t P) {
	int32_t Num, INum = N;
	float TNum;

	if(P == 0) {
		return (float) INum;
	}

	Num = N*(float)FPow(10, P);

	TNum = (float)Num/(float)FPow(10, P);
	return TNum;
}

void Flt2Str(float N, char *Str, uint8_t Prec){
	int32_t INum = N, INumT;
	uint32_t NLen = CheckNumLength(INum);

	Num2Str(INum, Str, 0);

	if(Prec == 0) {
		return;
	}

	Str[NLen] = '.';

	INumT = N*(float)FPow(10, Prec);
	INumT -= INum*FPow(10, Prec);

	Num2Str(Abs(INumT), &Str[NLen+1], 0);
}

int32_t Str2NumC(const char *S, char C){
	char NumBuf[20] = {0};
	uint8_t Cnt = 0;

	while(S[Cnt] != C){
		NumBuf[Cnt] = S[Cnt];
		Cnt++;
	}

	return Str2Num(NumBuf);
}

int32_t Str2Num(const char *S){
	uint32_t SLen = StrLen(S), Cnt;
	int32_t Num = 0;
	uint8_t Sign;

	if(S[0] == '-'){
		Sign = 0;
		S++;
		SLen--;
	}
	else Sign = 1;

	for(Cnt = 0; Cnt<SLen; Cnt++){
		Num += (S[Cnt]-'0')*FPow(10, (SLen-1)-Cnt);
	}

	if(!Sign){
		return -Num;
	}

	return Num;
}


int32_t Str2NumH(const char *S){
	uint32_t SLen = StrLen(S), Cnt;
	int32_t Num = 0;
	uint8_t Sign;
	char CChar = 0;

	//Check for leading "0x" and if the exist
	//advance pointer position by two and subtract
	//two from string length
	if(SLen>1 && S[0] == '0' && (S[1] == 'x' || S[1] == 'X')){
		S++;
		S++;
		SLen-=2;
	}

	//If h or H is found at the end denoting hex,
	//shorten string length by one.
	if(S[SLen-1] == 'h' || S[SLen-1] == 'H'){
		SLen--;
	}

	if(S[0] == '-'){
		Sign = 0;
		S++;
		SLen--;
	}
	else Sign = 1;

	for(Cnt = 0; Cnt<SLen; Cnt++){
		//Check for number
		if(S[Cnt] >= '0' && S[Cnt] <= '9'){
			Num |= (S[Cnt]-'0')<<(4*((SLen-1)-Cnt));
		}
		else if((S[Cnt] >= 'a' && S[Cnt] <= 'f')){
			CChar = S[Cnt]-('a'-10);
			Num |= (CChar)<<(4*((SLen-1)-Cnt));
		}
		else if((S[Cnt] >= 'A' && S[Cnt] <= 'F')){
			CChar = S[Cnt]-('A'-10);
			Num |= (CChar)<<(4*((SLen-1)-Cnt));
		}
	}

	if(!Sign){
		return -Num;
	}

	return Num;
}

float Str2FltC(const char *S, const char E){
	int32_t Cnt = 0;
	float PreDec, PostDec, RetNum;

	while(S[Cnt] != '.' && S[Cnt] != E && S[Cnt]) Cnt++;

	//No decimal point!
	if(!S[Cnt]) return Str2NumC(S, E);
	//Decimal point
	else{
		PreDec = (float) Str2NumC(S, '.');
		PostDec = (float) Str2NumC(&S[Cnt+1], E);
	}

	PostDec /= (float)FPow(10, CheckNumLength(PostDec));

	if(PreDec<0.0f){
		RetNum = PreDec-PostDec;
	}
	else{
		RetNum = PreDec+PostDec;
	}

	return RetNum;
}

//Compare two strings up to Amnt or null string
uint8_t StrCmp(char *StrA, char *StrB, uint32_t Amnt){
	uint8_t Cnt;

	for(Cnt = 0; Cnt<Amnt && StrA[Cnt] != 0 && StrB[Cnt] != 0; Cnt++){
		if(StrA[Cnt] != StrB[Cnt]){
			break;
		}
	}

	return(Cnt == Amnt);
}

//Returns the index of Cmp in Src if it exists, otherwise
//returns -1
int32_t StrFind(const char *Src, const char *Cmp){
	int32_t Cnt = 0, CCnt;
	uint8_t Found = 0;

	uint32_t CmpLen = StrLen(Cmp), SrcLen = StrLen(Src);

	while(Src[Cnt]){
		//First letter match
		if(Src[Cnt] == Cmp[0]){
			for(CCnt = 1; CCnt<CmpLen; CCnt++){
				if(Src[Cnt+CCnt] != Cmp[CCnt] || (Cnt>(SrcLen-CmpLen))){
					break;
				}
			}

			if(CCnt==CmpLen){
				return Cnt;
			}
		}

		Cnt++;
	}

	return -1;
}

int16_t ST[] = {0, 1608, 3212, 4808, 6393, 7962, 9512, 11039, 12539, 14010, 15446, 16846, 18204, 19519, 20787, 22005, 23170, 24279, 25329, 26319, 27245, 28105, 28898, 29621, 30273, 30852, 31356, 31785, 32137, 32412, 32609, 32728, 32767, 32728, 32609, 32412, 32137, 31785, 31356, 30852, 30273, 29621, 28898, 28105, 27245, 26319, 25329, 24279, 23170, 22005, 20787, 19519, 18204, 16846, 15446, 14010, 12539, 11039, 9512, 7962, 6393, 4808, 3212, 1608, 0, -1608, -3212, -4808, -6393, -7962, -9512, -11039, -12539, -14010, -15446, -16846, -18204, -19519, -20787, -22005, -23170, -24279, -25329, -26319, -27245, -28105, -28898, -29621, -30273, -30852, -31356, -31785, -32137, -32412, -32609, -32728, -32767, -32728, -32609, -32412, -32137, -31785, -31356, -30852, -30273, -29621, -28898, -28105, -27245, -26319, -25329, -24279, -23170, -22005, -20787, -19519, -18204, -16846, -15446, -14010, -12539, -11039, -9512, -7962, -6393, -4808, -3212, -1608};

float wSin(float P){
	uint32_t Index;
	while(P>TPI) P-=TPI;
	while(P<0) P+=TPI;

	float Phase = (P*127/TPI);
	Index = Phase;
	float Interp = Phase - (float)Index;

	return (ST[Index]*Interp + ST[Index]*(1.0f-Interp))*3.05185E-05f;
}

float wCos(float P){
    return wSin(P+MPI*0.5f);
}

float fSqrt(float V){
    int VI = *(int*)&V;
    VI -= (1<<23);
    VI >>= 1;
    VI += (1<<29);

    return *(float*)&VI;
}

float CalcDist(float La1, float Lo1, float La2, float Lo2) {
    const float R = 6371.0f;

    float RLa1 = TO_RAD(La1);
    float RLa2 = TO_RAD(La2);
    float RLo1 = TO_RAD(Lo1);
    float RLo2 = TO_RAD(Lo2);

    float DLat = TO_RAD(La2-La1);
    float DLon = TO_RAD(Lo2-Lo1);

    float X = DLon*wCos((RLa1+RLa2)*0.5f);
    float Y = DLat;
    float D = fSqrt(X*X + Y*Y)*R;

    return D;
}

