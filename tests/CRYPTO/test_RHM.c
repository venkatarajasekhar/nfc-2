/**
 * @file test_NFC.c
 *
 * This testsuit tests if NFC's DMA serialization works.
 *
 * @author fox.chao@gmail.com
 */

#include "NFC.h"
#include "crypto.h"
#include "crypto_ext.h"

#include "unity.h"


void setUp(void)
{
}

void tearDown(void)
{
}

#define MAX_BYTES   4096

static unsigned int in_data [MAX_BYTES >> 2];
static unsigned int out_buff[MAX_BYTES >> 2];

extern struct device_extension_s dx;



void rhm_cooperation(void) 
{
	static struct pt pt;

	PT_INIT(&pt);

	while (dx.rhm.length) 
	{
		rhm_cpu(&pt);
	}

	crypto_rhm_disable();
}


void test_rhm_init_ShouldWork(void)
{
	crypto_rhm_init();
	
	do {} while (is_rhm_in_use());
	crypto_rhm_request(&dx.rhm, 0, 0, 0);
	
	TEST_ASSERT_EQUAL_UINT(1, CRYPTO_REG[RHM_CTRL]);
	TEST_ASSERT_EQUAL_UINT(0, CRYPTO_REG[RHM_STS]);	// status should be zero after reset
	TEST_ASSERT_EQUAL_UINT(0, is_rhm_in_use());
}


void test_rhm_cpu_ShouldWork(void)
{
	dx.rhm.data = 0;
	dx.rhm.length = 0;
	dx.tmp = out_buff;
	
	for (int i = 0; i < (64/4); i++)
	{
		in_data[i] = 0x61626364;
	}
	
	CRYPTO_REG[RHM_CPUDIN2] = 0;
	CRYPTO_REG[RHM_CPUDIN3] = (64 << 3);
	crypto_rhm_request(&dx.rhm, in_data, 64, CPU_RHM);
	rhm_cooperation();
	
	TEST_ASSERT_EQUAL_UINT(0, is_rhm_in_use());
	
	TEST_ASSERT_EQUAL_HEX32(0xdebe9ff9, out_buff[0]);
	TEST_ASSERT_EQUAL_HEX32(0x2275b8a1, out_buff[1]);
	TEST_ASSERT_EQUAL_HEX32(0x38604889, out_buff[2]);
	TEST_ASSERT_EQUAL_HEX32(0xc18e5a4d, out_buff[3]);
	TEST_ASSERT_EQUAL_HEX32(0x6fdb70e5, out_buff[4]);
	TEST_ASSERT_EQUAL_HEX32(0x387e5765, out_buff[5]);
	TEST_ASSERT_EQUAL_HEX32(0x293dcba3, out_buff[6]);
	TEST_ASSERT_EQUAL_HEX32(0x9c0c5732, out_buff[7]);
}


