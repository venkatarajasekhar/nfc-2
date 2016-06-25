/****************************************************************************
 * usr/main.c
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
#include "otg.h"
#include "usb.h"
#include "thread.h"
#include "opt/printf.h"
#include "crypto.h"
#include "led.h"

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

int main()
{
	static struct pt usb_init_pt;
	static struct pt usb_idle_pt;
	static struct pt usb_protocol_pt;
	static struct pt usb_event_pt;
	static struct pt usb_bulkin_pt;
	static struct pt usb_bulkout_pt;
	static struct pt usb_dummy_pt;
	
	PT_INIT(&usb_init_pt);
	PT_INIT(&usb_idle_pt);
	PT_INIT(&usb_protocol_pt);
	PT_INIT(&usb_event_pt);
	PT_INIT(&usb_bulkin_pt);
	PT_INIT(&usb_bulkout_pt);
	PT_INIT(&usb_dummy_pt);
	
	usb_initialize_thread(&usb_init_pt);

	while (1)
	{
		usb_idle_thread(&usb_idle_pt);
		usb_dummy_thread(&usb_dummy_pt);
		usb_protocol_thread(&usb_protocol_pt);
		usb_event_thread(&usb_event_pt);
		usb_bulkin_thread(&usb_bulkin_pt);
		usb_bulkout_thread(&usb_bulkout_pt);
	
	}

}







