/*
 * common.h
 *
 *  Created on: Feb 11, 2025
 *      Author: JungBongJun
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

/* exact-width signed integer types */
typedef   signed          char s8;
typedef   signed short     int s16;
typedef   signed           int s32;

/* exact-width unsigned integer types */
typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

/* variable type chaned --------------- */
#define MAKEBYTE(a, b)		(((unsigned char)(a)&0x0F)|(((unsigned char)(b)&0x0F)<<4))
#define MAKEWORD(a, b)		((unsigned short)(((unsigned char)(a)) | ((unsigned short)((unsigned char)(b))) << 8))
#define MAKELONG(a, b)		((long)(((unsigned short)(a)) | ((unsigned long)((unsigned short)(b))) << 16))
#define MAKEUINT(a, b, c, d)((unsigned int)((a << 24)|(b << 16)|(c << 8)|(d)))
#define BUFFER_TO_BIN(b,t)	(*((t*)b))
#define BIN_TO_BUFFER(b,t)	((t*)&b)

/* Extract a short from a long
 */
#define HIWORD(x)		((uint16)((x) >> 16))
#define LOWORD(x)		((uint16)(x))
#define HIBYTE(x)		((unsigned char)((x) >> 8))
#define LOBYTE(x)		((unsigned char)(x))
#define HINIBBLE(x) 	(((x) >> 4) & 0xf)
#define LONIBBLE(x) 	((x) & 0xf)
#define MSB(word)      (u8)(((u16)(word) >> 8) & 0xff)
#define LSB(word)      (u8)((u16)(word) & 0xff)


#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#endif /* INC_COMMON_H_ */
