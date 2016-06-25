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



void rsem1_cooperation(void) 
{
	static struct pt pt;

	PT_INIT(&pt);
	
	while (dx.rsem1.length)
	{
		rsem1_cpu(&pt);
	} 

	crypto_rsem1_release(&dx.rsem1);
	crypto_rsem1_disable();
}


void test_rsem1_init_ShouldWork(void)
{
	crypto_rsem1_init();
	
	do {} while (is_rsem1_in_use());
	crypto_rsem1_request(&dx.rsem1, 0, 0, 0);
	
	TEST_ASSERT_EQUAL_UINT(1, CRYPTO_REG[RSEM1_CTRL]);
	TEST_ASSERT_EQUAL_UINT(0x10, CRYPTO_REG[RSEM1_STS]);
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_in_use());
}

PT_THREAD(rsem1_xfer(struct pt *pt))
{
	PT_BEGIN(pt);
	
	while (dx.rsem1.length)
	{
		PT_WAIT_UNTIL(pt, is_rsem1_infifonofull());

		CRYPTO_REG[RSEM1_CPUDIN0] = *(dx.rsem1.data ++);
		CRYPTO_REG[RSEM1_CPUDIN1] = *(dx.rsem1.data ++);
		CRYPTO_REG[RSEM1_CPUDIN2] = *(dx.rsem1.data ++);
		CRYPTO_REG[RSEM1_CPUDIN3] = *(dx.rsem1.data ++);
		CRYPTO_REG[RSEM1_CPUDSIZE] = (dx.rsem1.length >= 16) ? 16 : dx.rsem1.length;

		dx.rsem1.length = (dx.rsem1.length >= 16) ? (dx.rsem1.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);

}

int rsem1_cpu_block_xfer( unsigned int *i_buff, int cnt)
{
	static struct pt pt;
	int count = 10;
	
	dx.rsem1.data  = 0;
	dx.rsem1.length  = 0;

	do {} while (is_rsem1_in_use());
	
	crypto_rsem1_request(&dx.rsem1, i_buff, cnt, CPU_RSEM1);
		
	PT_INIT(&pt);
	do {
	
		rsem1_xfer(&pt);
	} while (dx.rsem1.length && (count--));

	crypto_rsem1_disable();	
	
	return dx.rsem1.length;
}

void test_rsem1_block_ShouldWork(void)
{
	crypto_rsem1_init();
	
	TEST_ASSERT_EQUAL_UINT(0, rsem1_cpu_block_xfer(in_data, 96));
	TEST_ASSERT_EQUAL_UINT(1008, rsem1_cpu_block_xfer(in_data, 1024));
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(RSEM1_STS_OUTFIFONOEMPTY, is_rsem1_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_in_use());
	
	crypto_rsem1_init();
}

void rsem1_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsem1.data  = 0;
	dx.rsem1.length  = 0;
	dx.tmp = o_buff;

	do {} while (is_rsem1_in_use());
	crypto_rsem1_request(&dx.rsem1, i_buff, cnt, CPU_RSEM1);
		
	rsem1_cooperation();	
}

void test_rsem1_cpu_ShouldWork(void)
{
	int count = 100;
	
	while (count --)
	{
		for (int i = 0; i < (MAX_BYTES >> 2); i++)
		{
			in_data[i]  = i;
			out_buff[i] = 0x55AA55AA;
		}
		
		rsem1_cpu_xfer(out_buff, in_data, MAX_BYTES);	
		
		TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, MAX_BYTES);	
		TEST_ASSERT_EQUAL_UINT(RSEM1_STS_INFIFONOFULL, is_rsem1_infifonofull());
		TEST_ASSERT_EQUAL_UINT(0, is_rsem1_outfifonoempty());
		TEST_ASSERT_EQUAL_UINT(0, is_rsem1_in_use());
	}
	
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}
	
	rsem1_cpu_xfer(out_buff, in_data, 512);	
	
	TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);
	TEST_ASSERT_EQUAL_UINT(RSEM1_STS_INFIFONOFULL, is_rsem1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_in_use());
}

static unsigned int sample_key[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int original_text[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int encrypt_text[4]={0x681edf34,0xd206965e,0x86b3e94f,0x536e4246};

void rsem1_encrypt_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsem1.data  = 0;
	dx.rsem1.length  = 0;
	dx.tmp = o_buff;

	do {} while (is_rsem1_in_use());
	crypto_rsem1_request(&dx.rsem1, i_buff, cnt, CPU_ENCRYPT_RSEM1);
	
	rsem1_cooperation();	
}

void test_rsem1_encrypt_xfer_cpu_ShouldWork(void)
{
	dx.rsem1.data  = 0;
	dx.rsem1.length  = 0;
	
	crypto_rsem1_init();
	do {} while (is_rsem1_in_use());
	crypto_rsem1_request(&dx.rsem1, 0, 0, 0);
	
	if (!rsem1_loadkey(sample_key))
	{
		TEST_ASSERT_EQUAL_UINT(0, 1);	// generate key fail
	}
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = original_text[i];
		out_buff[i]= encrypt_text[i];
	}
	
	rsem1_encrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(encrypt_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSEM1_STS_INFIFONOFULL, is_rsem1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_in_use());
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = original_text[i];
		out_buff[i]= encrypt_text[i];
	}
	
	rsem1_encrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(encrypt_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSEM1_STS_INFIFONOFULL, is_rsem1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_in_use());
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = original_text[i];
		out_buff[i]= encrypt_text[i];
	}
		
	rsem1_encrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(encrypt_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSEM1_STS_INFIFONOFULL, is_rsem1_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem1_in_use());
}







