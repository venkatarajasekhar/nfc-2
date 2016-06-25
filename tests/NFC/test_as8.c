/**
 * @file test_as8.c
 *
 * This testsuit tests asynchronous 8-bit Flash compatibility to ONFI
 *
 * @author fox.chao@gmail.com
 */

#include "ONFI.h"
#include "NFC.h"
#include "crypto_ext.h"

#include "unity.h"
#include "helpers.h"

void setUp(void)
{
    DBGCMDFF(0);
}

void tearDown(void)
{
}

void test_ONFI_init_ShouldBeOK(void)
{
    TEST_ASSERT_EQUAL_INT(0, ONFI_init(0));
}

void test_ONFI_Reset_ShouldWork(void)
{
    ONFI_Reset();
    TEST_ASSERT_EQUAL_HEX16(0xe0, ONFI_ReadStatus());
}

void test_ONFI_ReadID_ShouldRead_ONFI(void)
{
    static const int8_t expected[] = "ONFI";

    uint8_t buff[5];
    buff[4] = 0;

    ONFI_ReadID(0x20);
    ONFI_receive8(buff, 4);

    TEST_ASSERT_EQUAL_STRING(expected, buff);    
}

void test_ONFI_ReadParameterPage_ShouldRead_ONFI(void)
{
    static const int8_t expected[] = "ONFI";

    uint8_t buff[5];
    buff[4] = 0;

    ONFI_ReadParameterPage(0);
    ONFI_receive8(buff, 4);

    TEST_ASSERT_EQUAL_STRING(expected, buff);
}

void test_ONFI_Features_CouldBeChange(void)
{
    ONFI_SetFeatures(1, 0x05000000);
    TEST_ASSERT_EQUAL_HEX8(0xe0, ONFI_ReadStatus());

//----------------------------------------------------------------
// !!! need to check if set to mode 4 successfully here !!!
//----------------------------------------------------------------
    uint8_t buff[5];
    buff[4] = 0;

    ONFI_GetFeatures(1);
    ONFI_receive8(buff, 4);

    TEST_ASSERT_EQUAL_HEX8(0x05, buff[0]);
    TEST_ASSERT_EQUAL_HEX32(0, get_count(NFC_REG[NFC_FMICFF_SLOT]));
    TEST_ASSERT_EQUAL_HEX8(0xe0, ONFI_ReadStatus());
}

void test_ONFI_set_speed_ShouldWork(void)
{
    ONFI_set_speed(5);
    TEST_ASSERT_EQUAL_HEX16(0xe0, ONFI_ReadStatus());
    
}

void test_ONFI_EraseBlock_ShouldWork(void)
{
    TEST_ABORT();
    ONFI_EraseBlock(0x010200);
}

// To make crypto's DMA work, declare as followed.
extern struct device_extension_s dx;

void transmit(unsigned int *data, int length)
{
    static struct pt pt1;

    crypto_rsem1_request(&dx.rsem1, data, length, 0x8d);

    PT_INIT(&pt1);
    do {
    	rsem1_nfc(&pt1);
    } while (dx.rsem1.length || (!is_NFC_DMAW_done()));

    crypto_rsem1_disable();
}

void receive(unsigned int *data, int length)
{
    static struct pt pt1;

    crypto_rsdm1_request(&dx.rsdm1, data, length, 0x97);

    PT_INIT(&pt1);
    do {
    	nfc_rsdm1(&pt1);
    } while ((dx.rsdm1.length) || (!is_NFC_DMAR_done()));

    crypto_rsdm1_disable();
}

void setup_NFC_FTL(void)
{
    uint32_t tp_i, tp_addr, tp_rdat, tp_wdat;

    NFC_FTL_access_on();

//  Generate patterns as followed:
//      0x3a393837, 0x3b3a3938, 0x3c3b3a39, 0x3d3c3b3a,
//      0x3e3d3c3b, 0x3f3e3d3c, 0x403f3e3d, 0x41403f3e
	tp_rdat = NFC_REG[NFC_AUX_SAR];
	for (tp_i=0; tp_i<8; tp_i++)
	{
		tp_wdat = tp_i+55;
		tp_addr = tp_rdat + tp_i;
		NFC_FTL[tp_addr] = (((tp_wdat+3)<< 24)+((tp_wdat+2)<<16)+((tp_wdat+1)<<8)+(tp_wdat));
	}

	// Set up FTL List for AUX write to FTLRAM
	NFC_REG[NFC_LIST_SAR] = 0x00000100; // (DWORD address)
	tp_rdat = NFC_REG[NFC_LIST_SAR];
	for (tp_i=0; tp_i<2; tp_i++)
	{
		if (tp_i==0)	tp_wdat = 0x00001200+(tp_i*20); // (2 DWORD)
		else		    tp_wdat = 0x80000200+(tp_i*20); // (eol, 1 DWORD)
		tp_addr = tp_rdat + tp_i;
		NFC_FTL[tp_addr] = tp_wdat;
	}
}

unsigned int page_buff[kPAGE_BYTES / 4];

void setup_page_buff(void)
{
    // fill some patterns
	for (int i = 0; i < (kPAGE_BYTES / 4); i++)
	    page_buff[i] = i;
}

void test_ONFI_ProgramPage_ShouldWork(void)
{
    TEST_ABORT();

    unsigned const int data_bytes = kKLEN_DATA << 1;
    unsigned const int data_pack  = kNLEN_DATA << 1;

    unsigned const int aux_bytes = kKLEN_AUX << 1;
    unsigned const int aux_pack  = kNLEN_AUX << 1;

    setup_page_buff();
    setup_NFC_FTL();

	//----------------------------------------------------------------
	// Set up DMAW
	NFC_REG[NFC_DMAW_ENDCNT] = (data_bytes / 8) - 1;        // (QWORD)
	NFC_REG[NFC_H2F_ENDCNT]  = (data_bytes / 8) - 1;        // (QWORD)
	NFC_REG[NFC_AUX_ENDCNT]  = ((aux_bytes + 7) / 8) - 1;   // (2 QWORD > 6 WORD)
	//----------------------------------------------------------------
	// Enable Paths
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAW | NFC_ENABLE_AUX | NFC_ENABLE_BCHE | NFC_ENABLE_H2F;    

//  Generate patterns as followed:
//      0x03020100, 0x04030201, 0x05040302, 0x0605040302...
  	for (int i = 0; i< (data_bytes / 4); i++)
  	    page_buff[i] = ((i+3)<<24)+((i+2)<<16)+((i+1)<<8)+i;

    transmit(page_buff, data_bytes);

	//----------------------------------------------------------------
	// Write and Program Flash Memory
	//----------------------------------------------------------------
	//======= WRITE DATA TO NAND CACHE (Group 1) =======
    ONFI_ProgramPage(0x001004, 0x0000);
    ONFI_dma_write((data_pack / 2), 0);

	//======= CHANGE WRITE COLUMN ADDRESS (Group 1) =======
    ONFI_ChangeWriteColumn(0x10D);
    ONFI_dma_write((aux_pack / 2), ONFI_AUX_DATA);

	//======= WRITE DATA TO NAND CACHE (Group 1) =======
    ONFI_ProgramPage_end();

	//----------------------------------------------------------------
	// Check for MUX_DONE
    do {}  while ((NFC_REG[NFC_STATE] & 0x0000ff00) != 0x100);

    TEST_ASSERT_EQUAL_UINT32(0, get_count(NFC_REG[NFC_DMAWFF_SLOT]));

	//----------------------------------------------------------------
	// Disable Paths
    NFC_REG[NFC_ENABLE] &= ~(NFC_ENABLE_DMAW | NFC_ENABLE_AUX | NFC_ENABLE_BCHE | NFC_ENABLE_H2F);
}

void test_ONFI_ReadPage_ShouldWork(void)
{
    unsigned const int data_bytes = kKLEN_DATA << 1;
    unsigned const int data_pack  = kNLEN_DATA << 1;

    unsigned const int aux_bytes  = kKLEN_AUX << 1;
    unsigned const int aux_pack   = kNLEN_AUX << 1;

    setup_page_buff();

    setup_NFC_FTL();

    TEST_ASSERT_EQUAL_HEX32(0x40, NFC_REG[NFC_DMARFF_SLOT]);

	//----------------------------------------------------------------
	// Set up DMAR
	NFC_REG[NFC_DMAR_ENDCNT] = (data_bytes / 8) - 1;
	//----------------------------------------------------------------
	// Enable Paths
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAR | NFC_ENABLE_F2H | NFC_ENABLE_BCHD;

    ONFI_ReadPage(0x010004, 0x0);
    ONFI_dma_read((data_pack / 2), 0);

    ONFI_ChangeReadColumn(0x010D);
    ONFI_dma_read((aux_pack / 2), ONFI_AUX_DATA);

    receive(page_buff, data_bytes);

	//----------------------------------------------------------------
	// Check FLASH data
    DBGPRINTF("ReadPage_via_RSDM1:\n");
    for (int i = 0; i < 2; i++)
        DBGPRINTF("%08x, ", page_buff[i]);
    DBGPRINTF("\n");
    for (int i = 0; i < 4; i++)
        DBGPRINTF("%08x, ", page_buff[(data_bytes / 4) - 2 + i]);
    DBGPRINTF("\n");

	//----------------------------------------------------------------
	// Disable Paths
    NFC_REG[NFC_ENABLE] &= (~NFC_ENABLE_DMAR);

    // Writing FTLRAM
	//----------------------------------------------------------------
	// Enable Paths
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_FTLWR;
	//----------------------------------------------------------------
	// wait for FTL_DONE
    while ((NFC_REG[NFC_STATE] >> 24) != 1) {}
	//----------------------------------------------------------------
	// Disable Paths
    NFC_REG[NFC_ENABLE] &= (~NFC_ENABLE_FTLWR);

    NFC_DMARFFRD[0] = 2;    // pop
    NFC_DMARFFRD[0] = 0;    // lower DOWRD
    DBGPRINTF("%x, ", NFC_DMAWFFRD[0]);
    NFC_DMARFFRD[0] = 1;    // upper DWORD
    DBGPRINTF("%x, ", NFC_DMAWFFRD[0]);

    TEST_ASSERT_EQUAL_HEX32(0x40, NFC_REG[NFC_DMARFF_SLOT]);
}
