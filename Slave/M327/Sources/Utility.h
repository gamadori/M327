/*
 * Utility.h
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#define IsNumber(c)	(c >= '0' && c <= '9' || c == '-')

#define IsHexNumber(c)	(IsNumber(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))

#define SIZEOFW_BYTE	1

#define SIZEOFW_WORD	2

#define SIZEOFW_DWORD	4


extern dword sh_exp16[];

byte cVal(byte c);

void CopyDataFromCAN(byte *buffOr, byte *buffDest, word size);

void CopyDataToCAN(byte *buffOr, byte *buffDest, word size);

void BuffCopyValue(dword value, byte *buffDest, byte size);

dword DWGetValueFromBuff(byte *buff);

void DWSetValueToBuffer(dword value, byte *buffDest, word size);

void Move(byte *buffOr, byte *buffDest, word size);

dword DWSwap(dword value);

word WSwap(word value);

void ByteToHex(byte num, byte *hdigits);

short Alfasscanfl(char **p, long *value);

short Alfasscanfs(char **p, short *value);

short Alfasscanfb(char **p, char *value);

short Alfasscanfd(char **p, double *value);

short Alfasprintfl(char *p, long value);

short Alfasprints(char *p, char *out, short len);

long _ABS(long value);

short Alfasprintfd(char *p, double value);

void CopyBitsFromCan(byte *buffOr, byte *buffDest, byte byteInit, byte numBits);

void TmrDelay(long value);

#endif /* UTILITY_H_ */
