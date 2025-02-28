#include <MCAL/Inc/FMC.h>

void FMC_Init(void)
{
	MX_FMC_Init();
}

uint32_t FMC_Read(uint32_t Offset, uint32_t Size, uint8_t* pBuf)
{
	uint32_t Data, i;
	uint32_t Address;

	Address = (SDRAM_BANK_ADDR + Offset);

	for(i = 0; i < Size; i++)
	{
		Data = *(__IO uint32_t*)(Address+ (i * 4));                    // Read 32bit Data
        //PRINTF(("FMC Read Addr %.8X Data %.8X \r\n", (Address+i), Data));

		pBuf[i] = (uint8_t)Data;
	}

	return FMC_OK;
}

uint32_t FMC_Write(uint32_t Offset, uint32_t Size, uint8_t* pBuf)
{
	uint32_t Data, i;
	uint32_t Address;

	Address = (SDRAM_BANK_ADDR + Offset);

	for(i = 0; i < Size; i++)
	{
		Data = (uint32_t) (pBuf[i]);
		*(__IO uint32_t*)(Address + (i * 4)) = Data;
		//PRINTF(("FMC Write Addr %.8X Data %.8X \r\n", (Address+i), Data));
	}

	return FMC_OK;
}

uint8_t* FMC_GetBufAddr()
{
	return (uint8_t*)(SDRAM_BANK_ADDR + FMC_SDRAM_BLOCK_0);
}
