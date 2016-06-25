/****************************************************************************
 * usb/thread.h
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

#ifndef THREAD_H_
#define THREAD_H_


/********************************************************************************
 * Included Files
 ********************************************************************************/
#include "pt/pt.h"


/********************************************************************************
 * Definitions
 ********************************************************************************/

/* ****************************************
 * interrupt event handler 
 * ****************************************/
#define USB_DUMMY		0x0
#define USB_PROTOCOL	0x1
#define USB_CSWEVT		0x2
#define USB_WRITEOPR	0x3
#define USB_CBWEVT		0x4
#define USB_EVENT		0x5
#define USB_HOSTEVT		0x6
#define USB_BULKIN		0x7
#define USB_BULKOUT		0x8

#define USB_POSTINIT	0x01



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

/*****************************************
 * PT generater routines
 * ***************************************/
EXTERN PT_THREAD(usb_initialize_thread(struct pt *pt));
EXTERN PT_THREAD(usb_idle_thread(struct pt *pt));
EXTERN PT_THREAD(usb_dummy_thread(struct pt *pt));
EXTERN PT_THREAD(usb_protocol_thread(struct pt *pt));
EXTERN PT_THREAD(usb_event_thread(struct pt *pt));
EXTERN PT_THREAD(usb_bulkin_thread(struct pt *pt));
EXTERN PT_THREAD(usb_bulkout_thread(struct pt *pt));

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /*THREAD_H_*/
