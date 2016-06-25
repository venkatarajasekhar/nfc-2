/****************************************************************************
 * usb/thread.c
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

#include "thread.h"
#include "usb.h"
#include "isr.h"
#include "otg.h"
#include "crypto.h"
#include "nfc.h"
/****************************************************************************
 * Definitions
 ****************************************************************************/

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


/* ***********************************************
 * a generate routine for usb hardware initialize state
 * ***********************************************/
PT_THREAD(usb_initialize_thread(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_UNTIL(pt, dx.postinit != USB_POSTINIT);
	otg_init();
	enable_irq(USB_IRQ);

	dx.devicests	= DEV_ATTACHED_STS;
	dx.changests	= DEV_ATTACHED_STS;
	dx.deviceconfig	= 0;
	dx.devicehalt	= 0;
	dx.pdesc		= 0x00000000;
	dx.szdesc		= 0;
	dx.reqerror		= 0;
	dx.usbinterrupt = USB_IRQ_DEACTIVE;
	dx.cepsts		= 0;
	dx.usbsts		= 0;
	dx.postinit 	= USB_POSTINIT;
	
	PT_END(pt);
}

/* ***********************************************
 * a generate routine for usb idle stuff
 * ***********************************************/
PT_THREAD(usb_idle_thread(struct pt *pt))
{	
	PT_BEGIN(pt);
	
	PT_WAIT_UNTIL(pt, dx.usbinterrupt == USB_IRQ_DEACTIVE);
	asm("mov %g0, %asr19");
	asm("nop");
	asm("nop");
	asm("nop");
	
	PT_END(pt);
}

/* ***********************************************
 * a generate routine for usb idle stuff
 * ***********************************************/
PT_THREAD(usb_dummy_thread(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_UNTIL(pt, (dx.usbinterrupt == USB_IRQ_ACTIVE)&&(dx.usbevent.eventid == USB_DUMMY));
	dx.usbinterrupt = USB_IRQ_DEACTIVE;
	enable_irq(USB_IRQ);
	
	PT_END(pt);
}

/* ***********************************************
 * a generate routine for usb protocol stuff
 * ***********************************************/
PT_THREAD(usb_protocol_thread(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_UNTIL(pt, (dx.usbinterrupt == USB_IRQ_ACTIVE)&&(dx.usbevent.eventid == USB_PROTOCOL));
	(*dx.usbevent.eventhandler)();
	dx.usbevent.eventid = 0x00;
	dx.usbevent.eventhandler = 0x00;
	dx.usbinterrupt = USB_IRQ_DEACTIVE;
	enable_irq(USB_IRQ);
	
	PT_END(pt);
}

/* ***********************************************
 * a generate routine for usb event stuff
 * ***********************************************/
PT_THREAD(usb_event_thread(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_UNTIL(pt, (dx.usbinterrupt == USB_IRQ_ACTIVE)&&(dx.usbevent.eventid == USB_EVENT));
	(*dx.usbevent.eventhandler)();
	dx.usbevent.eventid = 0x00;
	dx.usbevent.eventhandler = 0x00;
	dx.usbinterrupt = USB_IRQ_DEACTIVE;
	enable_irq(USB_IRQ);
	
	PT_END(pt);
}

/* ***********************************************
 * a generate routine for usb send data stuff for bulk loop testing 
 * ***********************************************/
PT_THREAD(usb_bulkin_thread(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_UNTIL(pt, (dx.usbinterrupt == USB_IRQ_ACTIVE)&&(dx.usbevent.eventid == USB_BULKIN));
	(*dx.usbevent.eventhandler)();
	dx.usbevent.eventid = 0x00;
	dx.usbevent.eventhandler = 0x00;
	dx.usbinterrupt = USB_IRQ_DEACTIVE;
	enable_irq(USB_IRQ);
	
	PT_END(pt);
}

/* ***********************************************
 * a generate routine for usb receive data stuff for bulk loop testing
 * ***********************************************/
PT_THREAD(usb_bulkout_thread(struct pt *pt))
{
	PT_BEGIN(pt);
	
	PT_WAIT_UNTIL(pt, (dx.usbinterrupt == USB_IRQ_ACTIVE)&&(dx.usbevent.eventid == USB_BULKOUT));
	(*dx.usbevent.eventhandler)();
	dx.usbevent.eventid = 0x00;
	dx.usbevent.eventhandler = 0x00;
	dx.usbinterrupt = USB_IRQ_DEACTIVE;
	enable_irq(USB_IRQ);
	
	PT_END(pt);
}


