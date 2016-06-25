#ifndef __INCLUDE_ONFI_H
#define __INCLUDE_ONFI_H


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "NFC.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

/*
 * ONFI commands
 */

/**
 * ONFI_Reset function is used to put a target into a known condition and to
 * abort command sequences in progress.
 */
void ONFI_Reset(void);

/**
 * ONFI_SynchronousReset function resets the target and all LUNs. 
 */
void ONFI_SynchronousReset(void);

/**
 * ONFI_ReadID function identifies that the target supports the ONFI
 * specification. For the Read ID command, only addresses of 00h and 20h are
 * valid.
 *
 * @param addr 1 byte address
 */
void ONFI_ReadID(uint8_t addr);

/**
 * ONFI_ReadParameterPage function retrieves the data structure that describes
 * the target’s organization, features, timings and other behavioral
 * parameters.
 *
 * @param addr 1 byte address
 */
void ONFI_ReadParameterPage(uint8_t addr);

/**
 * ONFI_ReadUniqueID function is used to retrieve the 16 byte unique ID (UID)
 * for the device. The unique ID when combined with the device manufacturer
 * shall be unique.
 *
 * @param addr 1 byte address
 */
void ONFI_ReadUniqueID(uint8_t addr);

/**
 * ONFI_SetFeatures function writes the sub-feature parameters (P1-P4) to the
 * specified feature address to enable or disable target-specific features.
 * This command is accepted by the target only when all LUNs on the target are
 * idle.
 *
 * @param addr 1 byte address
 * @param param 32-bit parameter
 */
void ONFI_SetFeatures(uint8_t addr, uint32_t param);

/**
 * ONFI_GetFeatures function reads the sub-feature parameters (P1-P4) from the
 * specified feature address. This command is accepted by the target only when
 * all LUNs on the target are idle.
 *
 * @param addr 1 byte address
 */
void ONFI_GetFeatures(uint8_t addr);

/**
 * ONFI_ReadStatus function returns the status of the last-selected LUN on a
 * target. In the case of non-interleaved operations, the Read Status function
 * retrieves a status value for the last operation issued.
 */
uint16_t ONFI_ReadStatus(void);

/**
 * ONFI_SelectLunWithStatus function returns the status of the addressed LUN
 * on a target even when it is busy (RDY = 0).
 */
uint16_t ONFI_SelectLunWithStatus(uint32_t addr);

/**
 * ONFI_ChangeReadColumn function changes the column address of the selected
 * cache register and enables data output from the last selected LUN.
 */
void ONFI_ChangeReadColumn(uint16_t addr);

/**
 * ONFI_SelectCacheRegister function enables data output on the addressed
 * LUN’s cache register at the specified column address.
 */
void ONFI_SelectCacheRegister(uint32_t rowaddr, uint16_t coladdr);

/**
 * ONFI_ChangeWriteColumn function changes the column address of the selected
 * cache register and enables data input on the last-selected LUN.
 */
void ONFI_ChangeWriteColumn(uint16_t addr);

/**
 * ONFI_ChangeRowAddress function changes the row address (block and page)
 * where the cache register contents will be programmed in the NAND Flash
 * array. It also changes the column address of the selected cache register
 * and enables data input on the specified LUN.
 */
void ONFI_ChangeRowAddress(uint32_t rowaddr, uint16_t coladdr);

/**
 * ONFI_ReadMode function reads a page of data identified by a row address for
 * the LUN specified.
 */
void ONFI_ReadMode(void);

/**
 * ONFI_ReadPage function copies a page from the NAND Flash array to its
 * respective cache register and enables data output.
 */
void ONFI_ReadPage(uint32_t rowaddr, uint16_t coladdr);

/**
 * ONFI_ReadCacheSequential function reads the next page in sequence within a
 * block into the data register, while the previous page is output from the
 * cache register.
 */
void ONFI_ReadPageCacheSequential(void);

/**
 * ONFI_ReadPageCacheRandom function reads the specified block and page into
 * the data register while the previous page is output from the cache register.
 */
void ONFI_ReadPageCacheRandom(uint32_t rowaddr, uint16_t coladdr);

/**
 * ONFI_ReadCacheLast function ends the READ PAGE CACHE sequence and copies a
 * page from the data register to the cache register.
 */
void ONFI_ReadCacheLast(void);

/**
 * ONFI_ProgramPage function transfers a page or portion of a page of data
 * identified by a column address to the page register.
 */
void ONFI_ProgramPage(uint32_t rowaddr, uint16_t coladdr);

void ONFI_ProgramPage_end(void);

void ONFI_ProgramPageInterleaved(void);

/**
 * ONFI_EraseBlock function erases the specified block in the NAND Flash array
 * . This command is accepted by the LUN when it is ready (RDY = 1, ARDY = 1).
 */
void ONFI_EraseBlock(uint32_t addr);

void ONFI_EraseBlockInterleaved(void);

/**
 * ONFI_CopybackRead function reads a page of data from one location and then
 * moves that data to a second location on the same LUN.
 */
void ONFI_CopybackRead(uint32_t rowaddr, uint16_t coladdr,uint32_t n);

void ONFI_CopybackProgramInterleaved(void);

/*
 * custom ONFI supporting functions
 */
void ONFI_receive8 (uint8_t  *buff, int32_t cnt);
void ONFI_receive16(uint16_t *buff, int32_t cnt);
void ONFI_dma_read(int32_t cnt, uint32_t flag);
void ONFI_dma_write(int32_t cnt, uint32_t flag);

static inline void ONFI_set_mode(uint8_t mode)
{
    NFC_REG[NFC_MODE] = (NFC_REG[NFC_MODE] & 0xfff0000) | 0x1f00 | mode;
}

static inline uint8_t ONFI_mode(void)
{
    return (uint8_t)NFC_REG[NFC_MODE];
}

void ONFI_set_speed(uint8_t mode);
int  ONFI_init(uint8_t speed);


#endif  /* __INCLUDE_ONFI_H */
