/**
 * @file crypto.h
 *
 * @author kenichi@jetsec.com.cn
 */

#ifndef __INCLUDE_CRYPTO_H
#define __INCLUDE_CRYPTO_H

#include "pt/pt.h"



/****************************************************************************
 * CRYPTO MODE Definition
 ****************************************************************************/

#define DMA_USB_RSEM1_NFC	0x9d
#define DMA_USB_RSEM1		0x95
#define DMA_RSEM1_NFC		0x8d
#define CPU_RSEM1			0x85
#define CPU_ENCRYPT_RSEM1	0x05

#define DMA_NFC_RSDM1_USB	0x9d
#define DMA_RSDM1_USB		0x8d
#define DMA_NFC_RDSM1		0x97
#define CPU_RSDM1			0x87
#define CPU_DECRYPT_RSDM1	0x07

#define CPU_RSEM2			0x105
#define CPU_ENCRYPT_RSEM2	0x005
#define CPU_SCH_RSEM2		0x1084

#define CPU_RSDM2			0x87
#define CPU_DECRYPT_RSDM2	0x07

#define CPU_RHM				0x1001



/****************************************************************************
 * Definitions
 ****************************************************************************/

#define RSEM1_CTRL 						0x00000000/4 
#define RSEM1_STS 						0x00000004/4 
#define RSEM1_CPUDIN0 					0x00000010/4 
#define RSEM1_CPUDIN1 					0x00000014/4 
#define RSEM1_CPUDIN2					0x00000018/4 
#define RSEM1_CPUDIN3 					0x0000001C/4
#define RSEM1_CPUDSIZE					0x00000020/4

#define RSEM1_CTRL_EN					0x00000001
#define RSEM1_CTRL_SMS4_MODE			0x00000006
#define RSEM1_CTRL_DIRSEL				0x00000008
#define RSEM1_CTRL_SRCSEL				0x00000010
#define RSEM1_CTRL_DMAENDINTEN			0x00000040
#define RSEM1_CTRL_CRYPTOMODE			0x00000080
#define RSEM1_CTRL_SOFTRST				0x00004000
#define RSEM1_CTRL_GENKEYSTART			0x00008000

#define RSEM1_STS_DMAEND				0x00000002
#define RSEM1_STS_BUSY					0x00000004
#define RSEM1_STS_OUTFIFONOEMPTY		0x00000008
#define RSEM1_STS_INFIFONOFULL			0x00000010
#define RSEM1_STS_DATANUMBER			0x000007c0

#define RSDM1_CTRL 						0x00000100/4 
#define RSDM1_STS						0x00000104/4 
#define RSDM1_CPUDIN0					0x00000110/4 
#define RSDM1_CPUDIN1 					0x00000114/4 
#define RSDM1_CPUDIN2 					0x00000118/4 
#define RSDM1_CPUDIN3 					0x0000011C/4 
#define RSDM1_CPUDSIZE 					0x00000120/4 

#define RSDM1_CTRL_EN					0x00000001
#define RSDM1_CTRL_SMS4_MODE			0x00000006
#define RSDM1_CTRL_DIRSEL				0x00000008
#define RSDM1_CTRL_SRCSEL				0x00000010
#define RSDM1_CTRL_DMAENDINTEN			0x00000040
#define RSDM1_CTRL_CRYPTOMODE			0x00000080
#define RSDM1_CTRL_SOFTRST				0x00004000
#define RSDM1_CTRL_GENKEYSTART			0x00008000

#define RSDM1_STS_DMAEND				0x00000002
#define RSDM1_STS_BUSY					0x00000004
#define RSDM1_STS_OUTFIFONOEMPTY		0x00000008
#define RSDM1_STS_INFIFONOFULL			0x00000010
#define RSDM1_STS_DATANUMBER			0x000007c0

#define RSEM2_CTRL 						0x00000200/4 
#define RSEM2_STS 						0x00000204/4 
#define RSEM2_CPUDIN0 					0x00000210/4 
#define RSEM2_CPUDIN1 					0x00000214/4 
#define RSEM2_CPUDIN2 					0x00000218/4 
#define RSEM2_CPUDIN3 					0x0000021C/4 
#define RSEM2_RSVR_1 					0x00000220/4
#define RSEM2_RSVR_2	 				0x00000224/4
#define RSEM2_CPUDSIZE 					0x00000228/4 
#define RSEM2_HASHDOUT0					0x00000230/4
#define RSEM2_HASHDOUT1					0x00000234/4
#define RSEM2_HASHDOUT2					0x00000238/4
#define RSEM2_HASHDOUT3					0x0000023c/4
#define RSEM2_HASHDOUT4					0x00000240/4
#define RSEM2_HASHDOUT5					0x00000244/4
#define RSEM2_HASHDOUT6					0x00000248/4
#define RSEM2_HASHDOUT7					0x0000024c/4

#define RSEM2_CTRL_EN					0x00000001
#define RSEM2_CTRL_SMS4_MODE			0x00000006
#define RSEM2_CTRL_DIRSEL				0x00000008
#define RSEM2_CTRL_SRCSEL				0x00000010
#define RSEM2_CTRL_DMAENDINTEN			0x00000040
#define RSEM2_CTRL_HASHEN				0x00000080
#define RSEM2_CTRL_CRYPTOMODE			0x00000100
#define RSEM2_CTRL_HASHLENLOAD			0x00001000
#define RSEM2_CTRL_SOFTRST				0x00004000
#define RSEM2_CTRL_GENKEYSTART			0x00008000

#define RSEM2_STS_DMAEND				0x00000002
#define RSEM2_STS_BUSY					0x00000004
#define RSEM2_STS_OUTFIFONOEMPTY		0x00000008
#define RSEM2_STS_INFIFONOFULL			0x00000010
#define RSEM2_STS_OUTEND				0x00000020
#define RSEM2_STS_DNUMOUT				0x000007c0
#define RSEM2_STS_SCHRSLTVLD			0x00000800

#define RSDM2_CTRL 						0x00000300/4 
#define RSDM2_STS 						0x00000304/4 
#define RSDM2_CPUDIN0 					0x00000310/4 
#define RSDM2_CPUDIN1 					0x00000314/4 
#define RSDM2_CPUDIN2 					0x00000318/4 
#define RSDM2_CPUDIN3 					0x0000031C/4 
#define RSDM2_CPUDSIZE 					0x00000320/4 

#define RSDM2_CTRL_EN					0x00000001
#define RSDM2_CTRL_SMS4_MODE			0x00000006
#define RSDM2_CTRL_DIRSEL				0x00000008
#define RSDM2_CTRL_SRCSEL				0x00000010
#define RSDM2_CTRL_DMAENDINTEN			0x00000040
#define RSDM2_CTRL_CRYPTOMODE			0x00000080
#define RSDM2_CTRL_SOFTRST				0x00004000
#define RSDM2_CTRL_GENKEYSTART			0x00008000

#define RSDM2_STS_DMAEND				0x00000002
#define RSDM2_STS_BUSY					0x00000004
#define RSDM2_STS_OUTFIFONOEMPTY		0x00000008
#define RSDM2_STS_INFIFONOFULL			0x00000010
#define RSDM2_STS_DATANUMBER			0x000007c0

#define RHM_CTRL						0x00000400/4 
#define RHM_STS 						0x00000404/4 
#define RHM_CPUDIN0 					0x00000410/4 
#define RHM_CPUDIN1						0x00000414/4 
#define RHM_CPUDIN2 					0x00000418/4 
#define RHM_CPUDIN3 					0x0000041c/4 
#define RHM_HASHDOUT0 					0x00000420/4 
#define RHM_HASHDOUT1 					0x00000424/4 
#define RHM_HASHDOUT2 					0x00000428/4 
#define RHM_HASHDOUT3 					0x0000042C/4 
#define RHM_HASHDOUT4 					0x00000430/4 
#define RHM_HASHDOUT5 					0x00000434/4 
#define RHM_HASHDOUT6 					0x00000438/4 
#define RHM_HASHDOUT7					0x0000043C/4 
#define RHM_CPUDSIZE 					0x00000440/4


#define RHM_CTRL_EN						0x00000001
#define RHM_CTRL_SRCSEL					0x00000002
#define RHM_CTRL_DMAENDINTEN			0x00000008
#define RHM_CTRL_HASHLENLOAD			0x00001000
#define RHM_CTRL_SOFTRST				0x00004000

#define RHM_STS_DMAEND					0x00000002
#define RHM_STS_BUSY					0x00000004
#define RHM_STS_SCHRSLTVLD				0x00000008

#define CRYPTO_INT						0x00000500/4
#define CPUOUT_PORT						0x00000504/4
#define CPUOUT0							0x00000510/4
#define CPUOUT1							0x00000514/4
#define CPUOUT2							0x00000518/4
#define CPUOUT3							0x0000051C/4

#define NFCM_CTRL 						0x00000600/4 
#define NFCM_STS						0x00000604/4 
#define NFCM_RCVLEN 					0x00000610/4 
#define NFCM_SNDLEN 					0x00000614/4 
#define NFCM_ERRORVALH					0x00000618/4 
#define NFCM_ERRORVALL					0x0000061C/4 

#define USBCM_EP3_CTRL					0x00000700/4
#define USBCM_EP3_STS					0x00000704/4
#define USBCM_EP3_SNDLEN				0x00000708/4
#define USBCM_EP4_CTRL					0x00000710/4
#define USBCM_EP4_STS					0x00000714/4
#define USBCM_EP4_RCVLEN				0x00000718/4
#define USBCM_EP5_CTRL					0x00000720/4
#define USBCM_EP5_STS					0x00000724/4
#define USBCM_EP5_SNDLEN				0x00000728/4
#define USBCM_EP6_CTRL					0x00000730/4
#define USBCM_EP6_STS					0x00000734/4
#define USBCM_EP6_RCVLEN				0x00000738/4
#define USBCM_EP7_CTRL					0x00000740/4
#define USBCM_EP7_STS					0x00000744/4
#define USBCM_EP7_SNDLEN				0x00000748/4
#define USBCM_EP8_CTRL					0x00000750/4
#define USBCM_EP8_STS					0x00000754/4
#define USBCM_EP8_RCVLEN				0x00000758/4
#define USBCM_EP9_CTRL					0x00000760/4
#define USBCM_EP9_STS					0x00000764/4
#define USBCM_EP9_SNDLEN				0x00000768/4

#define NFCM_CTRL_AVLDATAEN				0x00000001
#define NFCM_CTRL_REQDATAEN				0x00000002
#define NFCM_CTRL_TEST_MODE				0x00000004
#define NFCM_CTRL_TEST_SOFTRST			0x00000008

#define NFCM_STS_CHECK_RESULT			0x00000001

#define USBC_EP3579_CTRL_AVLDATAEN		0x00000001
#define USBC_EP3579_CTRL_SOFTRST		0x00000002
#define USBC_EP3579_STS_NOMOREDATASEND	0x00000001
#define USBC_EP3579_SNDLEN				0xffffffff

#define USBC_EP468_REQDATAEN			0x00000001
#define USBC_EP468_SOFTRST				0x00000002
#define USBC_EP468_CHECKRESULT			0x00000001
#define USBC_EP468_RCVDATALEN			0xffffffff

#define CRYPTO_INT_RSEM1_DMAENDINT		0x00000001
#define CRYPTO_INT_RSDM1_DMAENDINT		0x00000002
#define CRYPTO_INT_RSEM2_DMAENTINT		0x00000004
#define CRYPTO_INT_RSDM2_DMAENTINT		0x00000008
#define CRYPTO_INT_RHM_DMAENDINT		0x00000010

#define CRYPTOBASE 						0x20010000
#define CRYPTO_REG    					((volatile unsigned int *)CRYPTOBASE)


struct cryptodma_t 
{
    unsigned int *data;   
    signed   int length;   
};

static inline void crypto_wait_reset(void)
{
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}

/* ***********************************************************************
 * RSEM1 engine interface 
 * ***********************************************************************/
static inline void crypto_rsem1_init()
{
	 CRYPTO_REG[RSEM1_CTRL]	= RSEM1_CTRL_SOFTRST;
	 crypto_wait_reset();
}

static inline void crypto_rsem1_request(struct cryptodma_t *dma, unsigned int* data, unsigned int length, unsigned int mode)
{
    dma->data  = data;
    dma->length  = length;
    CRYPTO_REG[RSEM1_CTRL] = mode | RSEM1_CTRL_EN;
    crypto_wait_reset();
}

static inline void crypto_rsem1_release(struct cryptodma_t *dma)
{
	dma->data	= 0;
	dma->length = 0;
}

static inline void crypto_rsem1_enable()
{
	CRYPTO_REG[RSEM1_CTRL] |= RSEM1_CTRL_EN;
}

static inline void crypto_rsem1_disable()
{
	CRYPTO_REG[RSEM1_CTRL] &= (~RSEM1_CTRL_EN);
}

static inline int is_rsem1_in_use()
{
	return (CRYPTO_REG[RSEM1_STS] & RSEM1_STS_BUSY);
}

static inline int is_rsem1_dma_end()
{
	return (CRYPTO_REG[RSEM1_STS] & RSEM1_STS_DMAEND);
}

static inline int is_rsem1_outfifonoempty()
{
	return (CRYPTO_REG[RSEM1_STS] & RSEM1_STS_OUTFIFONOEMPTY);
}

static inline int is_rsem1_infifonofull()
{
	return (CRYPTO_REG[RSEM1_STS] & RSEM1_STS_INFIFONOFULL);
}


/* *****************************************************************
 * RSDM1 engine interface
 * ******************************************************************/
static inline void crypto_rsdm1_init()
{
	 CRYPTO_REG[RSDM1_CTRL]	= RSDM1_CTRL_SOFTRST;
	 crypto_wait_reset();
}

static inline void crypto_rsdm1_request(struct cryptodma_t *dma, unsigned int* data, unsigned int length, unsigned int mode)
{
    dma->data  = data;
    dma->length  = length;
    CRYPTO_REG[RSDM1_CTRL] = mode | RSDM1_CTRL_EN;
    crypto_wait_reset();
}

static inline void crypto_rsdm1_release(struct cryptodma_t *dma)
{
	dma->data	= 0;
	dma->length = 0;	
}

static inline void crypto_rsdm1_enable()
{
	CRYPTO_REG[RSDM1_CTRL] |= RSDM1_CTRL_EN;
}

static inline void crypto_rsdm1_disable()
{
	CRYPTO_REG[RSDM1_CTRL] &= (~RSDM1_CTRL_EN);
}

static inline int is_rsdm1_dma_end()
{
	return (CRYPTO_REG[RSDM1_STS] & RSDM1_STS_DMAEND);
}

static inline int is_rsdm1_in_use()
{
	return (CRYPTO_REG[RSDM1_STS] & RSDM1_STS_BUSY);
}

static inline int is_rsdm1_infifonofull()
{
	return (CRYPTO_REG[RSDM1_STS] & RSDM1_STS_INFIFONOFULL);
}

static inline int is_rsdm1_outfifonoempty()
{
	return (CRYPTO_REG[RSDM1_STS] & RSDM1_STS_OUTFIFONOEMPTY);
}

/* ******************************************************************
 * RSEM2 API
 * ******************************************************************/

static inline void crypto_rsem2_init()
{
	 CRYPTO_REG[RSEM2_CTRL]	= RSEM2_CTRL_SOFTRST;
	 crypto_wait_reset();
}

static inline void crypto_rsem2_request(struct cryptodma_t *dma, unsigned int *data, unsigned int length, unsigned int mode)
{
    dma->data  = data;
    dma->length  = length;
    CRYPTO_REG[RSEM2_CTRL] = mode | RSEM2_CTRL_EN;
    crypto_wait_reset();
}

static inline void crypto_rsem2_release(struct cryptodma_t *dma)
{
	dma->data	= 0;
	dma->length = 0;	
}

static inline void crypto_rsem2_enable()
{
	CRYPTO_REG[RSEM2_CTRL] |= RSEM2_CTRL_EN;
}

static inline void crypto_rsem2_disable()
{
	CRYPTO_REG[RSEM2_CTRL] &= (~RSEM2_CTRL_EN);
}

static inline int is_rsem2_in_use()
{
	return (CRYPTO_REG[RSEM2_STS] & RSEM2_STS_BUSY);
}

static inline int is_rsem2_hashrsltvld()
{
	return (CRYPTO_REG[RSEM2_STS] & RSEM2_STS_SCHRSLTVLD);
}

static inline int is_rsem2_dma_end()
{
	return (CRYPTO_REG[RSEM2_STS] & RSEM2_STS_DMAEND);
}

static inline int is_rsem2_outfifonoempty()
{
	return (CRYPTO_REG[RSEM2_STS] & RSEM2_STS_OUTFIFONOEMPTY);
}

static inline int is_rsem2_infifonofull()
{
	return (CRYPTO_REG[RSEM2_STS] & RSEM2_STS_INFIFONOFULL);
}

/* *********************************************************************
 * RSDM2 engine interface
 * *********************************************************************/

static inline void crypto_rsdm2_init()
{
	 CRYPTO_REG[RSDM2_CTRL]	= RSDM2_CTRL_SOFTRST;
	 crypto_wait_reset();
}

static inline void crypto_rsdm2_request(struct cryptodma_t *dma, unsigned int *data, unsigned int length, unsigned int mode)
{
    dma->data  = data;
    dma->length  = length;
    CRYPTO_REG[RSDM2_CTRL] = mode | RSDM2_CTRL_EN;
    crypto_wait_reset();
}

static inline void crypto_rsdm2_release(struct cryptodma_t *dma)
{
	dma->data	= 0;
	dma->length = 0;
}

static inline void crypto_rsdm2_enable()
{
	CRYPTO_REG[RSDM2_CTRL] |= RSDM2_CTRL_EN;
}

static inline void crypto_rsdm2_disable()
{
	CRYPTO_REG[RSDM2_CTRL] &= (~RSDM2_CTRL_EN);
}

static inline int is_rsdm2_in_use()
{
	return (CRYPTO_REG[RSDM2_STS] & RSDM2_STS_BUSY);
}

static inline int is_rsdm2_dma_end()
{
	return (CRYPTO_REG[RSDM2_STS] & RSDM2_STS_DMAEND);
}

static inline int is_rsdm2_outfifonoempty()
{
	return (CRYPTO_REG[RSDM2_STS] & RSDM2_STS_OUTFIFONOEMPTY);
}

static inline int is_rsdm2_infifonofull()
{
	return (CRYPTO_REG[RSDM2_STS] & RSDM2_STS_INFIFONOFULL);
}

/* ********************************************************************
 * RHM API
 * ********************************************************************/
static inline void crypto_rhm_init()
{
	 CRYPTO_REG[RHM_CTRL]	= RHM_CTRL_SOFTRST;
	 crypto_wait_reset();
}

static inline void crypto_rhm_request(struct cryptodma_t *dma, unsigned int *data, unsigned int length, unsigned int mode)
{
    dma->data  = data;
    dma->length  = length;

    CRYPTO_REG[RHM_CTRL] = RHM_CTRL_EN | mode;
    crypto_wait_reset(); 
}

static inline void crypto_rhm_release(struct cryptodma_t *dma)
{
	dma->data	= 0;
	dma->length = 0;	
}

static inline void crypto_rhm_enable()
{
	CRYPTO_REG[RHM_CTRL] |= RHM_CTRL_EN;
}

static inline void crypto_rhm_disable()
{
	CRYPTO_REG[RHM_CTRL] &= (~RHM_CTRL_EN);
}

static inline int is_rhm_dma_end()
{
	return (CRYPTO_REG[RHM_STS] & RHM_STS_DMAEND);
}

static inline int is_rhm_in_use()
{
	return (CRYPTO_REG[RHM_STS] & RHM_STS_BUSY);
}

static inline int is_rhm_schresultvalid()
{
	return (CRYPTO_REG[RHM_STS] & RHM_STS_SCHRSLTVLD);
}


extern PT_THREAD(rsem1_cpu(struct pt *pt));
extern int rsem1_loadkey(unsigned int *key);

extern PT_THREAD(rsdm1_cpu(struct pt *pt));
extern int rsdm1_loadkey(unsigned int *key);

extern PT_THREAD(rsem2_cpu(struct pt *pt));
extern int rsem2_loadkey(unsigned int *key);

extern PT_THREAD(rsdm2_cpu(struct pt *pt));
extern int rsdm2_loadkey(unsigned int *key);

extern PT_THREAD(rhm_cpu(struct pt *pt));


#endif  /* __INCLUDE_CRYPTO_H */
