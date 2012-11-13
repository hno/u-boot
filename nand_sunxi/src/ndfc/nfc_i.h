/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef	_NFC_I1_H_
#define	_NFC_I1_H_

#include "../include/nand_reg.h"
#include "../include/nand_osal.h"
#include "../include/nand_ndfc.h"


#define    NFC_DBG(...)                         PRINT(__VA_ARGS__)

#if 1
static unsigned int _reg_dump_tmp;
#  define NFC_READ_REG(reg)   		((_reg_dump_tmp = (reg)), NFC_DBG("NFC READ "#reg "=%x\n", _reg_dump_tmp), _reg_dump_tmp)
#  define NFC_WRITE_REG(reg, data) 	(reg) = ((_reg_dump_tmp = (data)), NFC_DBG("NFC WRITE "#reg "=%x\n", _reg_dump_tmp), _reg_dump_tmp)
#else
#  define NFC_READ_REG(reg)   		(reg)
#  define NFC_WRITE_REG(reg,data) 	(reg) = (data)
#endif


#define ERR_ECC 	12
#define ECC_LIMIT 	10
#define ERR_TIMEOUT 14
#define READ_RETRY_MAX_TYPE_NUM 5
#define READ_RETRY_MAX_REG_NUM	4
#define READ_RETRY_MAX_CYCLE	10
#define LSB_MODE_MAX_REG_NUM	8

/* define various unit data input or output*/
#if 1
static inline __u8 NFC_READ_RAM_B(void *ram)    		{ unsigned r = (*((volatile __u8 *)(ram))); NFC_DBG("NFC RAM %p => %02x\n", ram, r); return r;}
static inline __u8 NFC_WRITE_RAM_B(void *ram, __u8 data)  	{ debug("NFC RAM %p <= %02x\n", data); (*((volatile __u8 *)(ram)) = (data)); return data;}
static inline __u16 NFC_READ_RAM_HW(void *ram)   		{ unsigned r = (*((volatile __u16 *)(ram))); NFC_DBG("NFC RAM %p => %04x\n", ram, r); return r;}
static inline __u16 NFC_WRITE_RAM_HW(void *ram, __u16 data) 	{ debug("NFC RAM %p <= %02x\n", data); (*((volatile __u16 *)(ram)) = (data)); return data;}
static inline __u32 NFC_READ_RAM_W(void *ram)   		{ unsigned r = (*((volatile __u32 *)(ram))); NFC_DBG("NFC RAM %p => %08x\n", ram, r); return r;}
static inline __u32 NFC_WRITE_RAM_W(void *ram,__u32 data) 	{ debug("NFC RAM %p <= %02x\n", data); (*((volatile __u32 *)(ram)) = (data)); return data;}
#else
#define NFC_READ_RAM_B(ram)    		(*((volatile __u8 *)(ram)))
#define NFC_WRITE_RAM_B(ram,data)  	(*((volatile __u8 *)(ram)) = (data))
#define NFC_READ_RAM_HW(ram)   		(*((volatile __u16 *)(ram)))
#define NFC_WRITE_RAM_HW(ram,data) 	(*((volatile __u16 *)(ram)) = (data))
#define NFC_READ_RAM_W(ram)   		(*((volatile __u32 *)(ram)))
#define NFC_WRITE_RAM_W(ram,data) 	(*((volatile __u32 *)(ram)) = (data))
#endif



#ifdef USE_PHYSICAL_ADDRESS
#define NFC_IS_SDRAM(addr)			((addr >= DRAM_BASE)?1:0)
#else
#define NFC_IS_SDRAM(addr)			( ((addr >= DRAM_BASE))&&(addr < SRAM_BASE)?1:0)
#endif

extern __hdle NAND_RequestDMA  (__u32 dmatype);
extern __s32  NAND_ReleaseDMA  (__hdle hDma);
extern __s32 NAND_SettingDMA(__hdle hDMA, void * pArg);
extern __s32 NAND_StartDMA(__hdle hDMA, __u32 saddr, __u32 daddr, __u32 bytes);
extern __s32 NAND_RestartDMA(__hdle hDma);
extern __s32 NAND_StopDMA(__hdle hDma);
extern __u32 NAND_QueryDmaStat(__hdle hDma);
extern __u32 NAND_QueryDmaChannelNo(__hdle hDma);
extern void NAND_GetPin(void);
extern void NAND_ReleasePin(void);
extern __u32 NAND_GetCmuClk(void);

extern __s32  NAND_SetClock(__u32 nand_max_clock);


#endif	/* _NFC_I_H_ */

