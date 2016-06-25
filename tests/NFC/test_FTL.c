/**
 * @file test_FTL.c
 *
 * This testsuit tests if NFC's FTL works.
 *
 * @author fox.chao@gmail.com
 */

#include "NFC.h"
#include "ONFI.h"

#include "unity.h"
#include "helpers.h"

void setUp(void)
{
}

void tearDown(void)
{
}

#include "crypto_ext.h"

struct cryptodma_t dmawdma;
struct cryptodma_t dmardma;

#define MAX_BYTES   4096
static unsigned int in_data [MAX_BYTES >> 2];
static unsigned int out_buff[MAX_BYTES >> 2];

void test_init_ShouldWork(void)
{
    NFC_init();

    // Additional settings to make loopback work
	GPOO[2] = 0x00000009; // mode 1
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	// For Loopback test we init CMDTBLE to all Fs
	for (int i = 0; i < 32; i++)
	{
	    NFC_CMDTBL[i] = 0xffffffff;
	}

    //----------------------------------------------------------------
    // Set up BCHE Main Data, parameters from config.h
	// For sync. mode N must be DWORD aligned
    NFC_REG[NFC_KN_DATA] = ((kNLEN_DATA - 1) << 16) | (kKLEN_DATA - 1);
    NFC_REG[NFC_TS_DATA] = (kSEED_DATA << 16) | kT_DATA;

    // Set up BCHE Aux Data, parameters from config.h
    NFC_REG[NFC_KN_AUX] = ((kNLEN_AUX - 1) << 16) | (kKLEN_AUX - 1);
    NFC_REG[NFC_TS_AUX] = (kSEED_AUX << 16) | kT_AUX;
}

void test_FTLSIZE_ShouldWork(void)
{
    int i;

    NFC_FTL_access_on();
    for (i=0; i<1024; i++)
        NFC_FTL[i] = -i;

    TEST_ASSERT_EQUAL_UINT32(-5,    NFC_FTL[5]);
    TEST_ASSERT_EQUAL_UINT32(-1023, NFC_FTL[1023]);
    NFC_FTL_access_off();
}

void dump_out_buff(int start, int cnt)
{
    DBGPRINTF("%8x:", start);

    for (int i = 0; i < cnt; i++)
    {
    	if ((i % 4) == 0)
    	{
    	    DBGPRINTF("\n");
    	}
        DBGPRINTF("%8x ", out_buff[start + i]);
    }
    DBGPRINTF("\n");
}

/* DMAW write in */
PT_THREAD(DMAWFF_write(struct pt *pt))
{
    PT_BEGIN(pt);

    while (dmawdma.length)
    {
        PT_WAIT_UNTIL(pt, get_slot(NFC_REG[NFC_DMAWFF_SLOT]));
        NFC_DMAWFF[0] = *(dmawdma.data++);  // lower DWORD
        NFC_DMAWFF[0] = *(dmawdma.data++);  // upper DWORD

        dmawdma.length--;
    }

    PT_WAIT_WHILE(pt, 1);

    PT_END(pt);
}

/* DMAR read out */
PT_THREAD(DMARFF_read(struct pt *pt))
{
    PT_BEGIN(pt);

    while (1)
    {
        PT_WAIT_UNTIL(pt, get_count(NFC_REG[NFC_DMARFF_SLOT]));

        NFC_DMARFFRD[0] = 2;    // pop
        NFC_DMARFFRD[0] = 0;    // lower DOWRD
        *(dmardma.data++) = NFC_DMARFFRD[0];
        NFC_DMAWFFRD[0] = 1;    // upper DWORD
        *(dmardma.data++) = NFC_DMARFFRD[0];

        dmardma.length--;
    }

    PT_END(pt);
}

void test_DMAW_loopback_DMAR_ShouldWork(void)
{
    int i;
    int cnt = 1024;

    static struct pt pt1;
    static struct pt pt2;

    // initialize the test patterns
	for (i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0x12345678;
        out_buff[i] = 0xFFFFFFFF;
	}

    NFC_REG[NFC_H2F_ENDCNT]  = (cnt >> 3) - 1;

    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_H2F | NFC_ENABLE_BCHE |
                           NFC_ENABLE_LOOPBACK |
                           NFC_ENABLE_BCHD | NFC_ENABLE_F2H;

    dmawdma.data   = in_data;
    dmawdma.length = cnt >> 3;
    dmardma.data   = out_buff;
    dmardma.length = cnt >> 3;

    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (dmardma.length) {
    	DMAWFF_write(&pt1);
        DMARFF_read (&pt2);
    };

    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMAWFF_SLOT]);
    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMARFF_SLOT]);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 1024);
// disable PATH
    NFC_REG[NFC_ENABLE] = 0;
}

void test_AUX_loopback_DMAR_ShouldWork(void)
{
    int i;
    int cnt = 1024;

    static struct pt pt1;
    static struct pt pt2;

    // initialize the test patterns
	for (i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0x12345678;
        out_buff[i] = 0xFFFFFFFF;
	}

    NFC_REG[NFC_H2F_ENDCNT]  = (cnt >> 3) - 1;
    NFC_REG[NFC_AUX_ENDCNT] = 2 - 1;
    NFC_REG[NFC_AUX_SAR] = 0;

    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_AUX |
                           NFC_ENABLE_H2F | NFC_ENABLE_BCHE |
                           NFC_ENABLE_LOOPBACK |
                           NFC_ENABLE_BCHD | NFC_ENABLE_F2H;

    dmawdma.data   = in_data;
    dmawdma.length = cnt >> 3;
    dmardma.data   = out_buff;
    dmardma.length = cnt >> 3;

    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (dmardma.length > 0 ) {
    	DMAWFF_write(&pt1);
        DMARFF_read (&pt2);
    };

    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMAWFF_SLOT]);
    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMARFF_SLOT]);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);

    dump_out_buff(254, 8);

// disable PATH
    NFC_REG[NFC_ENABLE] = 0;
}

void test_FTL_loopback_DMAR_ShouldWork(void)
{
    int i;
    int cnt = 1024;

    static struct pt pt1;
    static struct pt pt2;

    TEST_ABORT();

    NFC_FTL_access_on();
    // initialize the test patterns
    for (i=0; i<256; i++)
        NFC_FTL[i] = i;

    // end | count | offset
    NFC_FTL[0x100] = 0x80000000 | ((cnt >> 3) - 1) << 12 | 0;
    NFC_REG[NFC_LIST_SAR] = 0x100;

    NFC_REG[NFC_H2F_ENDCNT]  = (cnt >> 3) - 1;
//    NFC_FTL_access_off();

    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_FTLRD | NFC_ENABLE_H2F |
                           NFC_ENABLE_BCHE | NFC_ENABLE_LOOPBACK |
                           NFC_ENABLE_BCHD | NFC_ENABLE_F2H;

    dmardma.data   = out_buff;
    dmardma.length = cnt >> 3;

    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (dmardma.length > 0 ) {
    	DMAWFF_write(&pt1);
        DMARFF_read (&pt2);
        DBGPRINTF("%x\n", dmardma.length);
    };

    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMARFF_SLOT]);
//    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);
    dump_out_buff(0, 8);

// disable PATH
    NFC_REG[NFC_ENABLE] = 0;
}

void test_DMAW_loopback_FTL_ShouldWork(void)
{
    int i;
    int cnt = 1024;

    static struct pt pt1;
    static struct pt pt2;

    NFC_FTL_access_on();
    // initialize the test patterns
    for (i=0; i<256; i++)
        NFC_FTL[i] = i;

    // FTL_entry = end_bit | (count - 1) << 12 | offset
    NFC_FTL[0x100] = 0x80000000 | ((cnt >> 3) - 1) << 12 | 0x200;
    NFC_REG[NFC_LIST_SAR] = 0x100;

    NFC_REG[NFC_H2F_ENDCNT]  = (cnt >> 3) - 1;
//    NFC_FTL_access_off();

    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_H2F | NFC_ENABLE_BCHE |
                           NFC_ENABLE_LOOPBACK | NFC_ENABLE_BCHD | 
                           NFC_ENABLE_F2H | NFC_ENABLE_FTLWR;

    dmardma.data   = out_buff;
    dmardma.length = cnt >> 3;

    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (dmawdma.length > 0 ) {
    	DMAWFF_write(&pt1);
    };

    for (i=0; i<4; i++)
        DBGPRINTF("%x\n", NFC_FTL[0x200 + i]);

    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMARFF_SLOT]);
//    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);

// disable PATH
    NFC_REG[NFC_ENABLE] = 0;
}
