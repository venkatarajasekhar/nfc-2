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

void rsdm2_cooperation(void) 
{
	static struct pt pt;

	PT_INIT(&pt);
	
	while (dx.rsdm2.length)
	{
		rsdm2_cpu(&pt);
	}

	crypto_rsdm2_release(&dx.rsdm2);
	crypto_rsdm2_disable();
}

void test_rsdm2_init_ShouldWork(void)
{
	crypto_rsdm2_init();
	
	do {} while (is_rsdm2_in_use());
	crypto_rsdm2_request(&dx.rsdm2, 0, 0, 0);
	
	TEST_ASSERT_EQUAL_UINT(1, CRYPTO_REG[RSDM2_CTRL]);
	TEST_ASSERT_EQUAL_UINT(0x10, CRYPTO_REG[RSDM2_STS]);	// status should be zero after reset
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_in_use());
}


PT_THREAD(rsdm2_xfer(struct pt *pt))
{
	PT_BEGIN(pt);
	while (dx.rsdm2.length)
	{
		PT_WAIT_UNTIL(pt, is_rsdm2_infifonofull());

		CRYPTO_REG[RSDM2_CPUDIN0] = *(dx.rsdm2.data ++);
		CRYPTO_REG[RSDM2_CPUDIN1] = *(dx.rsdm2.data ++);
		CRYPTO_REG[RSDM2_CPUDIN2] = *(dx.rsdm2.data ++);
		CRYPTO_REG[RSDM2_CPUDIN3] = *(dx.rsdm2.data ++);
		CRYPTO_REG[RSDM2_CPUDSIZE] = (dx.rsdm2.length >= 16) ? 16 : dx.rsdm2.length;

		dx.rsdm2.length = (dx.rsdm2.length >= 16) ? (dx.rsdm2.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);

}

int rsdm2_cpu_block_xfer( unsigned int *i_buff, int cnt)
{
	static struct pt pt;
	int count = 10;
	
	dx.rsdm2.data  = 0;
	dx.rsdm2.length  = 0;

	do {} while (is_rsdm2_in_use());
	
	crypto_rsdm2_request(&dx.rsdm2, i_buff, cnt, CPU_RSDM2);
		
	PT_INIT(&pt);
	do {
	
		rsdm2_xfer(&pt);
	} while (dx.rsdm2.length && (count--));

	crypto_rsdm2_disable();	
	
	return dx.rsdm2.length;
}

void test_rsdm2_block_ShouldWork(void)
{
	crypto_rsdm2_init();
	
	TEST_ASSERT_EQUAL_UINT(0, rsdm2_cpu_block_xfer(in_data, 96));
	TEST_ASSERT_EQUAL_UINT(976, rsdm2_cpu_block_xfer(in_data, 1024));
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(RSDM2_STS_OUTFIFONOEMPTY, is_rsdm2_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_in_use());
	 
	crypto_rsdm2_init();
	
}

void rsdm2_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsdm2.data  = 0;
	dx.rsdm2.length  = 0;
	dx.tmp = o_buff;
	do {} while (is_rsdm2_in_use());
	crypto_rsdm2_request(&dx.rsdm2, i_buff, cnt, CPU_RSDM2);
		
	rsdm2_cooperation();	
}

void test_rsdm2_cpu_ShouldWork(void)
{
	int count = 100;
	
	while (count --)
	{
		for (int i = 0; i < (MAX_BYTES >> 2); i++)
		{
			in_data[i]  = i;
			out_buff[i] = 0x55AA55AA;
		}
		
		rsdm2_cpu_xfer(out_buff, in_data, MAX_BYTES);	
		
		TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, MAX_BYTES);	
		TEST_ASSERT_EQUAL_UINT(RSDM2_STS_INFIFONOFULL, is_rsdm2_infifonofull());
		TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_outfifonoempty());	
		TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_in_use());
	}
	
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}
	
	rsdm2_cpu_xfer(out_buff, in_data, 512);	
	
	TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);
	TEST_ASSERT_EQUAL_UINT(RSDM2_STS_INFIFONOFULL, is_rsdm2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_in_use());
}

static unsigned int sample_key[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int original_text[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int encrypt_text[4]={0x681edf34,0xd206965e,0x86b3e94f,0x536e4246};

void rsdm2_decrypt_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsdm2.data  = 0;
	dx.rsdm2.length  = 0;
	dx.tmp = o_buff;

	do {} while (is_rsdm2_in_use());
	crypto_rsdm2_request(&dx.rsdm2, i_buff, cnt, CPU_DECRYPT_RSDM2);
	
	rsdm2_cooperation();	
}

void test_rsdm2_decrypt_xfer_cpu_ShouldWork(void)
{
	dx.rsdm2.data  = 0;
	dx.rsdm2.length  = 0;
	
	crypto_rsdm2_init();
	do {} while (is_rsdm2_in_use());
	crypto_rsdm2_request(&dx.rsdm2, 0, 0, 0);
	
	if (!rsdm2_loadkey(sample_key))
	{
		TEST_ASSERT_EQUAL_UINT(0, 1);	// generate key fail
	}
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = encrypt_text[i];
		out_buff[i]= original_text[i];
	}
	
	rsdm2_decrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(original_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSDM2_STS_INFIFONOFULL, is_rsdm2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_in_use());
		
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = encrypt_text[i];
		out_buff[i]= original_text[i];
	}
		
	rsdm2_decrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(original_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSDM2_STS_INFIFONOFULL, is_rsdm2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_in_use());
		
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = encrypt_text[i];
		out_buff[i]= original_text[i];
	}
			
	rsdm2_decrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(original_text, out_buff, 16);	
	TEST_ASSERT_EQUAL_UINT(RSDM2_STS_INFIFONOFULL, is_rsdm2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm2_in_use());
}


