/****************************************************************************
 * usb/usb.h
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

#ifndef	USB_H_
#define	USB_H_

#include "crypto.h"

/* ****************************************
 * usb device states definion
 * ****************************************/
#define DEV_ATTACHED_STS	0
#define DEV_POWERED_STS		1
#define DEV_DEFAULT_STS		2
#define DEV_ADDRESS_STS		3
#define DEV_CONFIGURED_STS	4
#define DEV_SUSPENDED_STS	5

// command 
#define IDLECMD		1
#define STALLCMD	2
#define FAILCMD		3


/* ****************************************
 * Standard Device Requests Codes
 * ****************************************/
/* 		bRequest 					Value		*/
#define GET_STATUS					0
#define CLEAR_FEATURE				1
#define RSVD_1						2
#define SET_FEATURE					3
#define RSVD_2						4
#define SET_ADDRESS					5
#define GET_DESCRIPTOR				6
#define SET_DESCRIPTOR				7
#define GET_CONFIGURATION			8
#define SET_CONFIGURATION			9
#define GET_INTERFACE				10
#define SET_INTERFACE				11
#define SYNCH_FRAME					12
#define BULK_MASS_STORAGE_RESET		0xFF
#define GET_MAX_LUN					0xFE

/* ****************************************
 * Descriptor Types
 * ****************************************/
#define DEVICE						1
#define CONFIGURATION				2	
#define STRING						3
#define INTERFACE					4
#define ENDPOINT					5
#define DEVICE_QUALIFIER			6
#define OTHER_SPEED_CONFIGURATION	7
#define INTERFACE_POWER				8

/* ***************************************
 * Standard Feature Selectors
 * ***************************************/
#define DEVICE_REMOVE_WAKEUP		1
#define ENDPOINT_HALT				0
#define TEST_MODE					2

/* ***************************************
 * Descriptor and string parameters
 * ***************************************/
#define DEV_DESC_SIZE				sizeof (struct device_desc_s)
#define DEV_QUALIFIER_SIZE			sizeof (struct device_qualifier_s)
#define CONF_DESC_SIZE				sizeof (struct configure_desc_s)
#define INTR_DESC_SIZE				sizeof (struct interrupt_desc_s)
#define ENDP_DESC_SIZE				sizeof (struct endpoint_desc_s)
#define LANGID_SIZE					sizeof (struct langid_desc_s)
#define STRING_0_SIZE				0x1b
#define STRING_1_SIZE				0x17
#define STRING_2_SIZE				0xc
#define J3220_INQUIRY_SIZE			36
#define CBW_SIZE					31
#define CSW_SIZE					13
#define J3220_REQSENSE_SIZE			18
#define J3220_FORMAT_CAP_SIZE		12
#define J3220_READCAP_SIZE			8
#define J3220_MBR_SIZE				512
#define J3220_MODESENSE				12

#define STRING_INDEX_0				0
#define STRING_INDEX_1				1
#define STRING_INDEX_2				2
#define STRING_INDEX_3				3

#define ENDP_NUM_INTR0				10
#define ENDP_NUM_INTR1				0
#define ENDP_NUM					(ENDP_NUM_INTR0 + ENDP_NUM_INTR1)
#define INTR_NUM					1
#define TOTAL_CONF_DESC_SIZE	(unsigned int)(CONF_DESC_SIZE+INTR_DESC_SIZE*INTR_NUM+ENDP_DESC_SIZE*ENDP_NUM )

/* ***************************************
 * Command Block Status Values
 * ***************************************/
#define	CMD_PASS	0x00
#define CMD_FAIL	0x01
#define	PHASE_ERROR	0x02

/* ****************************************
 * SCSI / UFI Comamnds define
 * ****************************************/
#define	FORMAT_UNIT						0x04
#define	INQUIRY							0x12
#define	START_STOP_UNIT					0x1B
#define MODE_SELECT6					0x15
#define	MODE_SELECT10					0x55
#define MODE_SENSE6						0x1A
#define MODE_SENSE10					0x5A
#define	PREVENT_ALLOW_MEDIUM_REMOVAL	0x1E
#define	READ6							0x08
#define	READ10							0x28
#define	READ12							0xA8
#define	READ_CAPACITY10					0x25
#define	READ_FORMAT_CAPACITIES			0x23
#define READ_TOC_PMA_ATIP				0x43
#define	REQUEST_SENSE					0x03
#define	TEST_UNIT_READY					0x00
#define	VERIFY10						0x2F
#define WRITE6							0x0A
#define	WRITE10							0x2A
#define	WRITE12							0xAA


/* ********************************************
 * USB class code
 * ********************************************/
#define USB_HID				0x3
#define USB_MASS_STORAGE	0x8
#define USB_PRINT			0x7
#define USB_HUB				0x9
#define USB_CDC				0xA
#define USB_SMARTCARD		0xB
#define USB_VENDOR			0xFF

#define	USBSUBCLASSRBC		1
#define	USBSUBCLASSSFF8020I	2
#define	USBSUBCLASSQIC157	3
#define	USBSUBCLASSUFI		4
#define	USBSUBCLASSSFF8070I	5
#define	USBSUBCLASSSCSI		6

#define	USBPROTOCOLCBI0		0
#define	USBPROTOCOLCBI1		1
#define	USBPROTOCOLBULK		0x50


#pragma pack(1)

/* ***************************************************
 * setup package struct used in control transfer
 * ***************************************************/
struct setup_req_s 
{
	unsigned char		bmRequestType;
	unsigned char		bRequest;
	unsigned short int 	wValue;
	unsigned short int 	wIndex;
	unsigned short int 	wLength;	
};

/* *********************************************
 *  Standard USB Descriptor Definiations
 * *********************************************/

struct device_desc_s
{
	unsigned char		bLength;
	unsigned char		bDescriptorType;
	unsigned short int	bcdUSB;
	unsigned char		bDeviceClass;
	unsigned char		bDeviceSubClass;
	unsigned char 		bDeviceProtocol;
	unsigned char		bMaxPacketSize0;
	unsigned short int	idVendor;
	unsigned short int	idProduct;
	unsigned short int	bcdDevice;
	unsigned char		iManufacturer;
	unsigned char		iProduct;
	unsigned char		iSerialNumber;
	unsigned char		bNumConfigurations;
};

struct device_qualifier_s
{
	unsigned char		bLength;
	unsigned char 		bDescriptorType;
	unsigned short int	bcdUSB;
	unsigned char		bDeviceClass;
	unsigned char		bDeviceSubClass;
	unsigned char		bDeviceProtocol;
	unsigned char		bMaxPacketSize0;
	unsigned char		bNumConfigurations;
	unsigned char		bReserved;
};

struct configure_desc_s
{
	unsigned char		bLength;
	unsigned char		bDescriptorType;
	unsigned short int wTotalLength;
	unsigned char		bNumInterfaces;
	unsigned char		bConfigurationValue;
	unsigned char		iConfiguration;	
	unsigned char		bmAttrbutes;
	unsigned char		bMaxPower;
};

struct interrupt_desc_s
{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bInterfaceNumber;
	unsigned char	bAlternateSetting;
	unsigned char	bNumEndpoints;
	unsigned char	bInterfaceClass;
	unsigned char	bInterfaceSubClass;
	unsigned char	bInterfaceProtocol;
	unsigned char	iInterface;
};

struct endpoint_desc_s
{
	unsigned char		bLength;
	unsigned char		bDescriptorType;
	unsigned char		bEndpointAddress;
	unsigned char		bmAttributes;
	unsigned short int	wMaxPacketSize;
	unsigned char		bInterval;
};

struct langid_desc_s
{
	unsigned char		bLength;
	unsigned char		bDescriptorType;
	unsigned short int	wLANG0;
};

struct unicodestring_desc_s
{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bString[32];
};

struct configure_packet_desc_s
{
	struct configure_desc_s	cofig_desc;
	struct interrupt_desc_s	intr_desc0;
	struct endpoint_desc_s	endp_desc0[ENDP_NUM_INTR0];
//	struct interrupt_desc_s intr_desc1;
//	struct endpoint_desc_s  endp_desc1[ENDP_NUM_INTR1];
};

struct cbw_s
{
	unsigned int	dCBWSignature;
	unsigned int	dCBWTag;
	unsigned int	dCBWDataTransferLength;
	unsigned char	bmCBWFlags;
	unsigned char	bCBWLUN;
	unsigned char	bCBWCBLength;
	unsigned char	CBWCB[16];
};

struct csw_s
{
	unsigned int 	dCSWSignature;
	unsigned int 	dCSWTag;
	unsigned int	dCSWDataResidue;
	unsigned char	bCSWFlags;
	unsigned char	bCBWLUN;
	unsigned char	bCBWBLength;
	unsigned char	szbCBWCB[16];
};

struct inquiryresponse_s
{
	unsigned char	peripheral;
	unsigned char	removable;
	unsigned char	version;
	unsigned char	responsedataformat;
	unsigned char 	additionallength;
	unsigned char	sccstp;
	unsigned char	bqueetc;
	unsigned char	cmdque;
	char			vendorid[8];
	char			productid[16];
	char			productrev[4];
};

struct requestsenseresponse_s
{
	unsigned char	validresponsecode;
	unsigned char	obsolete;
	unsigned char	sensekey;
	unsigned int	information;
	unsigned char	addsenselen;
	unsigned int	cmdspecificinfo;
	unsigned char	asc;
	unsigned char	ascq;
	unsigned char	fruc;
	unsigned char	sensekeyspecific[3];
	
};

#pragma pack()

/* ************************************************
 * USB device extension structor defined by user
 * ************************************************/

struct device_event_s
{
	unsigned int	eventid;
	void (* eventhandler)(void);
};

struct device_extension_s
{
	unsigned int		devicests;
	unsigned int 		changests;
	unsigned int 		deviceconfig;
	unsigned int 		devicehalt;
	unsigned char  		reqdata[4];
	unsigned short int *pdesc;
	unsigned int  		szdesc;
	unsigned int 		reqerror;	
	unsigned int 		usbinterrupt;
	unsigned int 		cepsts;
	unsigned int 		usbsts;
	struct setup_req_s	setuppackage;
	unsigned short int	temp;
	unsigned int		transsize;
	unsigned short int	transdata[256];
	struct device_event_s	usbevent;
	unsigned int		postinit;
	int					datastageflag;
	struct cryptodma_t 	rsem1;
	struct cryptodma_t 	rsdm1;
	struct cryptodma_t 	rsem2;
	struct cryptodma_t 	rsdm2;
	struct cryptodma_t 	rhm;

};

/* *************************************************
 * command routine for register operation 
 * *************************************************/
static inline unsigned int readreg(unsigned int addr)
{
	volatile unsigned int *p  = (volatile unsigned int *)addr;
	return p[0];
}

static inline void writereg(unsigned int addr, unsigned int data)
{
	volatile unsigned int *p = (volatile unsigned int *)addr;
	p[0] = data;
}

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

/* ****************************************
 * Setup command handler
 * ****************************************/
EXTERN void usb_GetStatus(void);
EXTERN void usb_ClearFeature(void);
EXTERN void usb_rsvd(void);
EXTERN void usb_setfeature(void);
EXTERN void usb_setaddress(void);
EXTERN void usb_getdescriptor(void);
EXTERN void usb_setdescriptor(void);
EXTERN void usb_getconfiguration(void);
EXTERN void usb_setconfiguration(void);
EXTERN void usb_getinterface(void);
EXTERN void usb_setinterface(void);
EXTERN void usb_synchframe(void);
EXTERN void usb_getmaxlun(void);
EXTERN void usb_massreset(void);
EXTERN void usb_default(void);
EXTERN void usb_updatedevice(void);
EXTERN void usb_setupcommand(void);
EXTERN void usb_sentdescriptor(void);
EXTERN void usb_protocolhandler(void);
EXTERN void usb_writehandler(void);
EXTERN void usb_eventhandler(void);
EXTERN void usb_bulkout1handler(void);
EXTERN void usb_bulkin2handler(void);
EXTERN void usb_bulkout3handler(void);
EXTERN void usb_bulkin4handler(void);
EXTERN void usb_bulkout5handler(void);
EXTERN void usb_bulkin6handler(void);
EXTERN void usb_bulkout7handler(void);
EXTERN void usb_bulkin8handler(void);
EXTERN void usb_bulkout9handler(void);
EXTERN void usb_bulkin10handler(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* USB_H_ */



