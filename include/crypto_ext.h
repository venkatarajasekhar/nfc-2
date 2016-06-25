/**
 * @file crypto_ext.h
 *
 * @author kenichi@jetsec.com.cn
 */

#ifndef CRYPTO_EXT_H_
#define CRYPTO_EXT_H_


#include "pt/pt.h"
#include "crypto.h"



struct device_extension_s
{
	unsigned int *		tmp;
	struct cryptodma_t	dmar;
	struct cryptodma_t 	rsem1;
	struct cryptodma_t 	rsdm1;
	struct cryptodma_t 	rsem2;
	struct cryptodma_t 	rsdm2;
	struct cryptodma_t 	rhm;

};

extern PT_THREAD(usb_rsem1(struct pt *pt));
extern PT_THREAD(rsdm1_usb(struct pt *pt));
extern PT_THREAD(rsem1_nfc(struct pt *pt));
extern PT_THREAD(nfc_rsdm1(struct pt *pt));
extern PT_THREAD(usb_rsem1_nfc(struct pt *pt));
extern PT_THREAD(nfc_rsdm1_usb(struct pt *pt));



#endif /*CRYPTO_EXT_H_*/
