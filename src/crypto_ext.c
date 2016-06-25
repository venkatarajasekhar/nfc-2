/**
 * @file crypto_ext.c
 *
 * @author kenichi@jetsec.com.cn
 */

#include "crypto_ext.h"
#include "crypto.h"
#include "nfc.h"




struct device_extension_s dx;

/**
 *
 * usb_rsem1
 * 
 * usb and rsem1 works in dma mode
 * 
 */

PT_THREAD(usb_rsem1(struct pt *pt))
{
	static unsigned int tmpData;

	PT_BEGIN(pt);
	
	while (dx.rsem1.length)
	{
		PT_WAIT_UNTIL(pt, is_rsem1_outfifonoempty());

		CRYPTO_REG[CPUOUT_PORT] = 0x5a000001;
		tmpData = CRYPTO_REG[CPUOUT0];
		printf("%x ", tmpData);
		tmpData = CRYPTO_REG[CPUOUT1];
		printf("%x ", tmpData);
		tmpData = CRYPTO_REG[CPUOUT2];
		printf("%x ", tmpData);
		tmpData = CRYPTO_REG[CPUOUT3];
		printf("%x \n", tmpData);
		
		dx.rsem1.length = (dx.rsem1.length >= 16) ? (dx.rsem1.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);
}

/**
 *
 * rsdm1_usb
 * 
 * rsdm1 and usb works in dma mode
 * 
 */

PT_THREAD(rsdm1_usb(struct pt *pt))
{
	PT_BEGIN(pt);
	
	while (dx.rsdm1.length)
	{
		PT_WAIT_UNTIL(pt, is_rsdm1_infifonofull());

		CRYPTO_REG[RSDM1_CPUDIN0] = 0x11223344;
		CRYPTO_REG[RSDM1_CPUDIN1] = 0x55667788;
		CRYPTO_REG[RSDM1_CPUDIN2] = 0x99aabbcc;
		CRYPTO_REG[RSDM1_CPUDIN3] = 0xddeeff00;
		CRYPTO_REG[RSDM1_CPUDSIZE] = 16;
		
		dx.rsdm1.length = (dx.rsdm1.length >= 16) ? (dx.rsdm1.length - 16) : 0;
	}

	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);
}

/**
 * 
 * rsem1_nfc
 * 
 * rsem1 and nfc works in dma mode
 * 
 */

PT_THREAD(rsem1_nfc(struct pt *pt))
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

/**
 * 
 * nfc_rsdm1
 * 
 * nfc and rsdm1 works in dma mode
 * 
 */

PT_THREAD(nfc_rsdm1(struct pt *pt))
{
	PT_BEGIN(pt);

	while (dx.rsdm1.length)
	{
		PT_WAIT_UNTIL(pt, is_rsdm1_outfifonoempty());

		CRYPTO_REG[CPUOUT_PORT] = 0x5a000002;

		*(dx.rsdm1.data ++) = CRYPTO_REG[CPUOUT0];
		*(dx.rsdm1.data ++) = CRYPTO_REG[CPUOUT1];
		*(dx.rsdm1.data ++) = CRYPTO_REG[CPUOUT2];
		*(dx.rsdm1.data ++) = CRYPTO_REG[CPUOUT3];

		dx.rsdm1.length = (dx.rsdm1.length >= 16) ? (dx.rsdm1.length - 16) : 0;

	}
	
	PT_WAIT_WHILE(pt, 1);

	PT_END(pt);
}

/**
 * 
 * usb_rsem1_nfc
 * 
 * usb and rsem1 and nfc  works in dma mode
 * 
 */

PT_THREAD(usb_rsem1_nfc(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_WHILE(pt, 1);
	
	PT_END(pt);
}

/**
 * 
 * usb_rsem1_nfc
 * 
 * nfc and rsdm1 and usb  works in dma mode
 * 
 */

PT_THREAD(nfc_rsdm1_usb(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_WHILE(pt, 1);
	
	PT_END(pt);
}


