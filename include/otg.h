/****************************************************************************
 * otg/otg.h
 *
 *   Copyright (C) 2006-2010 Jetway Security Micro. All rights reserved.
 *   Author: Kenichi Cui <kenichi@jetsec.com.cn>
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/********************************************************************************
 * Included Files
 ********************************************************************************/

#ifndef	OTG_H_
#define	OTG_H_


/********************************************************************************
 * Definitions
 ********************************************************************************/


/************************************************************
 * Registers set for USB2.0 OTG controller
 ************************************************************/
#define	OTG_IRQ_STS					(OTG_BASE + 0x704)
#define	OTG_IRQ_STS_ACLRERR			(1 << 0)	/* 1 - a_clr_err */
#define	OTG_IRQ_STS_SRPDETECT		(1 << 1)	/* 1 - srp_detected */
#define	OTG_IRQ_STS_HANDOFFINTR		(1 << 2)	/* 1 - handoff intr */
#define	OTG_IRQ_STS_SRPFAIL			(1 << 3)	/* 1 - srp_fail */
#define	OTG_IRQ_STS_DEVICE			(1 << 4)	/* 1 - Device */
#define	OTG_IRQ_STS_HOST			(1 << 5)	/* 1 - Host */
#define	OTG_IRQ_STS_ABUSREQ			(1 << 6)	/* 1 - a_bus_req */
#define	OTG_IRQ_STS_ABUSDROP		(1 << 7)	/* 1 - b_bus_drop */
#define	OTG_IRQ_STS_AHNPEN			(1 << 8)	/* 1 - a_hnp_en */
#define	OTG_IRQ_STS_BHNPEN			(1 << 9)	/* 1 - b_hnp_en */
#define	OTG_IRQ_STS_BBUSREQ			(1 << 10)	/* 1 - b_bus_req */
#define	OTG_IRQ_STS_RSVD			(1 << 11)	/* 1 - reserved */
#define	OTG_IRQ_STS_ADEVICE			(1 << 12)	/* 1 - a-device */
#define	OTG_IRQ_STS_BDEVICE			(1 << 13)	/* 1 - b-device */
#define OTG_IRQ_STS_SESSIONFAIL		(1 << 14)	
#define	OTG_IRQ_STS_SUSPENDEN		(1 << 15)


#define	OTG_IRQ_EN					(OTG_BASE + 0x708)
#define	OTG_IRQ_EN_ACLRERR			(1 << 0)	/* 1 - a_clr_err */
#define	OTG_IRQ_EN_SRPDET			(1 << 1)	/* 1 - srp_det */
#define	OTG_IRQ_EN_TRANSINTR		(1 << 2)	/* 1 - transition intr */
#define	OTG_IRQ_EN_SRPFAIL			(1 << 3)	/* 1 - srp_fail */
#define	OTG_IRQ_EN_SESSIONFAIL		(1 << 4)
#define	OTG_IRQ_EN_ADEVCHANGE		(1 << 5)
#define	OTG_IRQ_EN_BDEVCHANGE		(1 << 6)
#define	OTG_IRQ_EN_DEVCHANGE		(1 << 7)
#define	OTG_IRQ_EN_HOSTCHANGE		(1 << 8)
#define	OTG_IRQ_EN_DISCONNECTDETECT	(1 << 9)
#define	OTG_IRQ_EN_RSVD				0xfffffc00

#define	AHB_CTRL					(OTG_BASE + 0x70c)
#define	AHB_CTRL_AHBCTRL			(1 << 0)
#define	AHB_CTRL_RSVD				0xfffffffe

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

EXTERN void otg_init(void);
EXTERN void otg_delay(unsigned int time_c);
EXTERN void otg_reset(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* OTG_H_ */

