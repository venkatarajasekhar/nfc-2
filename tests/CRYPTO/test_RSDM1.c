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


void rsdm1_cooperation(void) 
{
	static struct pt pt;

	PT_INIT(&pt);
	
	while (dx.rsdm1.length)
	{
		rsdm1_cpu(&pt);
	}

	crypto_rsdm1_release(&dx.rsdm1);
	crypto_rsdm1_disable();
}



void test_rsdm1_init_ShouldWork(void)
{
	CRYPTO_REG[RSDM1_CTRL]	= RSDM1_CTRL_SOFTRST;
	crypto_wait_reset();
	
	do {} while (is_rsdm1_in_use());
	crypto_rsdm1_request(&dx.rsdm1, 0, 0, 0);
	
	TEST_ASSERT_EQUAL_UINT(1, CRYPTO_REG[RSDM1_CTRL]);
	TEST_ASSERT_EQUAL_UINT(0x10, CRYPTO_REG[RSDM1_STS]);
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_in_use());
}

PT_THREAD(rsdm1_xfer(struct pt *pt))
{
	PT_BEGIN(pt);
	while (dx.rsdm1.length)
	{
		PT_WAIT_UNTIL(pt, is_rsdm1_infifonofull());

		CRYPTO_REG[RSDM1_CPUDIN0] = *(dx.rsdm1.data ++);
		CRYPTO_REG[RSDM1_CPUDIN1] = *(dx.rsdm1.data ++);
		CRYPTO_REG[RSDM1_CPUDIN2] = *(dx.rsdm1.data ++);
		CRYPTO_REG[RSDM1_CPUDIN3] = *(dx.rsdm1.data ++);
		CRYPTO_REG[RSDM1_CPUDSIZE] = (dx.rsdm1.length >= 16) ? 16 : dx.rsdm1.length;

		dx.rsdm1.length = (dx.rsdm1.length >= 16) ? (dx.rsdm1.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);

}

int rsdm1_cpu_block_xfer( unsigned int *i_buff, int cnt)
{
	static struct pt pt;
	int count = 10;
	
	dx.rsdm1.data  = 0;
	dx.rsdm1.length  = 0;

	do {} while (is_rsdm1_in_use());
	
	crypto_rsdm1_request(&dx.rsdm1, i_buff, cnt, CPU_RSDM1);
		
	PT_INIT(&pt);
	do {
	
		rsdm1_xfer(&pt);
	} while (dx.rsdm1.length && (count--));

	crypto_rsdm1_disable();	
	
	return dx.rsdm1.length;
}

void test_rsdm1_block_ShouldWork(void)
{
	crypto_rsdm1_init();
	
	TEST_ASSERT_EQUAL_UINT(0, rsdm1_cpu_block_xfer(in_data, 96));
	TEST_ASSERT_EQUAL_UINT(1024, rsdm1_cpu_block_xfer(in_data, 1024));
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(RSDM1_STS_OUTFIFONOEMPTY, is_rsdm1_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_in_use());
	
	crypto_rsdm1_init();
	
}

void rsdm1_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsdm1.data  = 0;
	dx.rsdm1.length  = 0;
	dx.tmp = o_buff;
	do {} while (is_rsdm1_in_use());
	crypto_rsdm1_request(&dx.rsdm1, i_buff, cnt, CPU_RSDM1);
		
	rsdm1_cooperation();	
}

void test_rsdm1_cpu_ShouldWork(void)
{
	int count = 100;
	
	while (count --)
	{
		for (int i = 0; i < (MAX_BYTES >> 2); i++)
		{
			in_data[i]  = i;
			out_buff[i] = 0x55AA55AA;
		}
		
		rsdm1_cpu_xfer(out_buff, in_data, MAX_BYTES);	
		
		TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, MAX_BYTES);	
		TEST_ASSERT_EQUAL_UINT(RSDM1_STS_INFIFONOFULL, is_rsdm1_infifonofull());
		TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_outfifonoempty());	
		TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_in_use());
	}
	
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}
	
	rsdm1_cpu_xfer(out_buff, in_data, 512);	
	
	TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);
	TEST_ASSERT_EQUAL_UINT(RSDM1_STS_INFIFONOFULL, is_rsdm1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_in_use());
}

static unsigned int sample_key[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int original_text[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int encrypt_text[4]={0x681edf34,0xd206965e,0x86b3e94f,0x536e4246};

void rsdm1_decrypt_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsdm1.data  = 0;
	dx.rsdm1.length  = 0;
	dx.tmp = o_buff;

	do {} while (is_rsdm1_in_use());
	crypto_rsdm1_request(&dx.rsdm1, i_buff, cnt, CPU_DECRYPT_RSDM1);
	
	rsdm1_cooperation();	
}

void test_rsdm1_decrypt_xfer_cpu_ShouldWork(void)
{
	dx.rsdm1.data  = 0;
	dx.rsdm1.length  = 0;
	
	crypto_rsdm1_init();
	do {} while (is_rsdm1_in_use());
	crypto_rsdm1_request(&dx.rsdm1, 0, 0, 0);
	
	if (!rsdm1_loadkey(sample_key))
	{
		TEST_ASSERT_EQUAL_UINT(0, 1);	// generate key fail
	}
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = encrypt_text[i];
		out_buff[i]= original_text[i];
	}
	
	rsdm1_decrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(original_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSDM1_STS_INFIFONOFULL, is_rsdm1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_in_use());
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = encrypt_text[i];
		out_buff[i]= original_text[i];
	}
		
	rsdm1_decrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(original_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSDM1_STS_INFIFONOFULL, is_rsdm1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_in_use());
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = encrypt_text[i];
		out_buff[i]= original_text[i];
	}
			
	rsdm1_decrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(original_text, out_buff, 16);	
	TEST_ASSERT_EQUAL_UINT(RSDM1_STS_INFIFONOFULL, is_rsdm1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsdm1_in_use());
}













