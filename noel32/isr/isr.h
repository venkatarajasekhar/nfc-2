/****************************************************************************
 * isr/isr.h
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

#ifndef ISR_H_
#define ISR_H_

/********************************************************************************
 * Definitions
 ********************************************************************************/
#define	USB_IRQ		4
#define NFC_IRQ		5

#define USB_IRQ_ACTIVE		0x01
#define USB_IRQ_DEACTIVE	0x00

#define IRQ_GMASK_BASE	0x08000218
#define IRQ_PEND_BASE	0x08000200
#define IRQ_MASK_BASE	0x08000240
#define IRQ_CLEAR_BASE	0x0800020c

#define GPIO_BASE		0x08000500
#define GCC_BASE		0x08000524

/********************************************************************************
 * Public Variables
 ********************************************************************************/

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

static inline void enable_gmask(void)
{
	volatile unsigned int *irqgmask = (volatile unsigned int *)IRQ_GMASK_BASE;
	*irqgmask = 1;
}

static inline void disable_gmask(void)
{
	volatile unsigned int *irqgmask  = (volatile unsigned int *)IRQ_GMASK_BASE;
	*irqgmask = 0;
}

static inline void clear_pend(void)
{
	volatile unsigned int *irqpend = (volatile unsigned int *)IRQ_PEND_BASE;
	*irqpend = 0;
}

static inline void enable_irq (unsigned int irq) 
{
	volatile unsigned int *irqmask = (volatile unsigned int *)IRQ_MASK_BASE;
	*irqmask = (1 << irq);	
}

static inline void disable_irq (unsigned int irq) 
{
	volatile unsigned int *irqmask = (volatile unsigned int *)IRQ_MASK_BASE;
	*irqmask = 0;
}

static inline void clear_irq (unsigned int irq) 
{
	volatile unsigned int *irqclear = (volatile unsigned int *)IRQ_CLEAR_BASE;
	*irqclear = (1 << irq);	
}


#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif
	
EXTERN void isr_handler(unsigned int irq_level);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* ISR_H_ */
