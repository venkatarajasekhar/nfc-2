/**
 * @file onfi.c
 *
 * @author fox.chao@gmail.com
 */

#include "ONFI.h"
#include "NFC.h"

#include "ONFI_timing.h"

static uint32_t onfi_speed;

/**
 * ONFI status
 *
 * <pre>
 *  [7] WP#  - If set to one, then the device is not write protected.
 *  [6] RDY  - If set to one, then the LUN or interleaved address is ready for
 *              another command and all other bits in the status value are
 *              valid.
 *  [5] ARDY - If set to one, then there is no array operation in progress.
 *  [4] VSP  - Vendor Specific
 *  [3] R
 *  [2] R
 *  [1] FAILC - If set to one, then the command issued prior to the last
 *               command failed.
 *  [0] FAIL  - If set to one, then the last command failed.
 * </pre>
 */
#define SR_WPn   7
#define SR_RDY   6
#define SR_ARDY  5
#define SR_VSP   4
#define SR_FAILC 1
#define SR_FAIL  0

/* Reset Operations */

void ONFI_Reset(void)
{
    CMDFF_SLOT_check(6);

    NFC_force(0x0500);              // FORCE 1, CE11, CE12
    NFC_nop(tCS[onfi_speed], 0);
    NFC_onfi(0xFF);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_nop(tRST_power_on,  0);
    NFC_wait(ONFI_WAIT_RDY | 0x100);
}

void ONFI_SynchronousReset(void)
{
    CMDFF_SLOT_check(5);

    NFC_nop(tCS[onfi_speed], 0);
    NFC_onfi(0xFC);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_nop(tRST_power_on, 0);
    NFC_wait(ONFI_WAIT_RDY | 0x100);
}

/* Identification Operations */

void ONFI_ReadID(uint8_t addr)
{
    CMDFF_SLOT_check(3 + 1);        // additional NFC_get

    NFC_onfi(0x90);
    NFC_1addr(addr);
    NFC_nop(tWHR[onfi_speed], 0);
}

void ONFI_ReadParameterPage(uint8_t addr)
{
    CMDFF_SLOT_check(5 + 1);        // additional NFC_get

    NFC_onfi(0xEC);
    NFC_1addr(addr);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_RDY | 0x100);
    NFC_nop(tRR[onfi_speed], 0);
}

void ONFI_ReadUniqueID(uint8_t addr)
{
    CMDFF_SLOT_check(5 + 1);        // additional NFC_get

    NFC_onfi(0xED);
    NFC_1addr(addr);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_RDY | 0x100);
    NFC_nop(tRR[onfi_speed], 0);
}

/* Configuration Operations */

void ONFI_SetFeatures(uint8_t addr, uint32_t param)
{
    CMDFF_SLOT_check(6);

    NFC_onfi(0xEF);
    NFC_1addr(addr);
    NFC_nop(tADL[onfi_speed], 0);
    NFC_put((uint8_t *)&param, 4);
    NFC_nop(tWB[onfi_speed], ONFI_TO_FLASH);
    NFC_wait(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY);
}

void ONFI_GetFeatures(uint8_t addr)
{
    CMDFF_SLOT_check(4 + 1);        // additional NFC_get

    NFC_onfi(0xEE);
    NFC_1addr(addr);
    NFC_nop(tWB[onfi_speed], 0);                 // NOP, tWB=200
    NFC_wait(ONFI_WAIT_RDY | 0x100);
}

//void ONFI_GetFeatures_end(void)
//{
//    NFC_nop(tRR[onfi_speed], 0);
//}

/* Status Operations */

uint16_t ONFI_ReadStatus(void)
{
    CMDFF_SLOT_check(3);

    NFC_onfi(0x70);
    NFC_nop(tWHR[onfi_speed], 0);
    NFC_get(1);

    return(NFC_FMICFF_read());
}

uint16_t ONFI_SelectLunWithStatus(uint32_t addr)
{
    CMDFF_SLOT_check(6);

    NFC_onfi(0x78);
    NFC_3addr(addr);
    NFC_nop(tWHR[onfi_speed], 0);
    NFC_get(1);

    return(NFC_FMICFF_read());
}

/* Column Address Operations */

void ONFI_ChangeReadColumn(uint16_t addr)
{
    CMDFF_SLOT_check(5);

    NFC_onfi(0x05);
    NFC_2addr(addr);
    NFC_onfi(0xE0);
    NFC_nop(tCCS[onfi_speed], 0);
}

void ONFI_SelectCacheRegister(uint32_t rowaddr, uint16_t coladdr)
{
    CMDFF_SLOT_check(9);

    NFC_onfi(0x06);
    NFC_nop(tRHW[onfi_speed], 0);
    NFC_2addr(coladdr);
    NFC_3addr(rowaddr);
    NFC_onfi(0xE0);
    NFC_nop(tCCS[onfi_speed], 0);
}

void ONFI_ChangeWriteColumn(uint16_t addr)
{
    CMDFF_SLOT_check(4);

    NFC_onfi(0x85);
    NFC_2addr(addr);
    NFC_nop(tCCS[onfi_speed], ONFI_TO_FLASH);
}

void ONFI_ChangeRowAddress(uint32_t rowaddr, uint16_t coladdr)
{
    CMDFF_SLOT_check(7);

    NFC_onfi(0x85);
    NFC_2addr(coladdr);
    NFC_3addr(rowaddr);
    NFC_nop(tCCS[onfi_speed], 0);
}

/* READ Operations */

void ONFI_ReadMode(void)
{
    NFC_onfi(0x00);
}

void ONFI_ReadPage(uint32_t rowaddr, uint16_t coladdr)
{
    CMDFF_SLOT_check(11);

    NFC_onfi(0x00);
    NFC_2addr(coladdr);
    NFC_3addr(rowaddr);
    NFC_onfi(0x30);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY);
    NFC_nop(tRR[onfi_speed], 0);
    NFC_onfi(0x00);
}

void ONFI_ReadCacheSequential(void)
{
    CMDFF_SLOT_check(4);

    NFC_onfi(0x31);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY);
    NFC_nop(tRR[onfi_speed], 0);
}

void ONFI_ReadPageCacheRandom(uint32_t rowaddr, uint16_t coladdr)
{
    CMDFF_SLOT_check(10);

    NFC_onfi(0x00);
    NFC_2addr(coladdr);
    NFC_3addr(rowaddr);
    NFC_onfi(0x31);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY);
    NFC_nop(tRR[onfi_speed], 0);
}

void ONFI_ReadCacheLast(void)
{
    CMDFF_SLOT_check(4);

    NFC_onfi(0x3F);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY);
    NFC_nop(tRR[onfi_speed], 0);    
}

/* Program Operations */

void ONFI_ProgramPage(uint32_t rowaddr, uint16_t coladdr)
{
    CMDFF_SLOT_check(7);

    NFC_onfi(0x80);
    NFC_2addr(coladdr);
    NFC_3addr(rowaddr);
    NFC_nop(tADL[onfi_speed], 0);
}

void ONFI_ProgramPage_end(void)
{
    CMDFF_SLOT_check(3);

    NFC_onfi(0x10);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY);
}

void ONFI_ProgramPageInterleaved(void)
{
    
}

void ONFI_PageCacheProgram(void)
{
    
}

/* ERASE Operations */

void ONFI_EraseBlock(uint32_t addr)
{
    CMDFF_SLOT_check(7);

    NFC_onfi(0x60);
    NFC_3addr(addr);
    NFC_onfi(0xD0);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_RDY | 0x100);
}

/**
 * ONFI_EraseBlock(addr) queues a block in the specified plane to be erased in
 * the NAND Flash array.
 */
void ONFI_EraseBlockMultiPlane(uint32_t addr)
{
    CMDFF_SLOT_check(7);

    NFC_onfi(0x60);
    NFC_3addr(addr);
    NFC_onfi(0xD1);
    NFC_nop(tWB[onfi_speed], 0);
    NFC_wait(ONFI_WAIT_RDY | 0x100);
}

/* COPYBACK Operations */

void ONFI_CopybackRead(uint32_t rowaddr, uint16_t coladdr, uint32_t cnt)
{
    CMDFF_SLOT_check(9);

    NFC_onfi(0x00);
    NFC_2addr(coladdr);
    NFC_3addr(rowaddr);
    NFC_onfi(0x35);
    NFC_nop(tWB[onfi_speed], 0);
// FIXME:   NFC_nop(tR);
    NFC_get(cnt);    
}

void ONFI_CopybackProgramInterleaved(void)
{
    
}

/* One-Time Programmable (OTP) Operations */

/* ================================================================== */

/**
 * ONFI_receive function receives data via FMICFF using a 16-bit buffer.
 */
void ONFI_receive8(uint8_t *buff, int32_t cnt)
{
    NFC_get(cnt);

    while (cnt--)
	    *(buff++) = (uint8_t)NFC_FMICFF_read();
}

/**
 * ONFI_receive function receives data via FMICFF using a 16-bit buffer.
 */
void ONFI_receive16(uint16_t *buff, int32_t cnt)
{
    NFC_get(cnt);

    while (cnt--)
	    *(buff++) = NFC_FMICFF_read();
}

/**
 * ONFI_dma_read(cnt, flag)
 */
void ONFI_dma_read(int32_t cnt, uint32_t flag)
{
    CMDFF_SLOT_check(2);

    NFC_dma_get(cnt, flag);
    NFC_nop(tWHR[onfi_speed], ONFI_USE_DMA | flag);
}

/**
 * ONFI_dma_write(cnt, flag)
 */
void ONFI_dma_write(int32_t cnt, uint32_t flag)
{
    CMDFF_SLOT_check(2);

    NFC_dma_put(cnt, flag);
    NFC_nop(tWHR[onfi_speed], ONFI_USE_DMA | ONFI_TO_FLASH | flag);
}

/* ================================================================== */

/**
 * ONFI_set_speed(speed)
 *
 * @param speed new speed mode
 */
void ONFI_set_speed(uint8_t speed)
{
    // !!! do not change CMDTBL unless SET_FEATURE is completed !!!
	// !!! watch out for the latency of CMDFF commands execution !!!
    switch (speed)
    {
        case 0:
            break;
        default:
        	//----------------------------------------------------------------
        	// Assume clock freq. of 33.33MHz (30ns clock)
        	// Start using mode 4 timing
        	//----------------------------------------------------------------
        	// CMDTBL
        	NFC_CMDTBL[0]=0x00000049;	//OP=CMD_1,
        	NFC_CMDTBL[1]=0x00000048;	//OP=CMD_2, tWP=12
        	NFC_CMDTBL[2]=0x000000c1;	//OP=CMD_3, tCLH=5
        	NFC_CMDTBL[3]=0x0000004a;	//OP=ADDR_1
        	NFC_CMDTBL[4]=0x00000048;	//OP=ADDR_2, tWP=12
        	NFC_CMDTBL[5]=0x000000c2;	//OP=ADDR_3, tALH=5
        	NFC_CMDTBL[6]=0x00000004;	//OP=GET_1, tRC-tRP=13
        	NFC_CMDTBL[7]=0x00000084;	//OP=GET_1, tRP=12
        	NFC_CMDTBL[8]=0x00000048;	//OP=PUT_1, tWC-tWp=13
        	NFC_CMDTBL[9]=0x000000c8;	//OP=PUT_1, tWP=12
    }
    onfi_speed = speed;
}

/**
 * ONFI_init function accepts the mode parameter to set NFC's mode register.
 *
 * @param mode set the mode of bit0-bit7
 */
int ONFI_init(uint8_t mode)
{
    NFC_init();

    ONFI_set_mode(mode);
    // Enable FMC and auto ReadStatus 
    NFC_REG[NFC_ENABLE] |= (NFC_ENABLE_FMC | NFC_ENABLE_RS);
    onfi_speed = 0;

	//----------------------------------------------------------------
	// Assume clock freq. of 33.33MHz (30ns clock)
	// Prepare CMDPTR and CMDTBL for async mode 0 and sync mode 1
	//----------------------------------------------------------------
	// CMDPTR
	NFC_CMDPTR[0]=0x00000200;	//OP=CMD
	NFC_CMDPTR[1]=0x00000263;	//OP=ADDR
	NFC_CMDPTR[2]=0x000002c6;	//OP=GET
	NFC_CMDPTR[3]=0x000002e8;	//OP=PUT
	NFC_CMDPTR[4]=0x000001ce;	//OP=NOP
	NFC_CMDPTR[6]=0x000001ef;	//OP=FORCE
	//NFC_CMDPTR[8]=0x00000010;	//OP=CMD(SYNC)
	//NFC_CMDPTR[9]=0x00000013;	//OP=ADDR(SYNC)
	//NFC_CMDPTR[10]=0x00000016;	//OP=GET(SYNC)
	//NFC_CMDPTR[11]=0x00000017;	//OP=PUT(SYNC)

	// CMDTBL
	NFC_CMDTBL[0]=0x00000049;	//OP=CMD_1
	NFC_CMDTBL[1]=0x00000148;	//OP=CMD_2
	NFC_CMDTBL[2]=0x000000c1;	//OP=CMD_3
	NFC_CMDTBL[3]=0x0000004a;	//OP=ADDR_1
	NFC_CMDTBL[4]=0x00000148;	//OP=ADDR_2
	NFC_CMDTBL[5]=0x000000c2;	//OP=ADDR_3
	NFC_CMDTBL[6]=0x00000104;	//OP=GET_1
	NFC_CMDTBL[7]=0x00000184;	//OP=GET_1
	NFC_CMDTBL[8]=0x00000148;	//OP=PUT_1
	NFC_CMDTBL[9]=0x000001c8;	//OP=PUT_1
	NFC_CMDTBL[14]=0x00000080;	//OP=NOP_1
	NFC_CMDTBL[15]=0x00000090;	//OP=FORCE_1
	NFC_CMDTBL[16]=0x00000041;	//OP=CMD_1
	NFC_CMDTBL[17]=0x00000041;	//OP=CMD_2
	NFC_CMDTBL[18]=0x000000c0;	//OP=CMD_3 , tCAD
	NFC_CMDTBL[19]=0x00000042;	//OP=ADDR_1
	NFC_CMDTBL[20]=0x00000042;	//OP=ADDR_2
	NFC_CMDTBL[21]=0x000000c0;	//OP=ADDR_3, tCAD
	NFC_CMDTBL[22]=0x000000a3;	//OP=GET_1
	NFC_CMDTBL[23]=0x000000e3;	//OP=PUT_1

	//----------------------------------------------------------------
	// Prepare HW Read Status RG_RS0 - RG_RS3
	// for async16 mode 0
    NFC_REG[NFC_RS_CMD] = 0x00007020;   // CMD, 70h
    NFC_REG[NFC_RS_NOP] = 0x00000384;   // NOP, tWHR=120
    NFC_REG[NFC_RS_GET] = 0x00000082;   // GET, 1 cycles
    NFC_REG[NFC_RS_WHR] = 0x01000684;   // NOP, tWHR=200, rs_last

    //----------------------------------------------------------------
    // Set up BCHE Main Data, parameters from config.h
	// For sync. mode N must be DWORD aligned
    NFC_REG[NFC_KN_DATA] = ((kNLEN_DATA - 1) << 16) | (kKLEN_DATA - 1);
    NFC_REG[NFC_TS_DATA] = (kSEED_DATA << 16) | kT_DATA;

    // Set up BCHE Aux Data, parameters from config.h
    NFC_REG[NFC_KN_AUX] = ((kNLEN_AUX - 1) << 16) | (kKLEN_AUX - 1);
    NFC_REG[NFC_TS_AUX] = (kSEED_AUX << 16) | kT_AUX;

    return 0;
}
