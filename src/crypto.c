/**
 * @file crypto.c
 *
 * @author kenichi@jetsec.com.cn
 */


#include "crypto.h"
#include "crypto_ext.h"
#include "opt/printf.h"
#include "nfc.h"

extern struct device_extension_s dx;


/**
 * 
 * rsem1_cpu
 * 
 * rsem1 works in cpu mode
 * 
 */
PT_THREAD(rsem1_cpu(struct pt *pt))
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

		PT_WAIT_UNTIL(pt, is_rsem1_outfifonoempty());

		CRYPTO_REG[CPUOUT_PORT] = 0x5a000001;
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT0];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT1];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT2];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT3];

		dx.rsem1.length = (dx.rsem1.length >= 16) ? (dx.rsem1.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);

}

/**
 * 
 * rsdm1_cpu
 * 
 * rsdm1 works in cpu mode
 * 
 */

PT_THREAD(rsdm1_cpu(struct pt *pt))
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

		PT_WAIT_UNTIL(pt, is_rsdm1_outfifonoempty());

		CRYPTO_REG[CPUOUT_PORT] = 0x5a000002;
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT0];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT1];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT2];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT3];

		dx.rsdm1.length = (dx.rsdm1.length >= 16) ? (dx.rsdm1.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);

}

/**
 * 
 * rsem2_cpu
 * 
 * rsem2 works in cpu mode
 * 
 */

PT_THREAD(rsem2_cpu(struct pt *pt))
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

		PT_WAIT_UNTIL(pt, is_rsem2_outfifonoempty());

		CRYPTO_REG[CPUOUT_PORT] = 0x5a000003;
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT0];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT1];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT2];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT3];

		dx.rsem2.length = (dx.rsem2.length >= 16) ? (dx.rsem2.length - 16) : 0;
	}
	
	PT_WAIT_WHILE(pt, 1);
	
	PT_END(pt);
}

/**
 * 
 * rsdm2_cpu
 * 
 * rsdm2 works in cpu mode
 * 
 */

PT_THREAD(rsdm2_cpu(struct pt *pt))
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

		PT_WAIT_UNTIL(pt, is_rsdm2_outfifonoempty());

		CRYPTO_REG[CPUOUT_PORT] = 0x5a000004;
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT0];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT1];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT2];
		*(dx.tmp ++) = CRYPTO_REG[CPUOUT3];

		dx.rsdm2.length = (dx.rsdm2.length >= 16) ? (dx.rsdm2.length - 16) : 0;
	}
	
	PT_WAIT_WHILE(pt, 1);
	
	PT_END(pt);
}

/**
 * 
 * rhm_cpu
 * 
 * rhm works in cpu mode
 * 
 */

PT_THREAD(rhm_cpu(struct pt *pt))
{
	PT_BEGIN(pt);
	
	while (dx.rhm.length)
	{
		PT_WAIT_UNTIL(pt, !is_rhm_in_use());

		CRYPTO_REG[RHM_CPUDIN0] = *(dx.rhm.data ++);
		CRYPTO_REG[RHM_CPUDIN1] = *(dx.rhm.data ++);
		CRYPTO_REG[RHM_CPUDIN2] = *(dx.rhm.data ++);
		CRYPTO_REG[RHM_CPUDIN3] = *(dx.rhm.data ++);
		CRYPTO_REG[RHM_CPUDSIZE] = (dx.rhm.length >= 16) ? 16 : dx.rhm.length;
		
		dx.rhm.length = (dx.rhm.length >= 16) ? (dx.rhm.length - 16) : 0;
	}

	do {}while (!is_rhm_schresultvalid());

	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT0];
	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT1];
	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT2];
	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT3];
	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT4];
	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT5];
	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT6];
	*(dx.tmp++) = CRYPTO_REG[RHM_HASHDOUT7];

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);
}

/**
 * 
 * rsem1_loadkey
 * 
 * rsem1 load encrypt key for transfer
 * 
 */

int rsem1_loadkey(unsigned int *key)
{
	int timeout = 30;
	
	CRYPTO_REG[RSEM1_CPUDIN0] = key[0];
	CRYPTO_REG[RSEM1_CPUDIN1] = key[1];
	CRYPTO_REG[RSEM1_CPUDIN2] = key[2];
	CRYPTO_REG[RSEM1_CPUDIN3] = key[3];
	
	CRYPTO_REG[RSEM1_CTRL] |= RSEM1_CTRL_GENKEYSTART;
	
	do {}while(is_rsem1_in_use() && (--timeout));
	
	if (timeout <= 0)
	{
		return 0;
	}
	
	return 1;
}

/**
 * 
 * rsdm1_loadkey
 * 
 * rsdm1 load decrypt key for transfer
 * 
 */

int rsdm1_loadkey(unsigned int *key)
{
	int timeout = 30;
	
	CRYPTO_REG[RSDM1_CPUDIN0] = key[0];
	CRYPTO_REG[RSDM1_CPUDIN1] = key[1];
	CRYPTO_REG[RSDM1_CPUDIN2] = key[2];
	CRYPTO_REG[RSDM1_CPUDIN3] = key[3];
	
	CRYPTO_REG[RSDM1_CTRL] |= RSDM1_CTRL_GENKEYSTART;
	
	do {}while(is_rsdm1_in_use() && (--timeout));
	
	if (timeout <= 0)
	{
		return 0;
	}
	
	return 1;	
}

/**
 * 
 * rsem2_loadkey
 * 
 * rsem2 load encrypt key for transfer
 * 
 */

int rsem2_loadkey(unsigned int *key)
{
	int timeout = 30;
		
	CRYPTO_REG[RSEM2_CPUDIN0] = key[0];
	CRYPTO_REG[RSEM2_CPUDIN1] = key[1];
	CRYPTO_REG[RSEM2_CPUDIN2] = key[2];
	CRYPTO_REG[RSEM2_CPUDIN3] = key[3];
		
	CRYPTO_REG[RSEM2_CTRL] |= RSEM2_CTRL_GENKEYSTART;
	
	do {}while(is_rsem2_in_use() && (--timeout));
		
	if (timeout <= 0)
	{
		return 0;
	}
		
	return 1;	
}

/**
 * 
 * rsdm2_loadkey
 * 
 * rsdm2 load decrypt key for transfer
 * 
 */

int rsdm2_loadkey(unsigned int *key)
{
	int timeout = 30;
		
	CRYPTO_REG[RSDM2_CPUDIN0] = key[0];
	CRYPTO_REG[RSDM2_CPUDIN1] = key[1];
	CRYPTO_REG[RSDM2_CPUDIN2] = key[2];
	CRYPTO_REG[RSDM2_CPUDIN3] = key[3];
		
	CRYPTO_REG[RSDM2_CTRL] |= RSDM2_CTRL_GENKEYSTART;
	
	do {}while(is_rsdm2_in_use() && (--timeout));
		
	if (timeout <= 0)
	{
		return 0;
	}
		
	return 1;	
}


