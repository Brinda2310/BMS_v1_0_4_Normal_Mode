/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*                                                                       */
/*   Portions COPYRIGHT 2015 STMicroelectronics                          */
/*   Portions Copyright (C) 2014, ChaN, all right reserved               */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/**
  ******************************************************************************
  * @file    diskio.c 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    08-May-2015
  * @brief   FatFs low level disk I/O module.
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <SD_SPI.h>
#include "diskio.h"

#define BLOCK_SIZE                512

static volatile DSTATUS Stat = STA_NOINIT;
/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	Stat = STA_NOINIT;

	SD_Status();

	/* Configure the uSD device */
	if (SdStatus == SD_PRESENT)
	{
		if (SD_Init() == MSD_OK)
		{
			Stat &= ~STA_NOINIT;
		}
	}
	else
		Stat = STA_NOINIT;

	return Stat;
}

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
	Stat = STA_NOINIT;

	SD_Status();

	if (SdStatus == SD_PRESENT)
	{
		if (SD_GetStatus() == MSD_OK)
		{
			Stat &= ~STA_NOINIT;
		}
	}

	return Stat;
}

/**
  * @brief  Reads Sector(s) 
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{

	DRESULT res = RES_OK;

	SD_Status();

	if (SdStatus == SD_PRESENT)
	{
		if (SD_ReadBlocks((uint32_t*) buff, (uint64_t) (sector * BLOCK_SIZE),BLOCK_SIZE, count) != MSD_OK)
		{
			res = RES_ERROR;
		}
	}
	else
		res = RES_ERROR;

	return res;
}

/**
  * @brief  Writes Sector(s)  
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT disk_write (
	BYTE pdrv,		/* Physical drive number to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
	DRESULT res = RES_OK;

	SD_Status();

	if (SdStatus == SD_PRESENT)
	{
		if (SD_WriteBlocks((uint32_t*) buff, (uint64_t) (sector * BLOCK_SIZE),BLOCK_SIZE, count) != MSD_OK)
		{
			res = RES_ERROR;
		}
	}
	else
		res = RES_ERROR;

	return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation  
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	 DRESULT res = RES_ERROR;
	  SD_CardInfo CardInfo;

	  if (Stat & STA_NOINIT) return RES_NOTRDY;

	  switch (ctrl)
	  {
	  /* Make sure that no pending write process */
	  case CTRL_SYNC :
	    res = RES_OK;
	    break;

	  /* Get number of sectors on the disk (DWORD) */
	  case GET_SECTOR_COUNT :

		SD_Status();

		if (SdStatus == SD_PRESENT)
		{
			SD_GetCardInfo(&CardInfo);
			*(DWORD*) buff = CardInfo.CardCapacity / BLOCK_SIZE;
			res = RES_OK;
		}
		else
			res = RES_ERROR;
	    break;

	  /* Get R/W sector size (WORD) */
	  case GET_SECTOR_SIZE :
	    *(WORD*)buff = BLOCK_SIZE;
	    res = RES_OK;
	    break;

	  /* Get erase block size in unit of sector (DWORD) */
	  case GET_BLOCK_SIZE :
	    *(DWORD*)buff = BLOCK_SIZE;
	    break;

	  default:
	    res = RES_PARERR;
	  }

	  return res;

	return RES_OK;
}
#endif /* _USE_IOCTL == 1 */

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime (void)
{
  return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

