/**
 * @file test_dump.c
 *
 * This testsuit dump a 
 *
 * @author fox.chao@gmail.com
 */

#include "ONFI.h"
#include "NFC.h"

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
    TEST_ASSERT_EQUAL_INT(0, ONFI_init(NFC_MODE_D16));
}

void test_ONFI_Reset_ShouldWork(void)
{
    ONFI_Reset();
    TEST_ASSERT_EQUAL_HEX16(0xe0e0, ONFI_ReadStatus());
}

void test_ONFI_Features_ShouldBeChanged(void)
{
    ONFI_SetFeatures(1, 0x05000000);
    TEST_ASSERT_EQUAL_HEX16(0xe0e0, ONFI_ReadStatus());

//----------------------------------------------------------------
// !!! need to check if set to mode successfully here !!!
//----------------------------------------------------------------
    uint16_t buff[5];
    buff[4] = 0;

    ONFI_GetFeatures(1);
    ONFI_receive16(buff, 4);

    TEST_ASSERT_EQUAL_HEX16(0x0505, buff[0]);
    TEST_ASSERT_EQUAL_HEX32(0, get_count(NFC_REG[NFC_FMICFF_SLOT]));
    TEST_ASSERT_EQUAL_HEX16(0xe0e0, ONFI_ReadStatus());
}

void test_ONFI_set_speed_ShouldWork(void)
{
    ONFI_set_speed(5);
    TEST_ASSERT_EQUAL_HEX16(0xe0e0, ONFI_ReadStatus());    
}

unsigned int page_buff[kPAGE_BYTES / 4];

void setup_page_buff(void)
{
    // fill some patterns
	for (int i = 0; i < (kPAGE_BYTES / 4); i++)
	    page_buff[i] = 0;
}

unsigned int page_buff[kPAGE_BYTES / 4];

void ReadPage_via_DMARFF(uint32_t row_addr, uint16_t col_addr, int cnt)
{
    unsigned const int data_bytes = (kKLEN_DATA << 1) * cnt;
    unsigned const int data_pack  = (kNLEN_DATA << 1) * cnt;

    unsigned const int aux_bytes  = (kKLEN_AUX << 1) * cnt;
    unsigned const int aux_pack   = (kNLEN_AUX << 1) * cnt;

    setup_page_buff();

	// Enable Paths
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_F2H | NFC_ENABLE_BCHD;

    ONFI_ReadPage(row_addr, col_addr);
    ONFI_dma_read((data_pack / 2), 0);

    for (int i = 0; i < (data_bytes / 8); i++)
    {
        do {} while (!get_count(NFC_REG[NFC_DMARFF_SLOT]));

        NFC_DMARFFRD[0] = 2;    // pop
        NFC_DMARFFRD[0] = 0;    // lower DOWRD
        page_buff[i << 1] = NFC_DMARFFRD[0];
        NFC_DMARFFRD[0] = 1;    // upper DWORD
        page_buff[(i << 1) + 1] = NFC_DMARFFRD[0];
    }

    TEST_ASSERT_EQUAL_UINT(0, get_count(NFC_REG[NFC_DMARFF_SLOT]));

	// Disable Paths
    NFC_REG[NFC_ENABLE] &= ~(NFC_ENABLE_F2H | NFC_ENABLE_BCHD);
}

void test_ReadPage_ShouldDump(void)
{
    uint32_t row_addr = 0x010203;
    uint16_t col_addr = 0x0420;

    ReadPage_via_DMARFF(row_addr, col_addr, 1);

    DBGPRINTF("[%08x]:[%04x]", row_addr, col_addr);
    dump(page_buff, 128);
}
