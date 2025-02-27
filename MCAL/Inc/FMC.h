#ifndef INC_FMC_H_
#define INC_FMC_H_

#include <MCAL/Inc/MCAL_Common.h>
#include <fmc.h>


/* Exported constants ------------------------------------------------------------------*/
#define SDRAM_BANK_ADDR                 ((uint32_t)0xD0000000)

/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_8 */
#define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_16

/* #define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2 */
#define SDCLOCK_PERIOD                FMC_SDRAM_CLOCK_PERIOD_3

#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF)


//#define FMC_SDRAM_BLOCK						     (

#define FMC_SDRAM_BLOCK_0     	((uint32_t)0x00080000) /* SDRAM Block 0 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_1     	((uint32_t)0x00100000) /* SDRAM Block 1 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_2     	((uint32_t)0x00180000) /* SDRAM Block 2 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_3     	((uint32_t)0x00200000) /* SDRAM Block 3 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_4     	((uint32_t)0x00280000) /* SDRAM Block 4 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_5     	((uint32_t)0x00380000) /* SDRAM Block 5 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_6     	((uint32_t)0x00400000) /* SDRAM Block 6 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_7     	((uint32_t)0x00480000) /* SDRAM Block 7 - 512 Kbyte */

enum
{
    FMC_OK = 0,
    FMC_FAIL = 1
};

enum
{
	FMC_BLOCK_0 = 0,
	FMC_BLOCK_1 = 1,
	FMC_BLOCK_2,
	FMC_BLOCK_3,
	FMC_BLOCK_4,
	FMC_BLOCK_5,
	FMC_BLOCK_6,
	FMC_BLOCK_7

};

void FMC_Init(void);
uint32_t FMC_Write(uint32_t Offset, uint32_t Size, uint8_t* pBuf);
uint32_t FMC_Read(uint32_t Offset, uint32_t Size, uint8_t* pBuf);
uint8_t* FMC_GetBufAddr();

#endif /* INC_FMC_H_ */
