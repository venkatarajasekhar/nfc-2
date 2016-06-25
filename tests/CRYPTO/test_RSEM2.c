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


void rsem2_cooperation(void) 
{
	static struct pt pt;

	PT_INIT(&pt);
	
	while (dx.rsem2.length)
	{
		rsem2_cpu(&pt);
	} 

	crypto_rsem2_release(&dx.rsem2);
	crypto_rsem2_disable();
}


void test_rsem2_init_ShouldWork(void)
{
	crypto_rsem2_init();
	
	do {} while (is_rsem2_in_use());
	crypto_rsem2_request(&dx.rsem2, 0, 0, 0);
	
	TEST_ASSERT_EQUAL_UINT(1, CRYPTO_REG[RSEM2_CTRL]);
	TEST_ASSERT_EQUAL_UINT(0x10, CRYPTO_REG[RSEM2_STS]);
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
}

PT_THREAD(rsem2_xfer(struct pt *pt))
{
	PT_BEGIN(pt);
	
	while (dx.rsem2.length)
	{
		PT_WAIT_UNTIL(pt, is_rsem2_infifonofull());

		CRYPTO_REG[RSEM2_CPUDIN0] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDIN1] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDIN2] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDIN3] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDSIZE] = (dx.rsem2.length >= 16) ? 16 : dx.rsem2.length;

		dx.rsem2.length = (dx.rsem2.length >= 16) ? (dx.rsem2.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);

}

int rsem2_cpu_block_xfer( unsigned int *i_buff, int cnt)
{
	static struct pt pt;
	int count = 10;
	
	dx.rsem2.data  = 0;
	dx.rsem2.length  = 0;

	do {} while (is_rsem2_in_use());
	
	crypto_rsem2_request(&dx.rsem2, i_buff, cnt, CPU_RSEM2);
		
	PT_INIT(&pt);
	do {
	
		rsem2_xfer(&pt);
	} while (dx.rsem2.length && (count--));

	crypto_rsem2_disable();	
	
	return dx.rsem2.length;
}

void test_rsem2_block_ShouldWork(void)
{
	crypto_rsem2_init();
	
	TEST_ASSERT_EQUAL_UINT(0, rsem2_cpu_block_xfer(in_data, 96));
	TEST_ASSERT_EQUAL_UINT(976, rsem2_cpu_block_xfer(in_data, 1024));
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(RSEM2_STS_OUTFIFONOEMPTY, is_rsem2_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
	
	crypto_rsem2_init();
}

void rsem2_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsem2.data  = 0;
	dx.rsem2.length  = 0;
	dx.tmp = o_buff;

	do {} while (is_rsem2_in_use());
	crypto_rsem2_request(&dx.rsem2, i_buff, cnt, CPU_RSEM2);
		
	rsem2_cooperation();	
}

void test_rsem2_cpu_ShouldWork(void)
{
	int count = 100;
	
	while (count --)
	{
		for (int i = 0; i < (MAX_BYTES >> 2); i++)
		{
			in_data[i]  = i;
			out_buff[i] = 0x55AA55AA;
		}
		
		rsem2_cpu_xfer(out_buff, in_data, MAX_BYTES);	
		
		TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, MAX_BYTES);	
		TEST_ASSERT_EQUAL_UINT(RSEM2_STS_INFIFONOFULL, is_rsem2_infifonofull());
		TEST_ASSERT_EQUAL_UINT(0, is_rsem2_outfifonoempty());	
		TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
	}
	
	for (int i = 0; i < (MAX_BYTES >> 2); i++)
	{
		in_data[i]  = i;
        out_buff[i] = 0x55AA55AA;
	}
	
	rsem2_cpu_xfer(out_buff, in_data, 512);	
	
	TEST_ASSERT_EQUAL_MEMORY(in_data, out_buff, 512);
	TEST_ASSERT_EQUAL_UINT(RSEM2_STS_INFIFONOFULL, is_rsem2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
}

static unsigned int sample_key[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int original_text[4]={0x01234567,0x89abcdef,0xfedcba98,0x76543210};
static unsigned int encrypt_text[4]={0x681edf34,0xd206965e,0x86b3e94f,0x536e4246};

void rsem2_encrypt_cpu_xfer(unsigned int *o_buff, unsigned int *i_buff, int cnt)
{
	dx.rsem2.data  = 0;
	dx.rsem2.length  = 0;
	dx.tmp = o_buff;

	do {} while (is_rsem2_in_use());
	crypto_rsem2_request(&dx.rsem2, i_buff, cnt, CPU_ENCRYPT_RSEM2);
	
	rsem2_cooperation();	
}

void test_rsem2_encrypt_xfer_cpu_ShouldWork(void)
{
	dx.rsem2.data  = 0;
	dx.rsem2.length  = 0;
	
	crypto_rsem2_init();
	do {} while (is_rsem2_in_use());
	crypto_rsem2_request(&dx.rsem2, 0, 0, 0);
	
	if (!rsem2_loadkey(sample_key))
	{
		TEST_ASSERT_EQUAL_UINT(0, 1);	// generate key fail
	}
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = original_text[i];
		out_buff[i]= encrypt_text[i];
	}
	
	rsem2_encrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(encrypt_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSEM2_STS_INFIFONOFULL, is_rsem2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = original_text[i];
		out_buff[i]= encrypt_text[i];
	}
	
	rsem2_encrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(encrypt_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSEM2_STS_INFIFONOFULL, is_rsem2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
	
	for (int i = 0; i < 4; i ++)
	{
		in_data[i] = original_text[i];
		out_buff[i]= encrypt_text[i];
	}
		
	rsem2_encrypt_cpu_xfer(out_buff, in_data, 16);
	
	TEST_ASSERT_EQUAL_MEMORY(encrypt_text, out_buff, 16);
	TEST_ASSERT_EQUAL_UINT(RSEM2_STS_INFIFONOFULL, is_rsem2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_outfifonoempty());	
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
}


PT_THREAD(rsem2_sch_cpu(struct pt *pt))
{
	PT_BEGIN(pt);
	
	while (dx.rsem2.length)
	{
		PT_WAIT_UNTIL(pt, is_rsem2_infifonofull());

		CRYPTO_REG[RSEM2_CPUDIN0] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDIN1] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDIN2] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDIN3] = *(dx.rsem2.data ++);
		CRYPTO_REG[RSEM2_CPUDSIZE] = (dx.rsem2.length >= 16) ? 16 : dx.rsem2.length;

		dx.rsem2.length = (dx.rsem2.length >= 16) ? (dx.rsem2.length - 16) : 0;
	}
	
	do {}while (!is_rsem2_hashrsltvld());

	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT0];
	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT1];
	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT2];
	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT3];
	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT4];
	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT5];
	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT6];
	*(dx.tmp++) = CRYPTO_REG[RSEM2_HASHDOUT7];
	
	PT_WAIT_WHILE(pt, 1);
	
	PT_END(pt);
}

void rsem2_sch_cooperation(void) 
{
	static struct pt pt;

	PT_INIT(&pt);

	while (dx.rsem2.length) 
	{
		rsem2_sch_cpu(&pt);
	}

	crypto_rsem2_disable();
}

 void crypto_rsem2_sch_request(struct cryptodma_t *dma, unsigned int *data, unsigned int length, unsigned int mode)
{
    dma->data  = data;
    dma->length  = length;
    CRYPTO_REG[RSEM2_CTRL] = mode;
    crypto_wait_reset();
}

void test_rsem2_sch_xfer_cpu_ShouldWork(void)
{
	
	dx.rsem2.data = 0;
	dx.rsem2.length = 0;
	dx.tmp = out_buff;
		
	for (int i = 0; i < (64/4); i++)
	{
		in_data[i] = 0x61626364;
	}
	
	CRYPTO_REG[RSEM2_CPUDIN2] = 0;
	CRYPTO_REG[RSEM2_CPUDIN3] = (64 << 3);
	crypto_rsem2_sch_request(&dx.rsem2, in_data, 64, CPU_SCH_RSEM2);
	rsem2_sch_cooperation();
	
	TEST_ASSERT_EQUAL_UINT(RSEM2_STS_INFIFONOFULL, is_rsem2_infifonofull());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_outfifonoempty());
	TEST_ASSERT_EQUAL_UINT(0, is_rsem2_in_use());
	
	TEST_ASSERT_EQUAL_HEX32(0xdebe9ff9, out_buff[0]);
	TEST_ASSERT_EQUAL_HEX32(0x2275b8a1, out_buff[1]);
	TEST_ASSERT_EQUAL_HEX32(0x38604889, out_buff[2]);
	TEST_ASSERT_EQUAL_HEX32(0xc18e5a4d, out_buff[3]);
	TEST_ASSERT_EQUAL_HEX32(0x6fdb70e5, out_buff[4]);
	TEST_ASSERT_EQUAL_HEX32(0x387e5765, out_buff[5]);
	TEST_ASSERT_EQUAL_HEX32(0x293dcba3, out_buff[6]);
	TEST_ASSERT_EQUAL_HEX32(0x9c0c5732, out_buff[7]);
	
	
}


