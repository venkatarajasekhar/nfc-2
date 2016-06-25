#ifndef DEVICE_H_
#define DEVICE_H_

/********************************************************************************
 * Definitions
 ********************************************************************************/

/*
 * Register Group Summary of USB2.0 Device Controler
 */
#define OTG_BASE	0x11100000
/***********************************************************
 * Main Control Registers
 ***********************************************************/
#define	IRQ_STAT_L	(OTG_BASE + 0x00)	/* Interrupt Status Low Register */
#define	IRQ_STAT_H	(OTG_BASE + 0x04)	/* Interrupt Status High Register */
#define	IRQ_ENB_L	(OTG_BASE + 0x08)	/* Interrupt Enable Low Register */
#define	IRQ_ENB_H	(OTG_BASE + 0x0c)	/* Interrupt Enable High Register */

#define	IRQ_STAT_L_USBINT		(1 << 0)	/* 1 - USB Interrupt */
#define	IRQ_STAT_L_CTRLEPINT	(1 << 1)	/* 1 - Control Endpoint Interrupt */
#define	IRQ_STAT_L_INTEP_L		0xfffc	/* 2 - Interrupt for Endpoints 1 and 2 */
#define	IRQ_STAT_H_INTEP_H		0xffff

#define	IRQ_ENB_L_USBINTENABLE		(1 << 0)	/* 1 - USB Interrupt Enable*/
#define	IRQ_ENB_L_CTRLEPINTENABLE	(1 << 1)	/* 1 - Control Endpoint Interrupt Enable */
#define	IRQ_ENB_L_INTENABLEEP_L		0xfffc	/* 2 - Interrupt Enable for Endpints 1 and 2 */
#define	IRQ_ENB_H_INTENABLEEP_H		0xffff

/***********************************************************
 * USB Control Registers
 ***********************************************************/
#define USB_IRQ_STAT	(OTG_BASE + 0x10)	/* USB Interrupt Status Register */
#define	USB_IRQ_ENB		(OTG_BASE + 0x14)	/* USB Interrupt Enable Register */
#define	USB_OPER		(OTG_BASE + 0x18)	/* USB Operational Register */
#define	USB_FRAME_CNT	(OTG_BASE + 0x1c)	/* USB Frame Count Register */
#define	USB_ADDR		(OTG_BASE + 0x20)	/* USB Address Register */
#define	USB_TEST		(OTG_BASE + 0x24)	/* USB Test Mode Register */

#define	USB_IRQ_STAT_SOF		(1 << 0)	/* 1 - SOF */
#define	USB_IRQ_STAT_RSTSTS		(1 << 1)	/* 1 - Reset Status */
#define	USB_IRQ_STAT_RESUME		(1 << 2)	/* 1 - Resume */
#define	USB_IRQ_STAT_SUSPREQ	(1 << 3)	/* 1 - Suspend Request */
#define	USB_IRQ_STAT_HISPDSET	(1 << 4)	/* 1 - High Speed Settle */
#define USB_IRQ_STAT_CLCUSEINT	(1 << 6)	/* 1 - Clock Usable Interrupt */

#define	USB_IRQ_ENB_SOF			(1 << 0)	/* 1 - SOF */
#define	USB_IRQ_ENB_RSTSTS		(1 << 1)	/* 1 - Reset Status */
#define	USB_IRQ_ENB_RESUME		(1 << 2)	/* 1 - Resume */
#define	USB_IRQ_ENB_SUSREQ		(1 << 3)	/* 1 - Suspend Request */
#define	USB_IRQ_ENB_HISPDSET	(1 << 4)	/* 1 - High Speed Settle */
#define USB_IRQ_ENB_CLCUSEINTENB	(1 << 6)	/* 1 - Clock Usable Interrupt enable*/

#define	USB_OPER_GENRSM		(1 << 0)	/* 1 - Generate Resume */
#define	USB_OPER_USBHISPD	(1 << 1)	/* 1 - USB High Speed */
#define	USB_OPER_USBCURSPD	(1 << 2)	/* 1 - USB Current Speed */


#define	USB_FRAME_CNT_MICFRAMECNT	0x7		/* 3 - MICRO FRAME COUNTER */
#define	USB_FRAME_CNT_FRAMECNG		0x3FF8	/* 11 - FRAME COUNTER */ 

#define	USB_ADDR_OURADDR	0x7f	/* 7 - Our Address */


#define	USB_TEST_TMODE	0x7 	/* 3 - Test Mode Select */
/*
 * value		test
 * 000			Normal Operation
 * 001			Test_J
 * 010			Test_K
 * 011			Test_SE0_NAK
 * 100			Test_Packet
 * 101			Test_Force_Enable
 * 110			Rsv
 * 111			Rsv
 */

/*************************************************************
 * Control Endpoint Registers 
 * ***********************************************************/
#define	CEP_DATA_BUF		(OTG_BASE + 0x28)	/* Control-ep Data Buffer */
#define	CEP_CTRL_STAT		(OTG_BASE + 0x2c)	/* Control-ep Control and Status */
#define	CEP_IRQ_ENB			(OTG_BASE + 0x30)	/* Control-ep Interrupt Enable */
#define	CEP_IRQ_STAT		(OTG_BASE + 0x34)	/* Control-ep interrupt Status */
#define	IN_TRANSFR_CNT		(OTG_BASE + 0x38)	/* In-transfer data count */
#define	OUT_TRANSFR_CNT		(OTG_BASE + 0x3c)	/* Out-transfer data count */
#define	SETUP1_0			(OTG_BASE + 0x44)	/* Setup1 & Setup0 bytes */
#define	SETUP3_2			(OTG_BASE + 0x48)	/* Setup3 & Setup2 bytes */
#define	SETUP5_4			(OTG_BASE + 0x4c)	/* Setup5 & Setup4 bytes */
#define	SETUP7_6			(OTG_BASE + 0x50)	/* Setup7 & Setup6 bytes */
#define	CEP_START_ADDR		(OTG_BASE + 0x54)	/* Control-ep RAM Start Addr Register */
#define	CEP_END_ADDR		(OTG_BASE + 0x58)	/* Control-ep RAM End Addr Register */

#define	CEP_DATA_BUF_CTRLEPDATABUFA		0xffff	/* 16 - Control-ep Data Buffer */

#define	CEP_CTRL_STAT_NAKCLEAR			(1 << 0) /* 1 - NAK_CLEAR */
#define	CEP_CTRL_STAT_STALL				(1 << 1) /* 1 - STALL */
#define	CEP_CTRL_STAT_ZEROLEN			(1 << 2) /* 1 - ZEROLEN Bit */
#define	CEP_CTRL_STAT_FLUSH				(1 << 3) /* 1 - CEP-FLUSH Bit */

#define	CEP_IRQ_ENB_SETUPTOKENINT		(1 << 0) /* 1 - Setup token interrupt enable */
#define	CEP_IRQ_ENB_SETUPPACKETINT		(1 << 1) /* 1 - Setup Packet Interrupt */
#define	CEP_IRQ_ENB_OUTTOKENINT			(1 << 2) /* 1 - Out token interrupt */
#define	CEP_IRQ_ENB_INTOKENINT			(1 << 3) /* 1 - In token interrupt */
#define	CEP_IRQ_ENB_PINGTOKENINT		(1 << 4) /* 1 - Ping token interrupt */
#define	CEP_IRQ_ENB_DATAPACKETTRANSINT	(1 << 5) /* 1 - Data packet transmitted interrupt */
#define	CEP_IRQ_ENB_DATAPACKETRECVINT	(1 << 6) /* 1 - Data packet received interrupt */
#define	CEP_IRQ_ENB_NAKSENTINT			(1 << 7) /* 1 - NAK sent interrupt */
#define	CEP_IRQ_ENB_STALLSENTINT		(1 << 8) /* 1 - STALL sent interrupt */
#define	CEP_IRQ_ENB_USBERRORINT			(1 << 9) /* 1 - USB Error interrupt */
#define	CEP_IRQ_ENB_STSCOMPINT			(1 << 10)/* 1 - Status Completion interrupt */
#define	CEP_IRQ_ENB_BUFFULLINT			(1 << 11)/* 1 - Buffer Full interrupt */
#define	CEP_IRQ_ENB_BUFEMPINT			(1 << 12)/* 1 - Buffer Empty interrupt */

#define	CEP_IRQ_STAT_SETUPTOKENINT		(1 << 0)	/* 1 - Setup token interrupt */
#define	CEP_IRQ_STAT_SETUPPACKETINT		(1 << 1)	/* 1 - Setup packet interrupt */
#define	CEP_IRQ_STAT_OUTTOKENINT		(1 << 2)	/* 1 - Out token interrupt */
#define	CEP_IRQ_STAT_INTOKENINT			(1 << 3)	/* 1 - In token interrupt */
#define	CEP_IRQ_STAT_PINGTOKENINT		(1 << 4)	/* 1 - Ping token interrupt */
#define	CEP_IRQ_STAT_DATAPACKETTRANSINT	(1 << 5)	/* 1 - Data packet transmitted interrupt */
#define	CEP_IRQ_STAT_DATAPACKETRECVINT	(1 << 6)	/* 1 - Data packet received interrupt */
#define	CEP_IRQ_STAT_NAKSENTINT			(1 << 7)	/* 1 - NAK sent interrupt */
#define	CEP_IRQ_STAT_STALLSENTINT		(1 << 8)	/* 1 - STALL sent interrupt */
#define	CEP_IRQ_STAT_USBERRORINT		(1 << 9)	/* 1 - USB Error interrupt */
#define	CEP_IRQ_STAT_STSCOMPINT			(1 << 10)	/* 1 - Status Completion interrupt */
#define	CEP_IRQ_STAT_BUFFULLINT			(1 << 11)	/* 1 - Buffer Full interrupt */
#define	CEP_IRQ_STAT_BUFEMPINT			(1 << 12) 	/* 1 - Buffer Empty interrupt */

#define	IN_TRANSFR_CNT_INTRANSDATACNT	0xffff		/* 16 - In-transfer data count */
#define	OUT_TRANSFR_CNT_OUTTRANSDATACNT	0xffff		/* 16 - Out-transfer data count */

#define	SETUP1_0_SETUPBYTE01			0xffff	/* 16 - Setup byte0 [7:0] and byte1 [15:8] */
#define	SETUP3_2_SETUPBYTE23			0xffff	/* 16 - Setup byte3 [7:0] and byte2 [15:8] */
#define	SETUP5_4_SETUPBYTE45			0xffff	/* 16 - Setup byte5 [7:0] and byte4 [15:8] */
#define	SETUP7_6_SETUPBYTE67			0xffff	/* 16 - Setup byte7 [7:0] and byte6 [15:8] */

#define	CEP_START_ADDR_CEPSTARTADDR		0xffff	/* 16 - Cep start_address */
#define	CEP_END_ADDR_CEPENDADDR			0xffff	/* 16 - Cep end_address */


/****************************************************************
 * Non control Endpoint Registers
 * **************************************************************/
#define	EP1_DATA_BUF	(OTG_BASE + 0x64)	/* EP1 data register */
#define	EP1_IRQ_STAT	(OTG_BASE + 0x68)	/* EP1 interrupt register */
#define	EP1_IRQ_ENB		(OTG_BASE + 0x6c)	/* EP1 interrupt enable register */
#define	EP1_AVAIL		(OTG_BASE + 0x70)	/* EP1 available count register */
#define	EP1_RSP_SC		(OTG_BASE + 0x74)	/* EP1 Response Set/Clear register */
#define	EP1_MPS			(OTG_BASE + 0x78)	/* EP1 Maximum packet size register */
#define	EP1_CNT			(OTG_BASE + 0x7c)	/* EP1 Transfer count register */
#define	EP1_CFG			(OTG_BASE + 0x80)	/* EP1 configuration register */
#define	EP1_START_ADDR	(OTG_BASE + 0x84)	/* EP1 RAM start address register */
#define	EP1_END_ADDR	(OTG_BASE + 0x88)	/* EP1 RAM end address register */

#define	EP2_DATA_BUF	(OTG_BASE + 0x8c)	/* EP2 data register */
#define	EP2_IRQ_STAT	(OTG_BASE + 0x90)	/* EP2 interrupt register */
#define	EP2_IRQ_ENB		(OTG_BASE + 0x94)	/* EP2 interrupt enable register */
#define	EP2_AVAIL		(OTG_BASE + 0x98)	/* EP2 available count register */
#define	EP2_RSP_SC		(OTG_BASE + 0x9c)	/* EP2 Response Set/Clear register */
#define	EP2_MPS			(OTG_BASE + 0xa0)	/* EP2 Maximum packet size register */
#define	EP2_CNT			(OTG_BASE + 0xa4)	/* EP2 Transfer count register */
#define	EP2_CFG			(OTG_BASE + 0xa8)	/* EP2 configuration register */
#define	EP2_START_ADDR	(OTG_BASE + 0xac)	/* EP2 RAM start address register */
#define	EP2_END_ADDR	(OTG_BASE + 0xb0)	/* EP2 RAM end address register */

#define	EP3_DATA_BUF	(OTG_BASE + 0xb4)	/* EP3 data register */
#define	EP3_IRQ_STAT	(OTG_BASE + 0xb8)	/* EP3 interrupt register */
#define	EP3_IRQ_ENB		(OTG_BASE + 0xbc)	/* EP3 interrupt enable register */
#define	EP3_AVAIL		(OTG_BASE + 0xc0)	/* EP3 available count register */
#define	EP3_RSP_SC		(OTG_BASE + 0xc4)	/* EP3 Response Set/Clear register */
#define	EP3_MPS			(OTG_BASE + 0xc8)	/* EP3 Maximum packet size register */
#define	EP3_CNT			(OTG_BASE + 0xcc)	/* EP3 Transfer count register */
#define	EP3_CFG			(OTG_BASE + 0xd0)	/* EP3 configuration register */
#define	EP3_START_ADDR	(OTG_BASE + 0xd4)	/* EP3 RAM start address register */
#define	EP3_END_ADDR	(OTG_BASE + 0xd8)	/* EP3 RAM end address register */

#define	EP4_DATA_BUF	(OTG_BASE + 0xdc)	/* EP4 data register */
#define	EP4_IRQ_STAT	(OTG_BASE + 0xe0)	/* EP4 interrupt register */
#define	EP4_IRQ_ENB		(OTG_BASE + 0xe4)	/* EP4 interrupt enable register */
#define	EP4_AVAIL		(OTG_BASE + 0xe8)	/* EP4 available count register */
#define	EP4_RSP_SC		(OTG_BASE + 0xec)	/* EP4 Response Set/Clear register */
#define	EP4_MPS			(OTG_BASE + 0xf0)	/* EP4 Maximum packet size register */
#define	EP4_CNT			(OTG_BASE + 0xf4)	/* EP4 Transfer count register */
#define	EP4_CFG			(OTG_BASE + 0xf8)	/* EP4 configuration register */
#define	EP4_START_ADDR	(OTG_BASE + 0xfc)	/* EP4 RAM start address register */
#define	EP4_END_ADDR	(OTG_BASE + 0x100)	/* EP4 RAM end address register */

#define	EP5_DATA_BUF	(OTG_BASE + 0x104)	/* EP5 data register */
#define	EP5_IRQ_STAT	(OTG_BASE + 0x108)	/* EP5 interrupt register */
#define	EP5_IRQ_ENB		(OTG_BASE + 0x10c)	/* EP5 interrupt enable register */
#define	EP5_AVAIL		(OTG_BASE + 0x110)	/* EP5 available count register */
#define	EP5_RSP_SC		(OTG_BASE + 0x114)	/* EP5 Response Set/Clear register */
#define	EP5_MPS			(OTG_BASE + 0x118)	/* EP5 Maximum packet size register */
#define	EP5_CNT			(OTG_BASE + 0x11c)	/* EP5 Transfer count register */
#define	EP5_CFG			(OTG_BASE + 0x120)	/* EP5 configuration register */
#define	EP5_START_ADDR	(OTG_BASE + 0x124)	/* EP5 RAM start address register */
#define	EP5_END_ADDR	(OTG_BASE + 0x128)	/* EP5 RAM end address register */

#define	EP6_DATA_BUF	(OTG_BASE + 0x12c)	/* EP6 data register */
#define	EP6_IRQ_STAT	(OTG_BASE + 0x130)	/* EP6 interrupt register */
#define	EP6_IRQ_ENB		(OTG_BASE + 0x134)	/* EP6 interrupt enable register */
#define	EP6_AVAIL		(OTG_BASE + 0x138)	/* EP6 available count register */
#define	EP6_RSP_SC		(OTG_BASE + 0x13c)	/* EP6 Response Set/Clear register */
#define	EP6_MPS			(OTG_BASE + 0x140)	/* EP6 Maximum packet size register */
#define	EP6_CNT			(OTG_BASE + 0x144)	/* EP6 Transfer count register */
#define	EP6_CFG			(OTG_BASE + 0x148)	/* EP6 configuration register */
#define	EP6_START_ADDR	(OTG_BASE + 0x14c)	/* EP6 RAM start address register */
#define	EP6_END_ADDR	(OTG_BASE + 0x150)	/* EP6 RAM end address register */

#define	EP7_DATA_BUF	(OTG_BASE + 0x154)	/* EP7 data register */
#define	EP7_IRQ_STAT	(OTG_BASE + 0x158)	/* EP7 interrupt register */
#define	EP7_IRQ_ENB		(OTG_BASE + 0x15c)	/* EP7 interrupt enable register */
#define	EP7_AVAIL		(OTG_BASE + 0x160)	/* EP7 available count register */
#define	EP7_RSP_SC		(OTG_BASE + 0x164)	/* EP7 Response Set/Clear register */
#define	EP7_MPS			(OTG_BASE + 0x168)	/* EP7 Maximum packet size register */
#define	EP7_CNT			(OTG_BASE + 0x16c)	/* EP7 Transfer count register */
#define	EP7_CFG			(OTG_BASE + 0x170)	/* EP7 configuration register */
#define	EP7_START_ADDR	(OTG_BASE + 0x174)	/* EP7 RAM start address register */
#define	EP7_END_ADDR	(OTG_BASE + 0x178)	/* EP7 RAM end address register */

#define	EP8_DATA_BUF	(OTG_BASE + 0x17c)	/* EP8 data register */
#define	EP8_IRQ_STAT	(OTG_BASE + 0x180)	/* EP8 interrupt register */
#define	EP8_IRQ_ENB		(OTG_BASE + 0x184)	/* EP8 interrupt enable register */
#define	EP8_AVAIL		(OTG_BASE + 0x188)	/* EP8 available count register */
#define	EP8_RSP_SC		(OTG_BASE + 0x18c)	/* EP8 Response Set/Clear register */
#define	EP8_MPS			(OTG_BASE + 0x190)	/* EP8 Maximum packet size register */
#define	EP8_CNT			(OTG_BASE + 0x194)	/* EP8 Transfer count register */
#define	EP8_CFG			(OTG_BASE + 0x198)	/* EP8 configuration register */
#define	EP8_START_ADDR	(OTG_BASE + 0x19c)	/* EP8 RAM start address register */
#define	EP8_END_ADDR	(OTG_BASE + 0x1a0)	/* EP8 RAM end address register */

#define	EP9_DATA_BUF	(OTG_BASE + 0x1a4)	/* EP9 data register */
#define	EP9_IRQ_STAT	(OTG_BASE + 0x1a8)	/* EP9 interrupt register */
#define	EP9_IRQ_ENB		(OTG_BASE + 0x1ac)	/* EP9 interrupt enable register */
#define	EP9_AVAIL		(OTG_BASE + 0x1b0)	/* EP9 available count register */
#define	EP9_RSP_SC		(OTG_BASE + 0x1b4)	/* EP9 Response Set/Clear register */
#define	EP9_MPS			(OTG_BASE + 0x1b8)	/* EP9 Maximum packet size register */
#define	EP9_CNT			(OTG_BASE + 0x1bc)	/* EP9 Transfer count register */
#define	EP9_CFG			(OTG_BASE + 0x1c0)	/* EP9 configuration register */
#define	EP9_START_ADDR	(OTG_BASE + 0x1c4)	/* EP9 RAM start address register */
#define	EP9_END_ADDR	(OTG_BASE + 0x1c8)	/* EP9 RAM end address register */

#define	EP10_DATA_BUF	(OTG_BASE + 0x1cc)	/* EP10 data register */
#define	EP10_IRQ_STAT	(OTG_BASE + 0x1d0)	/* EP10 interrupt register */
#define	EP10_IRQ_ENB	(OTG_BASE + 0x1d4)	/* EP10 interrupt enable register */
#define	EP10_AVAIL		(OTG_BASE + 0x1d8)	/* EP10 available count register */
#define	EP10_RSP_SC		(OTG_BASE + 0x1dc)	/* EP10 Response Set/Clear register */
#define	EP10_MPS		(OTG_BASE + 0x1e0)	/* EP10 Maximum packet size register */
#define	EP10_CNT		(OTG_BASE + 0x1e4)	/* EP10 Transfer count register */
#define	EP10_CFG		(OTG_BASE + 0x1e8)	/* EP10 configuration register */
#define	EP10_START_ADDR	(OTG_BASE + 0x1ec)	/* EP10 RAM start address register */
#define	EP10_END_ADDR	(OTG_BASE + 0x1f0)	/* EP10 RAM end address register */

#define	EP_DATA_BUF_EPDATAREG	0xffff	/* 16 - Endpoint Data Register */

#define	EP_IRQ_STAT_BUFFULL				(1 << 0)	/* 1 - Buffer Full */
#define	EP_IRQ_STAT_BUFEMP				(1 << 1)	/* 1 - Buffer Empty */
#define	EP_IRQ_STAT_SHORTPACKETTRANSINT (1 << 2)	/* 1 - Short packet transferred interrupt */
#define	EP_IRQ_STAT_DATAPACKETTRANSINT	(1 << 3)	/* 1 - Data pakcet transmitted interrupt */
#define	EP_IRQ_STAT_DATAPACKETRECVINT	(1 << 4)	/* 1 - Data packet received interrupt */
#define	EP_IRQ_STAT_DATAOUTTOKENINT		(1 << 5)	/* 1 - Data OUT token interrupt */
#define	EP_IRQ_STAT_DATAINTOKENINT		(1 << 6)	/* 1 - Data IN token interrupt */
#define	EP_IRQ_STAT_PINGTOKENINT		(1 << 7)	/* 1 - PING token interrupt */
#define	EP_IRQ_STAT_USBNAKSENT			(1 << 8)	/* 1 - USB NAK Sent */
#define	EP_IRQ_STAT_USBSTALLSENT		(1 << 9)	/* 1 - USB STALL Sent */	
#define	EP_IRQ_STAT_NYETSENT			(1 << 10)	/* 1 - NYET sent */
#define	EP_IRQ_STAT_ERRSENT				(1 << 11)	/* 1 - ERR sent */

#define	EP_IRQ_ENB_BUFFULLINT				(1 << 0)	/* 1 - Buffer Full interrupt */
#define	EP_IRQ_ENB_BUFEMPINT				(1 << 1)	/* 1 - Buffer Empty Interrupt */
#define	EP_IRQ_ENB_SHORTPACKETTRANSINTEN	(1 << 2)	/* 1 - Short packet transferred interrupt */
#define	EP_IRQ_ENB_DATAPACKETTRANSINTEN		(1 << 3)	/* 1 - Data packet transmitted interrupt */
#define	EP_IRQ_ENB_DATAPACKETRECVINTEN		(1 << 4)	/* 1 - Data packet received interrupt */
#define	EP_IRQ_ENB_DATAOUTTOKENINTEN		(1 << 5)	/* 1 - Data OUT token interrupt enable */
#define	EP_IRQ_ENB_DATAINTOKENINTEN			(1 << 6)	/* 1 - Data IN token interrupt enable */
#define	EP_IRQ_ENB_PINGTOKENINTEN			(1 << 7)	/* 1 - PING token interrupt enable */
#define	EP_IRQ_ENB_USBNAKSENTINTEN			(1 << 8)	/* 1 - USB NAK sent interrupt enable */
#define	EP_IRQ_ENB_USBSTALLSENTINTEN		(1 << 9)	/* 1 - USB STALL sent interrupt enable */
#define	EP_IRQ_ENB_NYETINTEN				(1 << 10)	/* 1 - NYET interrupt enable */
#define	EP_IRQ_ENB_ERRINTEN					(1 << 11)	/* 1 - ERR interrupt */

#define	EP_AVAIL_CNT			0xfff	/* 12 - valid bytes in endpoint packet buffer */

#define	EP_RSP_SC_BUFFLUSH		(1 << 0)	/* 1 - buffer flush */
#define	EP_RSP_SC_MODE			0x6			/* 2 - mdoe */
#define	EP_RSP_SC_EPTOGGLE		(1 << 3)	/* 1 - endpoint toggle */
#define	EP_RSP_SC_EPHALT		(1 << 4)	/* 1 - endpoint halt */
#define	EP_RSP_SC_ZEROLENIN		(1 << 5)	/* 1 - zerolen in */
#define	EP_RSP_SC_PACKETEND		(1 << 6)	/* 1 - packet end */
#define	EP_RSP_SC_NAKSEND		(1 << 7)	/* 1 - nak send */

#define	EP_MPS_MAXPACKETSIZE	0xfff	/* 12 -  endpoint maximum packet size */

#define	EP_CNT_EPTRANSCNT		0xffff	/* 16 - total number of bytes to be sent */

#define	EP_CFG_EPVALID			(1 << 0) /* 1 - endpoint valid */
#define	EP_CFG_EPTYPE			0x6		/* 2 - endpoint type */
#define	EP_CFG_EPDIR			0x8 	/* 1 - endpoint direction */
#define	EP_CFG_EPNUM			0xf0 	/* 4 - endpoint number */
#define	EP_CFG_MULTFIELD		0x300	/* 2 - mult field */

#define	EP_START_ADDR_EPSTARTADDR	0xffff	/* 16 - ep start address */
#define	EP_END_ADDR_EPENDADDR		0xffff	/* 16 - ep end address */


#undef	EXTERN
#if defined(__cplusplus)
#define EXTERN	extern "C"
extern "C" {
#else
#define EXTERN	extern
#endif

EXTERN void device_init(void);
EXTERN void device_enableinterrupt(void);

	
#undef	EXTERN
#if defined(__cplusplus)
}
#endif

#endif /*DEVICE_H_*/
