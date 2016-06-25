/****************************************************************************
 * usb/usb.c
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

#include "usb.h"
#include "otg.h"
#include "device.h"
#include "isr.h"
#include "util.h"
#include "opt/printf.h"
#include "led.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define USB_CPUMODE_
/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
struct device_extension_s dx;

static const struct device_desc_s dev_desc = {
		
		DEV_DESC_SIZE, 		// size of this descriptor 
		DEVICE,				// device descriptor type
		0x0002,				// USB Specification BCD
		0x00, 				// class code
		0x00,				// subclass code
		0x00, 				// Protocol code
		0x40,				// maximum package size
		0x601b,				// vendor ID
		0x2632,				// product ID
		0x0001, 			// device ID BCD
		0x00, 				// index of string descriptor manufacture
		0x01, 				// index of string descriptor product
		0x02, 				// index of string descriptor device's serial number
		0x01				// number of possible configuration
				
};

static const struct configure_packet_desc_s config_packet_desc = 
{ 
	// Configuration Description
	{
		CONF_DESC_SIZE,			// size of this descriptor
		CONFIGURATION,				// CONFIGURATION Desc Type
		(TOTAL_CONF_DESC_SIZE << 8),	// Total length of data returned
		INTR_NUM,						// number of interface
		1,							// number of configuration
		0,							// index of configuration string
		0xc0,						// Self-Power and support Remote wakeup
		0x32						// mA	

	},
	// Interface 0 Description
	{
		INTR_DESC_SIZE,			// size of this descriptor
		INTERFACE,				// INTERFACE Desc Type
		0,						// number of this interface
		0,						// alternate setting
		ENDP_NUM_INTR0,			// number of endpoint
		0xff,					// interface class code
		0x00,					// interface subclass code
		0x00,					// interface protocol
		0x00					// interface string index
	},
	{
		// Endpoint Descriptor, Logic Endpoint 1, OUT
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc TYPE
			0x1,					// OUT endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,      			// Max Packet Size, it is configurable
			0x00					// interval(ms)
		},
		// Endpoint Descriptor, Logic Endpoint 2, IN
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc Type
			0x82,					// IN endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,					// Max packet Size, it is configureable
			0x00					// interval(ms)
		},	
		
		// Endpoint Descriptor, Logic Endpoint 3, OUT
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc TYPE
			0x3,					// OUT endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,      			// Max Packet Size, it is configurable
			0x00					// interval(ms)
		},
		// Endpoint Descriptor, Logic Endpoint 4, IN
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc Type
			0x84,					// IN endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,					// Max packet Size, it is configureable
			0x00					// interval(ms)
		},				
		// Endpoint Descriptor, Logic Endpoint 5, OUT
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc TYPE
			0x5,					// OUT endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,      			// Max Packet Size, it is configurable
			0x00					// interval(ms)
		},
		// Endpoint Descriptor, Logic Endpoint 6, IN
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc Type
			0x86,					// IN endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,					// Max packet Size, it is configureable
			0x00					// interval(ms)
		},		
		// Endpoint Descriptor, Logic Endpoint 7, OUT
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc TYPE
			0x7,					// OUT endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,      			// Max Packet Size, it is configurable
			0x00					// interval(ms)
		},
		// Endpoint Descriptor, Logic Endpoint 8, IN
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc Type
			0x88,					// IN endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,					// Max packet Size, it is configureable
			0x00					// interval(ms)
		},
		// Endpoint Descriptor, Logic Endpoint 9, OUT
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc TYPE
			0x9,					// OUT endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,      			// Max Packet Size, it is configurable
			0x00					// interval(ms)
		},
		// Endpoint Descriptor, Logic Endpoint 10, IN
		{
			ENDP_DESC_SIZE,			// size of this descriptor
			ENDPOINT,				// ENDPOINT Desc Type
			0x8a,					// IN endpoint address
			0x2,					// Bulk Transfer Type
			0x0002,					// Max packet Size, it is configureable
			0x00					// interval(ms)
		},	
		
	}
	
};

static const struct langid_desc_s languageid_desc = 
{
		0x04, 						// total length
		0x03, 						// langusageid desc type
		0x0904						// langid0
};

static const char string0_desc[STRING_0_SIZE] = 
{
		0x1b, 0x03,
		0x4a, 0x65, 0x74, 0x77, 0x61, 0x79, 0x20, 0x53, 
		0x65, 0x63, 0x75, 0x72, 0x69, 0x74, 0x79, 0x20, 
		0x4d, 0x69, 0x63, 0x72, 0x6f, 0x20, 0x49, 0x6e,
		0x63,
};

static const char string1_desc[STRING_1_SIZE] = 
{
		0x17, 0x03,
		0x4a, 0x65, 0x74, 0x77, 0x61, 0x79, 0x20, 0x53, 
		0x65, 0x63, 0x75, 0x72, 0x69, 0x74, 0x79, 0x20, 
		0x44, 0x69, 0x73, 0x6b, 0x20
};

static const char string2_desc[STRING_2_SIZE] = 
{
		0x0c, 0x03,
		0x31, 0x00, 0x30, 0x00, 0x31, 0x00, 0x30, 0x00, 0x31, 0x00
						
};




/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

void usb_getstatus(void)
{
	dx.reqdata[0] = 0;
	dx.reqdata[1] = 0;
	dx.pdesc = (unsigned short int *)dx.reqdata;
	dx.szdesc = 2;
	writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
}

void usb_clearfeature(void)
{

}

void usb_rsvd(void)
{
	
}

void usb_setfeature(void)
{

}

void usb_setaddress(void)
{
	if ((dx.setuppackage.wValue & 0xffff) > 0x7f)
	{
		dx.reqerror = 1;
	}
	if (dx.devicests == DEV_CONFIGURED_STS)
	{
		dx.reqerror = 1;
	}
	if (dx.reqerror == 1)
	{
		return ;
	}
	if (dx.devicests == DEV_ADDRESS_STS)
	{
		if ((dx.setuppackage.wValue & 0xffff) == 0)
		{
			dx.changests = DEV_DEFAULT_STS;
		}
	}
	if (dx.devicests == DEV_DEFAULT_STS )
	{
		if ((dx.setuppackage.wValue & 0xffff) != 0)
		{
			dx.changests = DEV_ADDRESS_STS;
		}
		else
		{
			dx.changests = DEV_DEFAULT_STS;
		}
	}
	dx.pdesc = (unsigned short int *)dx.reqdata;
	dx.szdesc = 0;
	writereg(CEP_CTRL_STAT, 0);
	
}

void usb_getdescriptor(void)
{
	switch ((dx.setuppackage.wValue & 0xff00) >> 8)
	{
	case DEVICE:
		memcpy((unsigned char *)dx.transdata, (unsigned char *)&dev_desc, sizeof(struct device_desc_s));
		if ((dx.setuppackage.wLength & 0xff) > DEV_DESC_SIZE)
		{
			dx.szdesc = DEV_DESC_SIZE;
		}
		else
		{
			dx.szdesc = dx.setuppackage.wLength & 0xff;
		}
		dx.pdesc = (unsigned short int *)dx.transdata;
		writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
		break;
	case CONFIGURATION:
		memcpy((unsigned char *)dx.transdata, (unsigned char *)&config_packet_desc, TOTAL_CONF_DESC_SIZE);
		
		if ((dx.setuppackage.wLength & 0xff) <= CONF_DESC_SIZE)
		{
			dx.szdesc = (dx.setuppackage.wLength & 0xff);
			dx.pdesc = (unsigned short int *)dx.transdata;
			writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
		}
		else 
		{
			if (TOTAL_CONF_DESC_SIZE > 0x40)
			{
				dx.szdesc = 0x40;
				dx.pdesc = (unsigned short int *)dx.transdata;	
				usb_sentdescriptor();
				writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
				dx.szdesc = TOTAL_CONF_DESC_SIZE - 0x40;
				dx.pdesc = (unsigned short int *)&dx.transdata[0x20];	
				usb_sentdescriptor();
			}
			else
			{
				dx.szdesc = TOTAL_CONF_DESC_SIZE;
				dx.pdesc = (unsigned short int *)dx.transdata;	
			}
			writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
		}				
		
		break;
	case STRING:
		{
			switch(dx.setuppackage.wValue & 0xff)
			{
			case STRING_INDEX_0:
				memcpy((unsigned char *)dx.transdata, (unsigned char *)&languageid_desc, sizeof(struct langid_desc_s));
				if ((dx.setuppackage.wLength & 0xff) > LANGID_SIZE)
				{
					dx.szdesc = LANGID_SIZE;	
				}
				else
				{
					dx.szdesc = dx.setuppackage.wLength & 0xff;
				}				
				dx.pdesc = (unsigned short int *)dx.transdata;
				writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
				break;
			case STRING_INDEX_1:
				memcpy((unsigned char *)dx.transdata, (unsigned char *)&string0_desc, STRING_0_SIZE);
				if ((dx.setuppackage.wLength & 0xff) > STRING_0_SIZE)
				{
					dx.szdesc = STRING_0_SIZE;	
				}
				else
				{
					dx.szdesc =  dx.setuppackage.wLength & 0xff;
				}		
				dx.pdesc = (unsigned short int *)dx.transdata;
				writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
				break;
			case STRING_INDEX_2:
				memcpy((unsigned char *)dx.transdata, (unsigned char *)&string1_desc, STRING_1_SIZE);
				if ((dx.setuppackage.wLength & 0xff) > STRING_1_SIZE)
				{
					dx.szdesc = STRING_1_SIZE;	
				}
				else
				{
					dx.szdesc =  dx.setuppackage.wLength & 0xff;
				}			
				dx.pdesc = (unsigned short int *)dx.transdata;
				writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
				break;
			case STRING_INDEX_3:
				memcpy((unsigned char *)dx.transdata, (unsigned char *)&string2_desc, STRING_2_SIZE);
				if ((dx.setuppackage.wLength & 0xff) > STRING_2_SIZE)
				{
					dx.szdesc = STRING_2_SIZE;	
				}
				else
				{
					dx.szdesc =  dx.setuppackage.wLength & 0xff;
				}												
				dx.pdesc = (unsigned short int *)dx.transdata;
				writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
				break;
			default:
				writereg(CEP_CTRL_STAT, CEP_CTRL_STAT_STALL);
				writereg(CEP_IRQ_ENB, readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_STALLSENTINT);
				device_init();
				device_enableinterrupt();
				break;
			}
		}
		break;
	case INTERFACE:
		writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
		break;
	case ENDPOINT:
		writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
		break;
	case DEVICE_QUALIFIER:
		writereg(CEP_CTRL_STAT, CEP_CTRL_STAT_STALL);
		writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
		break;
	default:
		break;
	}
}

void usb_setdescriptor(void)
{
	dx.reqerror = 1;
}

void usb_getconfiguration(void)
{
	if (dx.devicests == DEV_DEFAULT_STS)
	{
		dx.reqerror = 1;
	}
	if (dx.reqerror == 1)
	{
		return ;
	}
	if (dx.devicests == DEV_ADDRESS_STS)
	{
		dx.reqdata[0] = 0x00;
		dx.pdesc = (unsigned short int *)dx.reqdata;
		dx.szdesc = 1;
	}
	if (dx.devicests == DEV_CONFIGURED_STS)
	{
		dx.reqdata[0] = dx.deviceconfig;
		dx.pdesc = (unsigned short int *)dx.reqdata;
		dx.szdesc = 1;
	}
	writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
}

void usb_setconfiguration(void)
{
	if (dx.devicests == DEV_DEFAULT_STS)
	{
		dx.reqerror = 1;
	}
	if (dx.reqerror == 1)
	{
		return ;
	}
	if ((dx.setuppackage.wValue & 0xffff) == 0)
	{
		dx.changests = DEV_ADDRESS_STS;
	}
	else
	{
		dx.changests = DEV_CONFIGURED_STS;
	}
	dx.deviceconfig = dx.setuppackage.wValue & 0xff;
	dx.pdesc = (unsigned short int *)dx.reqdata;
	dx.szdesc = 0;
	writereg(CEP_CTRL_STAT, 0);
}

void usb_getinterface(void)
{
	if ((dx.devicests == DEV_DEFAULT_STS) || (dx.devicests == DEV_ADDRESS_STS))
	{
		dx.reqerror = 1;
	}
	if (dx.reqerror == 1)
	{
		return ;
	}
	if (dx.devicests == DEV_CONFIGURED_STS)
	{
		dx.reqdata[0]= dx.devicehalt;
		dx.pdesc = (unsigned short int *)dx.reqdata;
		dx.szdesc = 1;
	}
	writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
}

void usb_setinterface(void)
{
	if (dx.devicests == DEV_CONFIGURED_STS)
	{
		
	}
	else
	{
		dx.reqerror = 1;
	}
	dx.pdesc = (unsigned short int *)dx.reqdata;
	dx.szdesc = 0;
	writereg(CEP_CTRL_STAT, 0);
}

void usb_synchframe(void)
{
	dx.reqerror = 1;
}

void usb_getmaxlun(void)
{
	dx.reqdata[0] = 0x00;
	dx.pdesc = (unsigned short int *)dx.reqdata;
	dx.szdesc = 1;
	writereg(CEP_IRQ_ENB,(readreg(CEP_IRQ_ENB) | CEP_IRQ_ENB_INTOKENINT ));
	writereg(EP2_RSP_SC,(readreg(EP2_RSP_SC) | EP_RSP_SC_BUFFLUSH));
	writereg(EP1_RSP_SC,(readreg(EP1_RSP_SC) | EP_RSP_SC_BUFFLUSH));
	writereg(EP2_IRQ_ENB,(readreg(EP2_IRQ_ENB) | EP_IRQ_ENB_DATAPACKETRECVINTEN));
}

void usb_massreset(void)
{
	dx.pdesc = (unsigned short int *)dx.reqdata;
	dx.szdesc = 0;
	writereg(CEP_CTRL_STAT, 0);
}

void usb_default(void)
{
	dx.reqerror = 1;
}

void usb_updatedevice(void)
{
	switch(dx.setuppackage.bRequest)
	{
	case SET_ADDRESS:
		writereg(USB_ADDR, dx.setuppackage.wValue & 0xff);
		dx.devicests = dx.changests;
		break;
	case SET_CONFIGURATION:
		writereg(EP2_RSP_SC,(readreg(EP2_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP1_RSP_SC,(readreg(EP1_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP1_IRQ_ENB,(readreg(EP1_IRQ_ENB) | EP_IRQ_ENB_DATAPACKETRECVINTEN));
		writereg(EP4_RSP_SC,(readreg(EP4_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP3_RSP_SC,(readreg(EP3_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP3_IRQ_ENB,(readreg(EP3_IRQ_ENB) | EP_IRQ_ENB_DATAPACKETRECVINTEN));
	//	writereg(EP4_IRQ_ENB, (readreg(EP4_RSP_SC) |EP_IRQ_ENB_DATAINTOKENINTEN));
		writereg(EP6_RSP_SC,(readreg(EP6_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP5_RSP_SC,(readreg(EP5_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP5_IRQ_ENB,(readreg(EP5_IRQ_ENB) | EP_IRQ_ENB_DATAPACKETRECVINTEN));
		writereg(EP8_RSP_SC,(readreg(EP8_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP7_RSP_SC,(readreg(EP7_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP7_IRQ_ENB,(readreg(EP7_IRQ_ENB) | EP_IRQ_ENB_DATAPACKETRECVINTEN));
		writereg(EP10_RSP_SC,(readreg(EP10_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP9_RSP_SC,(readreg(EP9_RSP_SC) | EP_RSP_SC_EPTOGGLE));
		writereg(EP9_IRQ_ENB,(readreg(EP9_IRQ_ENB) | EP_IRQ_ENB_DATAPACKETRECVINTEN));
		break;
	case SET_INTERFACE:
		break;
	case BULK_MASS_STORAGE_RESET:
		break;
	default:
		break;
	}
	 
}

void usb_setupcommand(void)
{
	if (dx.cepsts & CEP_IRQ_STAT_SETUPPACKETINT)
	{
		dx.temp = readreg(SETUP1_0);
		dx.setuppackage.bmRequestType 	= dx.temp & 0xff;
		dx.setuppackage.bRequest		= (dx.temp & 0xff00) >> 8;
		dx.setuppackage.wValue			= readreg(SETUP3_2);
		dx.setuppackage.wIndex			= readreg(SETUP5_4);
		dx.setuppackage.wLength			= readreg(SETUP7_6);

		switch (dx.setuppackage.bRequest)
		{
		case GET_STATUS:
			usb_getstatus();
			break;
		case CLEAR_FEATURE:
			usb_clearfeature();
			break;
		case RSVD_1:
		case RSVD_2:
			usb_rsvd();
			break;
		case SET_FEATURE:
			usb_setfeature();
			break;
		case SET_ADDRESS:
			usb_setaddress();
			break;
		case GET_DESCRIPTOR:
			usb_getdescriptor();
			break;
		case SET_DESCRIPTOR:
			usb_setdescriptor();
			break;
		case GET_CONFIGURATION:
			usb_getconfiguration();
			break;
		case SET_CONFIGURATION:
			usb_setconfiguration();
			break;
		case GET_INTERFACE:
			usb_getinterface();
			break;
		case SET_INTERFACE:
			usb_setinterface();
			break;
		case SYNCH_FRAME:
			usb_synchframe();
			break;
		case GET_MAX_LUN:
			usb_getmaxlun();
			break;
		case BULK_MASS_STORAGE_RESET:
			usb_massreset();
		default:
			usb_default();
			break;
		}
	}
}

void usb_sentdescriptor(void)
{
	int i;
	int tmp;
	
	writereg(CEP_CTRL_STAT,(readreg(CEP_CTRL_STAT) | CEP_CTRL_STAT_FLUSH));
	while (readreg(CEP_CTRL_STAT)& CEP_CTRL_STAT_FLUSH) {}

	tmp = ((dx.szdesc % 2) == 0)?(dx.szdesc / 2):((dx.szdesc + 1) / 2);
	
	for (i = 0; i < tmp; i++)
	{
		dx.temp = ((dx.pdesc[i] & 0xff) << 8) | ((dx.pdesc[i] & 0xff00) >> 8);
		writereg(CEP_DATA_BUF, dx.temp);
	}
	writereg(IN_TRANSFR_CNT, dx.szdesc);
	
	dx.szdesc = 0;
	dx.pdesc = 0; 
}
void usb_bulkout1handler(void)
{
	int i;
	int tmp;
	
	writereg(EP3_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP5_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP7_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP9_RSP_SC, EP_RSP_SC_NAKSEND);	

	dx.transsize = readreg(EP1_AVAIL);
		
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
		
	for (i = 0; i < tmp; i++)
	{
		dx.transdata[i] = readreg(EP1_DATA_BUF);
	}	

	writereg(EP3_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP3_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP5_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP5_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP7_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP7_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP9_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP9_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}	
	
	writereg(EP2_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 
	
}

void usb_bulkin2handler(void)
{
	int i;
	int tmp;

	led2_blinkon(RATE_HIGH8);
	
	writereg(EP2_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));				
	while(readreg(EP2_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
		
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
		
	for (i = 0; i < tmp; i++)
	{
		writereg(EP2_DATA_BUF, dx.transdata[i]);
	}
	writereg(EP2_CNT, dx.transsize);
		
	writereg(EP1_IRQ_ENB, EP_IRQ_ENB_DATAPACKETRECVINTEN);	
	
	led2_forceon();
}

void usb_bulkout3handler(void)
{
	
#ifndef USB_CPUMODE_
#ifdef USB_NFC_CPUMODE_
	usb_rsem1_cooperation();	
#else
	usb_rsem1_nfc_cooperation();
#endif

#else	
	int i;
	int tmp;

	writereg(EP1_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP5_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP7_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP9_RSP_SC, EP_RSP_SC_NAKSEND);	

	dx.transsize = readreg(EP3_AVAIL);
		
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
		
	for (i = 0; i < tmp; i++)
	{
		dx.transdata[i] = readreg(EP3_DATA_BUF);
	}	

	writereg(EP1_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP1_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP5_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP5_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP7_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP7_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP9_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP9_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
		
	writereg(EP4_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 
	
#endif
}

void usb_bulkin4handler(void)
{
#ifndef USB_CPUMODE_
#ifdef USB_NFC_CPUMODE_
	rsdm1_usb_cooperation();
#else
	nfc_rsdm1_usb_cooperation();
#endif
#else
	int i;
	int tmp;
				
	writereg(EP4_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));				
	while(readreg(EP4_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
		
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
		
	for (i = 0; i < tmp; i++)
	{
		writereg(EP4_DATA_BUF, dx.transdata[i]);
	}
	writereg(EP4_CNT, dx.transsize);
		
	writereg(EP3_IRQ_ENB, EP_IRQ_ENB_DATAPACKETRECVINTEN);	
#endif
}

void usb_bulkout5handler(void)
{
	int i;
	int tmp;
		
	writereg(EP1_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP3_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP7_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP9_RSP_SC, EP_RSP_SC_NAKSEND);
	
	dx.transsize = readreg(EP5_AVAIL);

	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
	
	for (i = 0; i < tmp; i++)
	{
		dx.transdata[i] = readreg(EP5_DATA_BUF);
	}	

	writereg(EP1_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP1_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP3_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP3_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP7_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP7_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP9_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP9_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	
	writereg(EP6_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN);
}

void usb_bulkin6handler(void)
{
	int i;
	int tmp;
				
	writereg(EP6_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));				
	while(readreg(EP6_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
		
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
		
	for (i = 0; i < tmp; i++)
	{
		writereg(EP6_DATA_BUF, dx.transdata[i]);
	}
	writereg(EP6_CNT, dx.transsize);
		
	writereg(EP5_IRQ_ENB, EP_IRQ_ENB_DATAPACKETRECVINTEN);
}

void usb_bulkout7handler(void)
{
	int i;
	int tmp;

	writereg(EP1_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP3_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP5_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP9_RSP_SC, EP_RSP_SC_NAKSEND);

	dx.transsize = readreg(EP7_AVAIL);
	
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
	
	for (i = 0; i < tmp; i++)
	{
		dx.transdata[i] = readreg(EP7_DATA_BUF);
	}	

	writereg(EP1_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP1_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP3_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP3_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP5_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP5_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP9_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP9_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}	

	writereg(EP8_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 	
}	
	

void usb_bulkin8handler(void)
{
	int i;
	int tmp;
				
	writereg(EP8_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));				
	while(readreg(EP8_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
		
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
		
	for (i = 0; i < tmp; i++)
	{
		writereg(EP8_DATA_BUF, dx.transdata[i]);
	}
	writereg(EP8_CNT, dx.transsize);
		
	writereg(EP7_IRQ_ENB, EP_IRQ_ENB_DATAPACKETRECVINTEN);
}

void usb_bulkout9handler(void)
{
	int i;
	int tmp;
	
	writereg(EP1_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP3_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP5_RSP_SC, EP_RSP_SC_NAKSEND);
	writereg(EP7_RSP_SC, EP_RSP_SC_NAKSEND);
	
	dx.transsize = readreg(EP9_AVAIL);
	
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
	
	for (i = 0; i < tmp; i++)
	{
		dx.transdata[i] = readreg(EP9_DATA_BUF);
	}	
	
	writereg(EP1_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP1_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP3_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP3_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP5_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP5_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
	writereg(EP7_RSP_SC, EP_RSP_SC_BUFFLUSH | 0x2);
	while(readreg(EP7_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}	

	
	writereg(EP10_IRQ_ENB, EP_IRQ_ENB_DATAINTOKENINTEN); 
}

void usb_bulkin10handler(void)
{
	int i;
	int tmp;
				
	writereg(EP10_RSP_SC,(EP_RSP_SC_BUFFLUSH | 0x2));				
	while(readreg(EP10_RSP_SC) & EP_RSP_SC_BUFFLUSH) {}
		
	tmp = ((dx.transsize % 2) == 0)?(dx.transsize / 2):((dx.transsize + 1) / 2);
		
	for (i = 0; i < tmp; i++)
	{
		writereg(EP10_DATA_BUF, dx.transdata[i]);
	}
	writereg(EP10_CNT, dx.transsize);
		
	writereg(EP9_IRQ_ENB, EP_IRQ_ENB_DATAPACKETRECVINTEN);
}

void usb_protocolhandler(void)
{
	if (dx.cepsts & CEP_IRQ_STAT_USBERRORINT)
	{
		writereg(CEP_CTRL_STAT, 2);
	}
	else if (dx.cepsts & CEP_IRQ_STAT_SETUPPACKETINT )
	{		
		usb_setupcommand();
	}		
	else if(dx.cepsts & CEP_IRQ_STAT_DATAPACKETTRANSINT)
	{  
		writereg(CEP_CTRL_STAT, 0);
	}		
	else if( (dx.cepsts & CEP_IRQ_STAT_INTOKENINT) && (!(dx.cepsts & CEP_IRQ_STAT_STSCOMPINT)) )
	{	
		if (dx.reqerror == 1)
		{
			writereg(CEP_CTRL_STAT, CEP_CTRL_STAT_STALL);
		}
		usb_sentdescriptor();	

	}
	else if(dx.cepsts & CEP_IRQ_STAT_STSCOMPINT) 
	{

		writereg(CEP_CTRL_STAT,(readreg(CEP_CTRL_STAT) | CEP_CTRL_STAT_FLUSH));
		usb_updatedevice();			 
		
	} 
	else if((dx.cepsts & CEP_IRQ_STAT_OUTTOKENINT) || (dx.cepsts & CEP_IRQ_STAT_DATAPACKETRECVINT) )
	{ 
		if (dx.reqerror == 1)
		{
			writereg(CEP_CTRL_STAT, CEP_CTRL_STAT_STALL);
		}
		else
		{
			writereg(CEP_CTRL_STAT, 0);
		}
	} 
}

void usb_eventhandler(void)
{
	if (dx.usbsts & USB_IRQ_STAT_RSTSTS)
	{
		dx.changests = DEV_DEFAULT_STS;
		dx.devicests = DEV_DEFAULT_STS;
		if (readreg(USB_OPER) & USB_OPER_USBCURSPD)
		{
			// device settled in HighSpeed after reset
		}
		else
		{
			// device settled in FullSpeed after reset
		}
		writereg(EP1_RSP_SC,(readreg(EP1_RSP_SC) | EP_RSP_SC_BUFFLUSH));
		writereg(EP2_RSP_SC,(readreg(EP2_RSP_SC) | EP_RSP_SC_BUFFLUSH));
		writereg(EP2_IRQ_ENB,EP_IRQ_ENB_DATAPACKETRECVINTEN);
		writereg(EP3_RSP_SC,(readreg(EP3_RSP_SC) | EP_RSP_SC_BUFFLUSH));
		writereg(EP4_RSP_SC,(readreg(EP4_RSP_SC) | EP_RSP_SC_BUFFLUSH));
		writereg(EP3_IRQ_ENB,EP_IRQ_ENB_DATAPACKETRECVINTEN);
		writereg(USB_ADDR,0); 

	}
	else if (dx.usbsts & USB_IRQ_STAT_SUSPREQ)
	{
	
	}
	else if (dx.usbsts & USB_IRQ_STAT_RESUME)
	{
		
	}
}


























