/*
 * Utility.c
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "Utility.h"

#define MAX_NUM_INTEGER	6
#define MAX_NUM_FRAZ	6

char *hLookUp = "0123456789abcdef";

dword sh_exp16[] = 
{
	0,
	4,
	8,
	12,
	16,
	20,
	24,
	28
};

/*
 * Ritorna il valore numerico di un carattere hexadecimale
 */
byte cVal(byte c)
{
	if (c >= 'a' && c <= 'f')
		return (byte)(10 + c - 'a');
	
	else if (c >= 'A' && c <= 'F')
		return (byte)(10 + c - 'A');
	
	else if IsNumber(c)
		return (byte)(c - '0');
	
	else
		return 0;
}

void Move(byte *buffOr, byte *buffDest, word size)
{
	short i;
	
	dword v = (dword)*buffOr ;
	
	if (!size)
	{
		// Si tratta di una stringa
		for (; *buffOr;)
		{
			*buffDest++ = *buffOr++;
			
		}
	}
	else
	{
		for (i = 0; i < size; ++i)
		{
			
			*buffDest++ = *buffOr++;
			
		}
	}
}

void CopyDataFromCAN(byte *buffOr, byte *buffDest, word size)
{
	short i;
	
	dword v = (dword)*buffOr ;
	
	if (!size)
	{
		// Si tratta di una stringa
		for (; *buffOr;)
		{
			*buffDest++ = *buffOr++;
			
		}
	}
	else
	{
		for (i = 0; i < size; ++i)
		{
			
			*buffDest++ = *buffOr++;
			
		}
	}
}


void CopyDataToCAN(byte *buffOr, byte *buffDest, word size)
{
	short i;
	
	for (i = 0; i < 8; ++i)
		buffDest[i] = 0;
	
	if (!size)
	{
		// String
		while (*buffOr)
		{
			*buffDest++ = *buffOr++;
		}
	}
	else
	{
		for (i = 0; i < size; ++i)
		{
			*buffDest++ = *buffOr++;
		}
	}
}

void CopyBitsFromCan(byte *buffOr, byte *buffDest, byte byteInit, byte numBits)
{
	short i;
	short orWord;
	word orBit;
	
	word tmp;
	short destWord = 0;
	short destBit = 0;
	
	orWord = byteInit / 16;
	orBit = (word)(byteInit % 16);

	for (i = 0; i < numBits; ++i)
	{
		tmp = (word)(1 << orBit);
		 
		if ((buffOr[orWord] & tmp) == tmp)
			buffDest[destWord] |= 1 << destBit;
		
		else
			buffDest[destWord] &= ~(1 << destBit);
		
		if (++orBit >= 16)
		{
			orBit = 0;
			orWord++;
		}
		
		if (++destBit >= 16)
		{
			destBit = 0;
			destWord++;
			
		}
		
	}
}

void BuffCopyValue(dword value, byte *buffDest, byte size)
{
	byte i;
	
	for (i = 0; i < size; ++i)
	{
		*buffDest++ = (word)(value & 0xFF);
		value >>= 8;
	}
	for (i = size; i < 4; ++i)
	{
		buffDest[i] = 0;
	}
}

dword DWGetValueFromBuff(byte *buff)
{
	short i;
	short sh = 0;
	dword value;
	
	value = 0;
	
	for (i = 0; i < 4; ++i)
	{
		value |= buff[i] << sh;
		sh += 8;
	}
	
	return value;
}

void DWSetValueToBuffer(dword value, byte *buffDest, word size)
{
	word i;
	
	for (i = 0; i < size; ++i)
	{
		*buffDest++ = (byte)(value & 0xFF);
		value >>= 8;
				
	}

}

/*
 * Swaps a double word (32bits)
 */
dword DWSwap(dword value)
{
	dword sw;
	
	sw = (value &  0xFF000000) >> 24;
	sw |= (value & 0x00FF0000) >> 8;
	sw |= (value & 0x0000FF00) << 8;
	sw |= (value & 0x000000FF) << 24;
	
	return sw;
}

/*
 * Swaps a  word (16bits)
 */
word WSwap(word value)
{
	word sw;
	
	
	sw = (value & 0x0000FF00) >> 8;
	sw |= (value & 0x000000FF) << 8;
	
	return sw;
}

/*********************************************************
 * Module: char *ByteToHex(byte num)
 * 
 * Description:
 * 	Converte un byte in una stringa esadecimale
 *
 * Inputs:
 *	byte num: numero da convertire
 *
 * Outputs:
 *	char *hdigits: stringa hesadecimale del numero	
 * Return:
 *	none
 *
 ********************************************************/
void ByteToHex(byte num, byte *hdigits)
{
	
	char x;
	short pos = 0;
	
										//corresponding index.
	
	byte temp = num;					//temporary place holder for _num of the same type
										//as _num
	
	if (num < 0)						
	{									
		hdigits[0] = '-';					//if _num is negative make the sign negative
		num *= -1;
		pos = 1;						//and make _num positive to clear(zero) the sign bit
	}
	
	
	for (x = 0; x < 2; x++)
	{	
		temp = num;							//temp is assigned _num each time
		temp >>= (4 * x);					//shift it in multiples of 4 bits each time
		temp &= 0x00f;						//mask the integer so it will only have lowest 4 bits
		hdigits[pos + 1 - x] = (byte)hLookUp[temp];	//temp now contains a numeric value which will point
											//to the corresponding index in the look up table so
											//all we have to do is use it as an index to the lookup
	}
										
	hdigits[pos + 2] = 0;				//the last element will store a null										
}	

/*===================================
 *
 * Da una stringa recupera il valore
 * di un numero di tipo double
 *
 *===================================*/

short Alfasscanfd(char **p, double *value)
{
	short i;
	double exp;
	short numChr = 0;
	short sgn;
	double v[2];

	
	short k;
	
	while (!IsNumber(**p))
		(*p)++;
	
	if (**p == '-')	
	{
		sgn = -1;
		(*p)++;
	}
	else
		sgn = 1;
	
	*value = 0;
	
	k = 0;
	
	v[0] = 0;
	v[1] = 0;
	
	do
	{
		
		if (k == 1 && **p == '.')
			*p += 1;
		
		for (i = 0; IsNumber((*p)[i]); ++i)
		;
		
		numChr = i;
		
		
		
		if (k == 0)
		{
			exp = 1;
			while (--i >= 0)
			{
				v[k] += (double)((*p)[i] - '0') * exp;
				exp *= 10;
			}
		}
		else
		{
			exp = 10;
			for (i = 0; i < numChr; ++i)
			{
				v[k] += (double)((*p)[i] - '0') / exp;
				exp *= 10;
			}
		}
		(*p) += numChr;
		
		k++;
	}
	while (k < 2 && **p == '.');
	
	*value = v[0] + v[1];
	*value *= sgn;
	
	
	return numChr;
}


/*===================================
 *
 * Da una stringa recupera il valore
 * di un numero di tipo intero
 *
 *===================================*/
short Alfasscanfl(char **p, long *value)
{
	short i;
	long exp = 1;
	short numChr = 0;
	short sgn;
	
	
	
	while (!IsNumber(**p))
		(*p)++;
	
	if (**p == '-')	
	{
		sgn = -1;
		(*p)++;
	}
	else
		sgn = 1;
	
	for (i = 0; IsNumber((*p)[i]); ++i)
	;
	
	numChr = i;
	
	
	*value = 0;
	
	while (--i >= 0)
	{
		*value += ((*p)[i] - '0') * exp;
		exp *= 10;
	}

	*value *= sgn;
	
	(*p) += numChr;
	
	return numChr;
		
}

short Alfasscanfb(char **p, char *value)
{
	long lval;
	short numChr;
	
	numChr = Alfasscanfl(p, &lval);
	
	*value = (char)lval;
	
	return numChr;
}
/*
 * Converte un numero di 16 bits in una stringa
 *
 */
short Alfasscanfs(char **p, short *value)
{
	long lval;
	short numChr;
	
	numChr = Alfasscanfl(p, &lval);
	
	*value = (short)lval;
	
	return numChr;
	
}

/*
 * Converte un numero double in stringa
 */ 
short Alfasprintfd(char *p, double value)
{
	char buff[MAX_NUM_INTEGER];
	char decimal[MAX_NUM_FRAZ];
	short nCharInt;
	short nCharFraz;
	
	float num;
	long inum;
	
	short nchar = 0;
	short i;
	
	// Parte intera
	inum = (long)value;
	
	nCharInt = 0;
	
	if (inum == 0)
		buff[nCharInt++] = '0';
	
	else
	{
		while (inum != 0 && nCharInt < MAX_NUM_INTEGER)
		{
			buff[nCharInt++] = (char)((char)(inum % 10) + '0');
			inum /= 10;
		}
	}
	
	// Parte Frazionaria
	nCharFraz = 0;
	
	if ((long)value != value)
	{
				
		num = ((float)(value - (long)value));
		
		while (nCharFraz < MAX_NUM_FRAZ && num != 0)
		{
			num *= 10;
			
			decimal[nCharFraz++] = (char)((char)num + '0') ;
			
			num = num - (long)num;
			
		}
	}
	
	while (nCharInt > 0)
	{
		*p++ = buff[--nCharInt];
		nchar++;
	}
	
	if (nCharFraz > 0)
	{
		*p++ = '.';
		nchar++;
	}
	
	for (i = 0; i < nCharFraz; ++i)
	{
		*p++ = decimal[i];
		nchar++;
	}
	
	*p = 0;
	
	return nchar;
}


/*
 * Copia una string a su un'altra
 * Inputs:
 *	char *p: stringa di origine
 *	char *out: puntatore alla stringa di uscita
 *	short len: massima dimensione
 */
short Alfasprints(char *p, char *out, short len)
{
	short i;
	
	for (i = 0; *p && (i < len || len == 0); ++i)
	{
		*out++ = *p++;
	}
	*out = 0;
	
	return i;
			
}

long _ABS(long value) 
{
	
	if (value >= 0) 
		return value;
	else
		return -value;
}
/*
 * Converte un numero di 32 bits in una stringa
 *
 */
	
short Alfasprintfl(char *p, long value)
{

	char buff[10];
	short i = 0;
	short nchar = 0;
	
	if (value == 0)
		buff[i++] = '0';
	
	
	if (value < 0)
	{
		value = -value;
		
		*p++ = '-';
		*p = 0;
		
		nchar++;
	}
	
	while (value != 0)
	{
		buff[i++] = (char)((value % 10) + '0');
		
		value /= 10;	
	}
		
	
	while (--i >= 0)
	{
		*p++ = buff[i];
		nchar++;
	}
	
	*p = 0;
	
	return nchar;
	
	
}

void TmrDelay(long value)
{
	while (value-- > 0)
	;
}

