/**
 * @file test_NFC.c
 *
 * This testsuit tests if NFC's DMA serialization works.
 *
 * @author fox.chao@gmail.com
 */

#include "NFC.h"
#include "ONFI.h"
#include "crypto_ext.h"

#include "unity.h"
#include "helpers.h"

void setUp(void)
{
}

void tearDown(void)
{
}

struct cryptodma_t dmawdma;
struct cryptodma_t dmardma;

#define MAX_BYTES   4096
static unsigned int in_data [MAX_BYTES >> 2];
static unsigned int out_buff[MAX_BYTES >> 2];

void test_init_ShouldWork(void)
{
    NFC_init();
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

/* A general routine for DMAW */
PT_THREAD(NFC_DMAW_cpu(struct pt *pt))
{
    PT_BEGIN(pt);

    while (1)
    {
        PT_WAIT_UNTIL(pt, get_count(NFC_REG[NFC_DMAWFF_SLOT]));

        NFC_DMAWFFRD[0] = 2;    // pop
        NFC_DMAWFFRD[0] = 0;    // lower DOWRD
        *(dmawdma.data++) = NFC_DMAWFFRD[0];
        NFC_DMAWFFRD[0] = 1;    // upper DWORD
        *(dmawdma.data++) = NFC_DMAWFFRD[0];
    }

    PT_END(pt);
}

// To make crypto's DMA work, declare as followed.
extern struct device_extension_s dx;

void rsem1_to_dmaw(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
    static struct pt pt1;
    static struct pt pt2;

    do {} while (is_NFC_DMAW_in_use());
    NFC_DMAW_request(cnt);

    dmawdma.data = o_buff;

    do {} while (is_rsem1_in_use());
    crypto_rsem1_request(&dx.rsem1, i_buff, cnt, 0x8d);

    PT_INIT(&pt1);
    PT_INIT(&pt2);
    while (dx.rsem1.length || !is_NFC_DMAW_done()) {
    	rsem1_nfc(&pt1);
        NFC_DMAW_cpu(&pt2);
    };

    NFC_DMAW_disable();
    crypto_rsem1_disable();
}

// first test if they could cooperate well.
void test_SEM1_to_DMAW_ShouldWork(void)
{
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}

    rsem1_to_dmaw(out_buff, in_data, MAX_BYTES);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, MAX_BYTES);
    TEST_ASSERT_EQUAL_UINT(0, NFC_REG[NFC_INTR_SRC]);

    // Transfer again to see if everything's clean.
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}
    rsem1_to_dmaw(out_buff, in_data, 64);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 64);
    TEST_ASSERT_EQUAL_UINT(0x55AA55AA, out_buff[16]);
    TEST_ASSERT_EQUAL_UINT(0, NFC_REG[NFC_INTR_SRC]);
}

int sem1_xfer(unsigned int *i_buff, int cnt)
{
    static struct pt pt1;
    int counter = 10;

    // Transfer again to see if everything's clean.
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0xFFFFFFFF;
        out_buff[i] = 0x55AA55AA;
	}
    do {} while (is_NFC_DMAW_in_use());
    NFC_DMAW_request(cnt);

    // Transfer again to see if everything's clean.
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0xFFFFFFFF;
        out_buff[i] = 0x55AA55AA;
	}
    do {} while (is_rsem1_in_use());
    crypto_rsem1_request(&dx.rsem1, i_buff, cnt, 0x8d);

    PT_INIT(&pt1);

    while (dx.rsem1.length && (counter--)) {
    	rsem1_nfc(&pt1);
    };

    NFC_DMAW_disable();
    crypto_rsem1_disable();

    return (dx.rsem1.length);
}

void test_SEM1_ShouldBlock(void)
{
    // it won't block, but NFC's DMAW would be filled full.
    TEST_ASSERT_EQUAL_UINT(0, sem1_xfer(in_data, 512));

    // so it blocks.
    TEST_ASSERT_EQUAL_UINT(896, sem1_xfer(in_data, 1024));

    // reset RSEM1 FIFOs
    crypto_rsem1_init();
}

/* A general routine for DMAR */
PT_THREAD(NFC_DMAR_cpu(struct pt *pt))
{
    PT_BEGIN(pt);

    while (dmardma.length)
    {
        PT_WAIT_UNTIL(pt, get_slot(NFC_REG[NFC_DMARFF_SLOT]));
        NFC_DMARFF[0] = *(dmardma.data++);  // lower DWORD
        NFC_DMARFF[0] = *(dmardma.data++);  // upper DWORD
        dmardma.length--;
    }

    PT_WAIT_WHILE(pt, 1);

    PT_END(pt);
}

void dmar_to_rsdm1(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
    static struct pt pt1;
    static struct pt pt2;

    do {} while (is_NFC_DMAR_in_use());

    dmardma.data = i_buff;
    dmardma.length = cnt >> 3;
    NFC_DMAR_request(cnt);

    do {} while (is_rsdm1_in_use());
    crypto_rsdm1_request(&dx.rsdm1, o_buff, cnt, 0x97);

    PT_INIT(&pt1);
    PT_INIT(&pt2);
    while (dx.rsdm1.length || !is_NFC_DMAR_done()) {
        NFC_DMAR_cpu(&pt1);
    	nfc_rsdm1(&pt2);
    };

    crypto_rsdm1_disable();
    NFC_DMAR_disable();

}

void test_dmar_to_rsdm1_ShouldWork(void)
{
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}
    dmar_to_rsdm1(out_buff, in_data, MAX_BYTES);    
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, MAX_BYTES);
    TEST_ASSERT_EQUAL_UINT(0, NFC_REG[NFC_INTR_SRC]);

    // Transfer again to see if everything's clean.
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}
    dmar_to_rsdm1(out_buff, in_data, 128);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 128);
    TEST_ASSERT_EQUAL_UINT(0x55AA55AA, out_buff[32]);
    TEST_ASSERT_EQUAL_UINT(0, NFC_REG[NFC_INTR_SRC]);

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

void NFC_loopback(unsigned int *o_buff, unsigned int *i_buff, unsigned int cnt)
{
    static struct pt pt1;
    static struct pt pt2;

    NFC_REG[NFC_H2F_ENDCNT]  = (cnt >> 3) - 1;
// FIXME: if using NFC_DMAW
//    NFC_REG[NFC_DMAW_ENDCNT] = (cnt >> 3) - 1;
// FIXME: if using NFC_DMAR
//    NFC_REG[NFC_DMAR_ENDCNT] = (cnt >> 3) - 1;

    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_H2F | NFC_ENABLE_BCHE |
                           NFC_ENABLE_LOOPBACK |
// FIXME: if using NFC_DMAW and NFC_DMAR
//                          NFC_ENABLE_DMAW | NFC_ENABLE_DMAR |
                           NFC_ENABLE_BCHD | NFC_ENABLE_F2H;

//
// FIXME: if using DMAR and DMAW, don't use all the following code.
//
    dmawdma.data   = i_buff;
    dmawdma.length = cnt >> 3;
    dmardma.data   = o_buff;
    dmardma.length = cnt >> 3;

    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (dmardma.length) {
    	DMAWFF_write(&pt1);
        DMARFF_read (&pt2);
    };

    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMAWFF_SLOT]);
    TEST_ASSERT_EQUAL(0x00000040, NFC_REG[NFC_DMARFF_SLOT]);

// disable PATH
    NFC_REG[NFC_ENABLE] = 0;
}

void test_internal_loopback_ShouldWork(void)
{
    int i;

    // initialize the test patterns
	for (i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0x12345678;
        out_buff[i] = 0xFFFFFFFF;
	}
    
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
    // Set up BCHE Main Data (T=16)
	// For sync. mode N must be DWORD aligned
    NFC_REG[NFC_KN_DATA] = 0x010c00ff;  // RG_KN_DATA (N=269, K=256 WORD)
    NFC_REG[NFC_TS_DATA] = 0x13150010;  // RG_TS_DATA (8192-4304)

    // Set up BCHE Aux Data (T=12)
    NFC_REG[NFC_KN_AUX]  = 0x000f0005;  // (N=16, K=6 WORD)
    NFC_REG[NFC_TS_AUX]  = 0x1141000c;  // (8192-256)

    NFC_loopback(out_buff, in_data, 1024);
//    dump_out_buff(0, 64);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 1024);
}

void rsem1_to_rsdm1(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
    static struct pt pt1;
    static struct pt pt2;

    do {} while (is_rsem1_in_use());
    crypto_rsem1_request(&dx.rsem1, i_buff, cnt, 0x8d);

    do {} while (is_rsdm1_in_use());
    crypto_rsdm1_request(&dx.rsdm1, o_buff, cnt, 0x97);

// FIXME: if using NFC_DMAW
    NFC_REG[NFC_DMAW_ENDCNT] = (cnt >> 3) - 1;
    NFC_REG[NFC_H2F_ENDCNT]  = (cnt >> 3) - 1;
// FIXME: if using NFC_DMAR
    NFC_REG[NFC_DMAR_ENDCNT] = (cnt >> 3) - 1;

    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAW | NFC_ENABLE_DMAR |
                           NFC_ENABLE_H2F | NFC_ENABLE_BCHE |
                           NFC_ENABLE_LOOPBACK |
                           NFC_ENABLE_BCHD | NFC_ENABLE_F2H;

    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (dx.rsdm1.length) {
    	rsem1_nfc(&pt1);
// FIXME: cooperation or send all before receive
//    } while (dmawdma.length);
//    do {
    	nfc_rsdm1(&pt2);
//        DBGPRINTF("NFC_DMAWFF_SLOT: %x, %x\n", NFC_REG[NFC_DMAWFF_SLOT], dx.rsdm1.length);
    };

    crypto_rsdm1_disable();
    crypto_rsem1_disable();

// disable PATH
//    NFC_REG[NFC_ENABLE] &= ~(NFC_ENABLE_H2F | NFC_ENABLE_DMAW | NFC_ENABLE_DMAR);
    NFC_REG[NFC_ENABLE] = 0;
}

void test_external_loopback_ShouldWork(void)
{
    int i;

    // initialize the test patterns
	for (i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0x12345678;
        out_buff[i] = 0xFFFFFFFF;
	}
    rsem1_to_rsdm1(out_buff, in_data, 512);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);

    // initialize the test patterns
	for (i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0x12345678;
        out_buff[i] = 0xFFFFFFFF;
	}
    rsem1_to_rsdm1(out_buff, in_data, 1024);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 1024); 

    // initialize the test patterns
	for (i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = 0x12345678;
        out_buff[i] = 0xFFFFFFFF;
	}
    rsem1_to_rsdm1(out_buff, in_data, 4096);
    TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 4096); 
}






