/*
 * comport.h
 *
 *  Created on: Feb 20, 2025
 *      Author: JungBongJun
 */

#ifndef BSW_COMPORT_H_
#define BSW_COMPORT_H_

#include "stdbool.h"

struct ComPacket
{
	uint8_t Start;
	uint8_t ID;
	uint8_t Ctrl;
	uint8_t data[4];
	uint8_t ExtDataLen_H;
	uint8_t ExtDataLen_L;
	uint8_t ErrCode;
	uint8_t CheckSum;
	uint8_t End;
};

typedef struct
{
	bool bOpen;
	bool bExtSendMask;
	struct ComPacket Tx;
	struct ComPacket* pRx;
}COM_Handle;

#endif /* BSW_COMPORT_H_ */
