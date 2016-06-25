/****************************************************************************
 * isr/isr.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "isr.h"
#include "device.h"
#include "usb.h"
#include "thread.h"
#include "opt/printf.h"
#include "nfc.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define DUMP_INTERRUPT_
/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern struct device_extension_s dx;

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

void isr_handler(unsigned int irq_level)
{	
#ifdef HAS_USB	
	unsigned int irqsts, cepsts, usbsts, usben, cepen, ncepsts;
#endif	
	switch (irq_level)
	{
	case USB_IRQ:
		disable_irq(USB_IRQ);
		
#ifdef HAS_USB		
		dx.usbinterrupt = USB_IRQ_ACTIVE;
		
		irqsts = readreg(IRQ_STAT_L);
		writereg(IRQ_STAT_L, irqsts);
		
		if (irqsts & 0x1)
		{
			usbsts = readreg(USB_IRQ_STAT);
			usben = readreg(USB_IRQ_ENB);
			writereg(USB_IRQ_STAT, usbsts);
			
			if (usbsts & USB_IRQ_STAT_CLCUSEINT & usben)
			{
				writereg(USB_OPER,USB_OPER_USBHISPD);
				device_init();
				device_enableinterrupt();
			}
			else
			{
				dx.usbsts = usbsts;
				writereg(IRQ_STAT_L,0x1);
	
				dx.usbevent.eventid = USB_EVENT;
				dx.usbevent.eventhandler = usb_eventhandler;
			}
		}
		else if (irqsts & 0x2)
		{
			cepsts = readreg(CEP_IRQ_STAT);
			cepen  = readreg(CEP_IRQ_ENB);
			writereg(CEP_IRQ_STAT, cepsts);
			
			if( cepsts & CEP_IRQ_STAT_INTOKENINT  & cepen)
			{
				writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) & 0xFFF7));
			}
			
			if (cepsts & CEP_IRQ_STAT_USBERRORINT & cepen)
			{
				writereg(CEP_CTRL_STAT, CEP_CTRL_STAT_FLUSH);
			}
			dx.cepsts = cepsts;
			
			dx.usbevent.eventid = USB_PROTOCOL;
			dx.usbevent.eventhandler = usb_protocolhandler;
		}
		else if (irqsts & 0x4) 
		{
#ifndef DUMP_INTERRUPT_
			printf("EP1 OUT trigger \n");
#endif			
			ncepsts = readreg(EP1_IRQ_STAT);
			writereg(EP1_IRQ_STAT, ncepsts);
			writereg(EP1_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAOUTTOKENINT)
			{
				writereg(EP2_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 
			}
			if (ncepsts & EP_IRQ_STAT_DATAPACKETRECVINT)
			{		
				dx.usbevent.eventid = USB_BULKOUT;
				dx.usbevent.eventhandler = usb_bulkout1handler;
			}
						
		}
		else if (irqsts & 0x8) 
		{
#ifndef DUMP_INTERRUPT_			
			printf("EP2 IN trigger \n");
#endif			
			ncepsts = readreg(EP2_IRQ_STAT);
			writereg(EP2_IRQ_STAT, ncepsts);
			writereg(EP2_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAPACKETTRANSINT)
			{
				writereg(EP2_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));
			}	
			if (ncepsts & EP_IRQ_STAT_DATAINTOKENINT)
			{
				dx.usbevent.eventid = USB_BULKIN;
				dx.usbevent.eventhandler = usb_bulkin2handler;
			}			
		}
		else if (irqsts & 0x10) 
		{	
#ifndef DUMP_INTERRUPT_			
			printf("EP3 OUT trigger \n");
#endif			
			ncepsts = readreg(EP3_IRQ_STAT);
			writereg(EP3_IRQ_STAT, ncepsts);
			writereg(EP3_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAOUTTOKENINT)
			{
				writereg(EP4_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 
			}
			if (ncepsts & EP_IRQ_STAT_DATAPACKETRECVINT)
			{		
			//	crypto_rsem1_init();
			//	NFC_DMAW_init();
				dx.usbevent.eventid = USB_BULKOUT;
				dx.usbevent.eventhandler = usb_bulkout3handler;
			}	
		}
		else if (irqsts & 0x20) 
		{
#ifndef DUMP_INTERRUPT_		
			printf("EP4 IN trigger \n");
#endif			
			
			ncepsts = readreg(EP4_IRQ_STAT);
			writereg(EP4_IRQ_STAT, ncepsts);
			writereg(EP4_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAPACKETTRANSINT)
			{
				writereg(EP4_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));
			}	
			if (ncepsts & EP_IRQ_STAT_DATAINTOKENINT)
			{
		//		crypto_rsdm1_init();
		//		NFC_DMAR_init();
				dx.usbevent.eventid = USB_BULKIN;
				dx.usbevent.eventhandler = usb_bulkin4handler;
			}	
		}		
		else if (irqsts & 0x40) 
		{
#ifndef DUMP_INTERRUPT_			
			printf("EP5 OUT trigger \n");
#endif				
			ncepsts = readreg(EP5_IRQ_STAT);
			writereg(EP5_IRQ_STAT, ncepsts);
			writereg(EP5_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAOUTTOKENINT)
			{
				writereg(EP6_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 
			}
			if (ncepsts & EP_IRQ_STAT_DATAPACKETRECVINT)
			{		
				dx.usbevent.eventid = USB_BULKOUT;
				dx.usbevent.eventhandler = usb_bulkout5handler;
			}
		}
		else if (irqsts & 0x80) 
		{
#ifndef DUMP_INTERRUPT_			
			printf("EP6 IN trigger \n");
#endif				
			ncepsts = readreg(EP6_IRQ_STAT);
			writereg(EP6_IRQ_STAT, ncepsts);
			writereg(EP6_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAPACKETTRANSINT)
			{
				writereg(EP6_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));
			}	
			if (ncepsts & EP_IRQ_STAT_DATAINTOKENINT)
			{
				dx.usbevent.eventid = USB_BULKIN;
				dx.usbevent.eventhandler = usb_bulkin6handler;
			}			
		}
		else if (irqsts & 0x100) 
		{
#ifndef DUMP_INTERRUPT_			
			printf("EP7 OUT trigger \n");
#endif				
			ncepsts = readreg(EP7_IRQ_STAT);
			writereg(EP7_IRQ_STAT, ncepsts);
			writereg(EP7_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAOUTTOKENINT)
			{
				writereg(EP8_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 
			}
			if (ncepsts & EP_IRQ_STAT_DATAPACKETRECVINT)
			{		
				dx.usbevent.eventid = USB_BULKOUT;
				dx.usbevent.eventhandler = usb_bulkout7handler;
			}
		}
		else if (irqsts & 0x200) 
		{
#ifndef DUMP_INTERRUPT_			
			printf("EP8 IN trigger \n");
#endif				
			ncepsts = readreg(EP8_IRQ_STAT);
			writereg(EP8_IRQ_STAT, ncepsts);
			writereg(EP8_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAPACKETTRANSINT)
			{
				writereg(EP8_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));
			}	
			if (ncepsts & EP_IRQ_STAT_DATAINTOKENINT)
			{
				dx.usbevent.eventid = USB_BULKIN;
				dx.usbevent.eventhandler = usb_bulkin8handler;
			}				
		}
		else if (irqsts & 0x400) 
		{
#ifndef DUMP_INTERRUPT_			
			printf("EP9 OUT trigger \n");
#endif				
			ncepsts = readreg(EP9_IRQ_STAT);
			writereg(EP9_IRQ_STAT, ncepsts);
			writereg(EP9_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAPACKETTRANSINT)
			{
				writereg(EP10_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));
			}	
			if (ncepsts & EP_IRQ_STAT_DATAPACKETRECVINT)
			{		
				dx.usbevent.eventid = USB_BULKOUT;
				dx.usbevent.eventhandler = usb_bulkout9handler;
			}
		}
		else if (irqsts & 0x800) 
		{
#ifndef DUMP_INTERRUPT_			
			printf("EP10 IN trigger \n");
#endif				
			ncepsts = readreg(EP10_IRQ_STAT);
			writereg(EP10_IRQ_STAT, ncepsts);
			writereg(EP10_IRQ_ENB, 0);
			if (ncepsts & EP_IRQ_STAT_DATAPACKETTRANSINT)
			{
				writereg(EP10_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));
			}	
			if (ncepsts & EP_IRQ_STAT_DATAINTOKENINT)
			{
				dx.usbevent.eventid = USB_BULKIN;
				dx.usbevent.eventhandler = usb_bulkin10handler;
			}				
		}	
#endif		
		break;
	case NFC_IRQ:
		disable_irq(NFC_IRQ);
		break;
	default:
		break;
	}

}




