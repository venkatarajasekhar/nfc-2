/**
 * @file test_ONFI.c
 *
 * This testsuit tests asynchronous Flash compatibility to ONFI
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
    ONFI_receive(buff, 4);

    TEST_ASSERT_EQUAL_HEX16(0x0505, buff[0]);
    TEST_ASSERT_EQUAL_HEX32(0, get_count(NFC_REG[NFC_FMICFF_SLOT]));
    TEST_ASSERT_EQUAL_HEX16(0xe0e0, ONFI_ReadStatus());
}

void test_ONFI_onfi_speed_ShouldWork(void)
{
    ONFI_onfi_speed(5);
    TEST_ASSERT_EQUAL_HEX16(0xe0e0, ONFI_ReadStatus());    
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

unsigned int page_buff[kPAGE_BYTES / 4];

void setup_page_buff(void)
{
    // fill some patterns
	for (int i = 0; i < (kPAGE_BYTES / 4); i++)
	    page_buff[i] = i;
}

void test_ReadPage_via_FMICFF_ShouldWork(void)
{
    unsigned int read_bytes = 8192;

    setup_page_buff();

    ONFI_ReadPage(0x010004, 0x0);
    ONFI_receive((uint16_t *)page_buff, read_bytes / 2);

    TEST_ASSERT_EQUAL_UINT(0xFFFFFFFF, fletcher32((uint16_t *)page_buff, read_bytes / 2));
}

void ReadPage_via_DMARFF(unsigned int cnt)
{
    unsigned const int data_bytes = (kKLEN_DATA << 1) * cnt;
    unsigned const int data_pack  = (kNLEN_DATA << 1) * cnt;

    unsigned const int aux_bytes  = (kKLEN_AUX << 1) * cnt;
    unsigned const int aux_pack   = (kNLEN_AUX << 1) * cnt;

    setup_page_buff();

	// Enable Paths
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_F2H | NFC_ENABLE_BCHD;

    ONFI_ReadPage(0x010004, 0x0);
    ONFI_dma_read((data_pack / 2), 0);
    DBGCMDFF(0);

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
    TEST_ASSERT_EQUAL_UINT(0xFFFFFFFF, fletcher32((uint16_t *)page_buff, data_bytes / 2));

	// Disable Paths
    NFC_REG[NFC_ENABLE] &= ~(NFC_ENABLE_F2H | NFC_ENABLE_BCHD);
}

void test_ReadPage_via_DMARFF_ShouldWork(void)
{
    ReadPage_via_DMARFF(16);
}

void ReadPage_via_RSDM1(int cnt)
{
    unsigned const int data_bytes = (kKLEN_DATA << 1) * cnt;
    unsigned const int data_pack  = (kNLEN_DATA << 1) * cnt;

    unsigned const int aux_bytes  = (kKLEN_AUX << 1) * cnt;
    unsigned const int aux_pack   = (kNLEN_AUX << 1) * cnt;

    setup_page_buff();

	//----------------------------------------------------------------
	// Set up DMAR
	NFC_REG[NFC_DMAR_ENDCNT] = (data_bytes / 8) - 1; // DMAR_ENDCNT
	//----------------------------------------------------------------
	// Enable Paths
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAR | NFC_ENABLE_F2H | NFC_ENABLE_BCHD;

    ONFI_ReadPage(0x010004, 0x0);
    ONFI_dma_read((data_pack / 2), 0);

    receive(page_buff, data_bytes);

    TEST_ASSERT_EQUAL_UINT(0, get_count(NFC_REG[NFC_DMARFF_SLOT]));
    TEST_ASSERT_EQUAL_UINT(0xFFFFFFFF, fletcher32((uint16_t *)page_buff, data_bytes / 2));

	//----------------------------------------------------------------
	// Disable Paths
    NFC_REG[NFC_ENABLE] &= ~(NFC_ENABLE_DMAR | NFC_ENABLE_F2H);
}

void test_ReadPage_via_RSDM1_ShouldWork(void)
{
    ReadPage_via_RSDM1(16);
}
