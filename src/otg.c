/****************************************************************************
 * otg/otg.c
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

#include "otg.h"
#include "device.h"
#include "usb.h"
#include "isr.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/


void otg_init(void)
{
	unsigned int *gpio = (unsigned int *)GPIO_BASE;
	
	while ((gpio[0] & 0x10000) != 0x00010000) {}
	gpio[6] = 0x0000dfff;
	
	writereg(OTG_IRQ_EN, 0);
	writereg(OTG_IRQ_STS, OTG_IRQ_STS_SUSPENDEN + OTG_IRQ_STS_BBUSREQ);
}

void otg_delay(unsigned int time_c)
{
	while (time_c--)
	{
		asm("nop");
		asm("nop");
	}
}

void otg_reset(void)
{
	unsigned int *gcc = (unsigned int *)GCC_BASE;
	
	gcc[0] = gcc[0] & 0xffffefff;
	otg_delay(0x2000);
	gcc[0] = gcc[0] | 0x00001000;
	
	otg_init();
	device_init();
	device_enableinterrupt();
}

void device_init(void)
{
	writereg(CEP_START_ADDR, 0x0);
	writereg(CEP_END_ADDR, 0x1f);
	writereg(USB_OPER, USB_OPER_USBHISPD);
	writereg(USB_ADDR, 0x0);
	
	writereg(EP1_IRQ_ENB, 0x10);
	writereg(EP1_RSP_SC, 0x00);
	writereg(EP1_MPS, 0x200);
	writereg(EP1_CFG, 0x13);
	writereg(EP1_START_ADDR, 0x520);
	writereg(EP1_END_ADDR, 0x61f);

	writereg(EP2_IRQ_ENB, 0x08);
	writereg(EP2_RSP_SC, 0x0);
	writereg(EP2_MPS, 0x200);
	writereg(EP2_CFG, 0x2b);
	writereg(EP2_START_ADDR, 0x020);
	writereg(EP2_END_ADDR, 0x11f);	

	writereg(EP3_IRQ_ENB, 0x10);
	writereg(EP3_RSP_SC, 0x0);
	writereg(EP3_MPS, 0x200);
	writereg(EP3_CFG, 0x33);
	writereg(EP3_START_ADDR, 0x520);
	writereg(EP3_END_ADDR, 0x61f);

	writereg(EP4_IRQ_ENB, 0x08);
	writereg(EP4_RSP_SC, 0x0);
	writereg(EP4_MPS, 0x200);
	writereg(EP4_CFG, 0x4b);
	writereg(EP4_START_ADDR, 0x120);
	writereg(EP4_END_ADDR, 0x21f);

	writereg(EP5_IRQ_ENB, 0x10);
	writereg(EP5_RSP_SC, 0x0);
	writereg(EP5_MPS, 0x200);
	writereg(EP5_CFG, 0x53);
	writereg(EP5_START_ADDR, 0x520);
	writereg(EP5_END_ADDR, 0x61f);
		
	writereg(EP6_IRQ_ENB, 0x08);
	writereg(EP6_RSP_SC, 0x0);
	writereg(EP6_MPS, 0x200);
	writereg(EP6_CFG, 0x6b);
	writereg(EP6_START_ADDR, 0x220);
	writereg(EP6_END_ADDR, 0x31f);

	writereg(EP7_IRQ_ENB, 0x10);
	writereg(EP7_RSP_SC, 0x0);
	writereg(EP7_MPS, 0x200);
	writereg(EP7_CFG, 0x73);
	writereg(EP7_START_ADDR, 0x520);
	writereg(EP7_END_ADDR, 0x61f);
	
	writereg(EP8_IRQ_ENB, 0x08);
	writereg(EP8_RSP_SC, 0x0);
	writereg(EP8_MPS, 0x200);
	writereg(EP8_CFG, 0x8b);
	writereg(EP8_START_ADDR, 0x320);
	writereg(EP8_END_ADDR, 0x41f);

	writereg(EP9_IRQ_ENB, 0x10);
	writereg(EP9_RSP_SC, 0x0);
	writereg(EP9_MPS, 0x200);
	writereg(EP9_CFG, 0x93);
	writereg(EP9_START_ADDR, 0x520);
	writereg(EP9_END_ADDR, 0x61f);
	
	writereg(EP10_IRQ_ENB, 0x08);
	writereg(EP10_RSP_SC, 0x0);
	writereg(EP10_MPS, 0x200);
	writereg(EP10_CFG, 0xab);
	writereg(EP10_START_ADDR, 0x420);
	writereg(EP10_END_ADDR, 0x51f);


}

void device_enableinterrupt(void)
{
	writereg(IRQ_ENB_L, 0xffff);
	writereg(IRQ_ENB_H, 0x00);
	writereg(USB_IRQ_ENB, 	USB_IRQ_ENB_RSTSTS | \
							USB_IRQ_ENB_CLCUSEINTENB | \
							USB_IRQ_ENB_SUSREQ | \
							USB_IRQ_ENB_RESUME);
	writereg(CEP_IRQ_ENB, 	CEP_IRQ_ENB_STSCOMPINT | \
							CEP_IRQ_ENB_SETUPPACKETINT | \
							CEP_IRQ_ENB_DATAPACKETTRANSINT | \
							CEP_IRQ_ENB_USBERRORINT);
}


