#ifndef __INCLUDE_NFC_H
#define __INCLUDE_NFC_H


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "config.h"


/****************************************************************************
 * Definitions
 ****************************************************************************/

#define NFC_REG             ((volatile uint32_t *)0x20030000)
/*
 * From 0x20030000 to 0x200303FF
 * CPU issues write or read with the intended address
 */


/* Reset Register - Soft Reset to Modules and FIFO */
#define NFC_RESET           0
/* Byte 0 */
#define NFC_RESET_H2F       (1 << 0)    /* 1 - H2F (Host to Flash) Module Reset */
#define NFC_RESET_FTL       (1 << 1)    /* 1 - FTL Module Reset */
#define NFC_RESET_BCHE      (1 << 2)    /* 1 - BCH Encoder Module Reset */
#define NFC_RESET_FMC       (1 << 3)    /* 1 - FMC (Flash Memory Control) Module Reset */
#define NFC_RESET_FMI       (1 << 4)    /* 1 - FMI (Flash Memory Interface) Module Reset */
#define NFC_RESET_BCHD      (1 << 5)    /* 1 - BCH Decoder Module Reset */
#define NFC_RESET_F2H       (1 << 6)    /* 1 - F2H (Flash to Host) Module Reset */
    /* 7 Reserved */
/* Byte 1 */
#define NFC_RESET_DMAWFF    (1 << 8)    /* 1 - DMA Write FIFO Reset */
#define NFC_RESET_H2FFF     (1 << 9)    /* 1 - H2F FIFO Reset */
#define NFC_RESET_BCHEIFF   (1 << 10)   /* 1 - BCHE Input FIFO Reset */
#define NFC_RESET_CMDFF     (1 << 11)   /* 1 - CMD FIFO Reset */
#define NFC_RESET_FMIIFF    (1 << 12)   /* 1 - FMI Input FFIO Reset */
#define NFC_RESET_FMICPUFF  (1 << 13)   /* 1 - FMI CPU FIFO Reset */
#define NFC_RESET_FMIOFF    (1 << 14)   /* 1 - FMI Output FIFO Reset */
#define NFC_RESET_DMARFF    (1 << 15)   /* 1 - DMA Read FIFO Reset */
/* Byte 2,3 Reserved */


/* Enable Register - Module Enable */
#define NFC_ENABLE          1
/* Byte 0 */
#define NFC_ENABLE_DMAW     (1 << 0)    /* 0 - Enable DMA write */
#define NFC_ENABLE_AUX      (1 << 1)    /* 0 - Enable auxiliary data path */
#define NFC_ENABLE_FTLRD    (1 << 2)    /* 0 - Enable FTL RAM data to flash memory path */
#define NFC_ENABLE_H2F      (1 << 3)    /* 0 - Enable Host to flash data path */
#define NFC_ENABLE_BCHE     (1 << 4)    /* 0 - Enable BCH encoder */
#define NFC_ENABLE_BCHE_BYPASS  (1 << 5)    /* 0 - Bypass BCH encoder logic */
#define NFC_ENABLE_FMC      (1 << 6)    /* 0 - Enable FMC module */
#define NFC_ENABLE_RS		(1 << 7)	/* 0 - Enable auto read status register */
/* Byte 1 */
#define NFC_ENABLE_BCHD     (1 << 8)    /* 0 - Enable BCH decoder */
#define NFC_ENABLE_BCHD_BYPASS  (1 << 9)    /* 0 - Bypass BCH decoder logic */
#define NFC_ENABLE_F2H      (1 << 10)   /* 0 - Enable Flash to Host path */
#define NFC_ENABLE_FTLWR    (1 << 11)   /* 0 - Enable flash memory to FTL RAM data path */
#define NFC_ENABLE_MERGE    (1 << 12)   /* 0 - Enable host data and flash data merge operation */
#define NFC_ENABLE_DMAR     (1 << 13)   /* 0 - Enable DMA read */
/* 14 Reserved */
#define NFC_ENABLE_LOOPBACK (1 << 15)   /* 0 - Enable loop back from BCHE to BCHD
                                                (This register is for diagnostic use only) */
/* Byte 2,3 Reserved */


/* Mode Register */
#define NFC_MODE            2
/* Byte 0 */
#define NFC_MODE_AMBA_32    (1 << 0)    /* RO - REG bus is 32 bit */
#define NFC_MODE_SYNC       (1 << 1)    /* 0 - Enable ONFI synchronous interface */
#define NFC_MODE_D16        (1 << 2)    /* 0 - Enable ONFI 16-bit bus */
/* 7:3 Reserved */
/* Byte 1 Reserved */
/* Byte 2 - Polarity Control */
#define NFC_MODE_LOW_CLE1   (1 << 16)   /* set to 1 for active low CLE1 */
#define NFC_MODE_LOW_CLE2   (1 << 17)   /* set to 1 for active low CLE2 */
#define NFC_MODE_LOW_ALE1   (1 << 18)   /* set to 1 for active low ALE1 */
#define NFC_MODE_LOW_ALE2   (1 << 19)   /* set to 1 for active low ALE2 */
#define NFC_MODE_LOW_RE1    (1 << 20)   /* set to 1 for active low RE1 */
#define NFC_MODE_LOW_RE2    (1 << 21)   /* set to 1 for active low RE2 */
#define NFC_MODE_LOW_WE1    (1 << 22)   /* set to 1 for active low WE1 */
#define NFC_MODE_LOW_WE2    (1 << 23)   /* set to 1 for active low WE2 */
/* Byte 3 - Polarity Control */
#define NFC_MODE_LOW_CE1    (1 << 24)   /* set to 1 for active low CE11 to CE14 */
#define NFC_MODE_LOW_CE2    (1 << 25)   /* set to 1 for active low CE21 to CE24 */
/* 31:26 Reserved */


/* Flash Control Register */
#define NFC_FCTL            3
/* Byte 0 */
#define NFC_FCTL_GROUP2     (1 << 0)    /* 0 - GROUP2 Select
                                                Select 1st group (CLE1, ALE1, RE1, WE1)
                                                or 2nd group (CLE2, ALE2, RE2, WE2) */
#define NFC_FCTL_LANE2      (1 << 1)    /* 0 - LANE2 Select
                                                Select 1st flash chip data bus
                                                or 2nd flash chip data bus */


/* Interrupt Source and Clear Register */
#define NFC_INTR_SRC        4
/* R/W - Write 1 to corresponding bits to clear the interrupt source. */
/* Byte 0 */
#define NFC_INTR_SRC_DMAW_DONE              (1 << 0)
#define NFC_INTR_SRC_H2F_DONE               (1 << 1)
#define NFC_INTR_SRC_BCHE_DONE              (1 << 2)
#define NFC_INTR_SRC_DMAR_DONE              (1 << 3)
#define NFC_INTR_SRC_BCHD_ERROR_EXCEED      (1 << 4)
#define NFC_INTR_SRC_FTL_DONE               (1 << 5)
/* Byte 1 */
#define NFC_INTR_SRC_ONFI_STATUS_FAIL       (1 << 8)
#define NFC_INTR_SRC_ONFI_STATUS_FAILC      (1 << 9)
#define NFC_INTR_SRC_ONFI_STATUS_SRRDY      (1 << 10)
#define NFC_INTR_SRC_ONFI_STATUS_SRARDY     (1 << 11)
#define NFC_INTR_SRC_ONFI_RDY_FALLING_EDGE  (1 << 12)
#define NFC_INTR_SRC_ONFI_RDY_RISING_EDGE   (1 << 13)
/* 15:14 Reserved */


/* Interrupt Enable Register */
#define NFC_INTR_ENB        5
/* 0 - Set 1 to enable corresponding interrupt to CPU */
/* Byte 0 */
/* Byte 1 */
/* Byte 2 */
/* Byte 3 */


/* DAMW Transfer Count Register */
#define NFC_DMAW_ENDCNT     6
/* Byte 0,1 */
    /* 0 - DMA Write Count
        - DMA is terminated upon reaching this count.
        - The unit of transfer is either 64 bit or 32 bits depending on AMBA_32 bit in MODE Register
        - Programmed with (total transfer count - 1)
        - When this register is set to 0, the DMA engine will NOT be terminated by end of count */


/* DAMR Transfer Count Register */
#define NFC_DMAR_ENDCNT     7
/* Byte 0,1 */
    /* 0 - DMA Read Count
        - DMA is terminated upon reaching this count.
        - The unit of transfer is either 64 bit or 32 bits depending on AMBA_32 bit in MODE Register
        - Programmed with (total transfer count - 1)
        - When this register is set to 0, the DMA engine will NOT be terminated by end of count */


/* AUX Transfer Count */
#define NFC_AUX_ENDCNT      8
/* Byte 0,1 */
    /* 0 - Auxiliary Data Transfer Count
            Programmed with (total transfer count -1)
            The unit of transfer is either 64 bit or 32 bits
            depending on AMBA_32 bit in MODE Register */

/* H2F Transfer Count */
#define NFC_H2F_ENDCNT      9
    /* 0 - Host to Flash Transfer Count
            Programmed with (total transfer count -1)
            The unit of transfer is either 64 bit or 32 bits
            depending on AMBA_32 bit in MODE Register */


/* Starting Address Register */
#define NFC_AUX_SAR         10
/* Byte 0,1 */
    /* 0 - AUX Data Starting Address
            The starting address of auxiliary data in the
            FTL RAM to be written to flash memory */


/* LIST Starting Address Register */
#define NFC_LIST_SAR        11
/* Byte 0,1 */
    /* 0 - DMA List Starting Address
            The address of a list which contains the
            address of FTL RAM tables which may have
            scattered location in the FTL RAM */


/* Merge Table Register */
#define NFC_MERGE           12
/* Byte 0,1 */
//#define WNFC_SECTOR    NFC_MERGE
//    /* 0 - Each bit represents a sector of a page.
//            Set the corresponding bit to 1 if the
//            flash memory data is to be kept and
//            not to be overwritten by new data
//            from host. A sector is 512 bytes. */
///* Byte 2,3 */
//#define CNFC_SPP       (NFC_MERGE + 2)
//    /* 0 - Number of sector per page */


/* Main Data K & N Parameters Register */
#define NFC_KN_DATA             0x10
/* Byte 0,1 */
#define NFC_KN_DATA_KLEN(x)     (x)
    /* 0 - BCH Data Length Register
            The BCH encoder encodes K unit of data and
            then append ECC parity after the data. The
            "unit" is defined as 16 bits or 2 bytes. For
            typical data K is either (512 bytes/2) or (256
            bytes/2). The programmed value is the actual
            value minus 1. */
/* Byte 2,3 */
#define NFC_KN_DATA_NLEN(x)     (x << 16)
    /* 0 - BCH Data Total Length Register
            The N units of data equal to K units of data
            plus the ECC parity units. The programmed
            value is the actual value minus 1. */


/* Main Data T and SEED Parameters Register */
#define NFC_TS_DATA             0x11
/* Byte 0 [4:0] */
#define NFC_TS_DATA_BCH_ECR(x)  (x)
    /* 0 - BCH Error Capability Register
            BCH error capability is from 2 to 16
            even numbers of errors. Thus T can be 2, 4, 6, ...,
            16. Since we use a polynomial degree of 13,
            we designate a vale M=13. The number of
            parity bits equal to M*T. For 16 error case,
            the parity bits are m*16 = 208 bits or 26 bytes.
            For other cases, the parity bits will be rounded
            up to be byte aligned. */
/* Byte 1 - Reserved */
/* Byte 2,3 */
#define NFC_TS_DATA_BCH_DSR(x)  (x << 16)
    /* 0 - BCH Decoder Seed Register
            Depending on K and T value there is a seed
            value to be used by BCH decoder to search
            for error. This register will be further defined
            by a separate table. */


/* Auxiliary Data K & N Parameters Register */
#define NFC_KN_AUX              0x12
/* Byte 0,1 */
#define NFC_KN_AUX_KLEN(x)      (x)
    /* 0 - BCH Data Length Register
            The BCH encoder encodes K unit of data and
            then append ECC parity after the data. The
            "unit" is defined as 16 bits or 2 bytes. For
            typical data K is either (512 bytes/2) or (256
            bytes/2). The programmed value is the actual
            value minus 1. */
/* Byte 2,3 */
#define NFC_KN_AUX_NLEN(x)      (x << 16)
    /* 0 - BCH Data Total Length Register
            The N units of data equal to K units of data
            plus the ECC parity units. The programmed
            value is the actual value minus 1. */


/* Auxiliary Data T and SEED Parameters Register */
#define NFC_TS_AUX              0x13
/* Byte 0 */
#define NFC_TSAUX_BCH_ECR(x)    (x)
    /* 0 - BCH Error Capability Register
            BCH error capability is from 2 to 16
            even numbers of errors. Thus T can be 2, 4, 6, ...,
            16. Since we use a polynomial degree of 13,
            we designate a vale M=13. The number of
            parity bits equal to M*T. For 16 error case,
            the parity bits are m*16 = 208 bits or 26 bytes.
            For other cases, the parity bits will be rounded
            up to be byte aligned. */
/* Byte 1 - reserved */
/* Byte 2,3 */
#define NFC_TSAUX_BCH_DSR(x)    (x << 16)
    /* 0 - BCH Decoder Seed Register
            Depending on K and T value there is a seed
            value to be used by BCH decoder to search
            for error. This register will be further defined
            by a separate table. */


/* Status Register */
#define NFC_STA0                0x20
/* Byte 0,1 */
#define NFC_STA0_DMAW_DONE      (1 << 0)    /* RO - DMA Write Done
                                                    Write DMA completion flag. */
#define NFC_STA0_BCHE_DONE      (1 << 1)    /* RO - BCHE Done
                                                    BCH encoder completion flag. */
#define NFC_STA0_DMAR_DONE      (1 << 2)    /* RO - DMA Read Done
                                                    Read DMA completion flag. */
#define NFC_STA0_ERROR_EXCEED   (1 << 3)    /* RO - Error Exceed Limit Register
                                                    Number of errors exceed the BCH correction
                                                    capability set at BCH Error Capability
                                                    Register */
    /* 15:4 Reserved */
/* Byte 2,3 */
#define NFC_STA0_CMDFF_2ND_READ     (1 << 16)   /* RO - CMDFF Second Read
                                                    Due to clock domain difference CPU must
                                                    issue read command twice to read an entry
                                                    from the command FIFO. The first command
                                                    must be issued when this bit is low and the
                                                    second command must be issued when this
                                                    register is high.
                                                    (This register is for diagnostic use only) */
#define NFC_STA0_CMDPTR_2ND_READ    (1 << 17)   /* RO - CMDPTR Second Read
                                                    Due to clock domain difference CPU must
                                                    issue read command twice to read an entry
                                                    from the command pointer. The first
                                                    command must be issued when this bit is low
                                                    and the second command must be issued
                                                    when this register is high.
                                                    (This register is for diagnostic use only) */
#define NFC_STA0_CMDTBL_2ND_READ (1 << 18)   /* RO - CMDTBL Second Read
                                                    Due to clock domain difference CPU must
                                                    issue read command twice to read an entry
                                                    from the command table. The first command
                                                    must be issued when this bit is low and the
                                                    second command must be issued when this
                                                    register is high.
                                                    (This register is for diagnostic use only) */
    /* 31:19 Reserved */


#define NFC_DMAWFF_SLOT     0x21
/* Byte 0,1 */
    /* RO - DMA Write FIFO Slot Count
            CPU reads this register to know how many
            slots are available in the DMA Write FIFO
            that it can write. */

/* Byte 2,3 */
    /* RO - DMA Read FIFO Data Count
            CPU reads this register to know how many
            data are available in the DMA Write FIFO
            that it can read. */


#define NFC_DMARFF_SLOT     0x22
/* Byte 0,1 */
    /* RO - DMA Read FIFO Slot Count
            CPU reads this register to know how many
            slots are available in the DMA Read FIFO
            that it can write. */

/* Byte 2,3 */
    /* RO - DMA Read FIFO Data Count
            CPU reads this register to know how many
            data are available in the DMA Read FIFO
            that it can read. */


#define NFC_CMDFF_SLOT      0x23
/* Byte 0,1 */
    /* RO - CMDFF FIFO Slot Count
            CPU reads this register to know how many
            slots are available in the CMDFF FIFO that it
            can write. CPU writes flash memory access
            commands and non-DMA write data to this
            FIFO for NFC to write to flash memory. */
/* Byte 2,3 */
    /* RO - CMDFF FIFO Data Count
            CPU reads this register to know how many
            data are available in the CMDFF FIFO that it
            can read. */


#define NFC_FMICFF_SLOT     0x24
/* Byte 0,1 - Reserved */
/* Byte 2,3 */
    /* RO - FMICFF FIFO Data Count
            CPU reads this register to know how many
            data are available in the FMICFF FIFO that it
            can read. Non-DMA flash memory read data
            is store in FMICFF FIFO for CPU to read. */


/* STATE Register */
#define NFC_STATE           0x25
/* Byte 0 */
    /* RO - DMAW State
            DMA Write state machine
            Return value of 0x01 indicates the END of state machine.
            The corresponding ENABLE must be cleared for the state machine to
            start again. */
/* Byte 1 */
    /* RO - MUX State
            Host to Flash data path state machine
            (This register is for diagnostic use only) */
/* Byte 2 */
    /* RO - DMAR State
            DMA Read state machine
            Return value of 0x01 indicates the END of state machine.
            The corresponding ENABLE must be cleared for the state machine to
            start again. */
/* Byte 3 */
    /* RO - FTL State
            FTL RAM data path state machine
            (This register is for diagnostic use only) */


#define NFC_RS_CMD			0x31
/* 0 - Automatic ONFI Status Command
        This register is to be programmed with 0x00000700 */

#define NFC_RS_NOP			0x32
/* 0 - Automatic ONFI Status WHR Delay
        This register is to be programmed with 0x0000yy84
        where yy depends on ONFI mode and clock speed */

#define NFC_RS_GET			0x33
/* 0 - Automatic ONFI Status Read
        This register is to be programmed with 0x000000A2 */

#define NFC_RS_WHR          0x34
/* 0 - Automatic ONFI Status WHR Delay
        This register is to be programmed with 0x0000zz84
        where zz depends on ONFI mode and clock speed */


#define NFC_CMDFF           ((volatile uint32_t *)0x20034000)
/**
    1. CPU read RG_CMDFF_DSLOT
    2. CPU issues write to this address
    3. Repeat step 2 as necessary

[3:0] 0 - OpCode
        0: ASYNC. CMD
        1: ASYNC. ADDR
        2: ASYNC. GET
        3: ASYNC. PUT
        4: NOP
        5: (Not used)
        6: FORCE
        7: (Not used)
        8: SYNC. CMD
        9: SYNC. ADDR
        10: SYNC. GET
        11: SYNC. PUT
        15-12: (Not used)
[4] 0 - DMA PATH
        0: Non-DMA
        1: DMA
[5] 0 - DIRECTION
        0: From Flash memory
        1: To Flash Memory
[6] 0 - WAIT ENABLE
        If [17]=1, Wait for ONFI_RDY
            Wait till ONFI_RDY is high and then proceed to next command
        If [18]=1, Wait for SRRDY
            Wait till SRRDY is high and then proceed to next command
        If [19]=1, Wait for SRARDY
            Wait till SRARDY is high and then proceed to next command
[7] 0 - REPEAT ENABLE
        1: Repeat Current Command
            Repeat count is from [21:8]
[15:8] 0
    - As Repeat Count [7:0]
    or
    - As Singal Value for FORCE command
        CE1x means the chip select for the 1st flash memory chip,
        and CE2x are for the 2nd flash memory chip.
        For each flash chip both sets of control signal (CLE1, CLE2, etc.)
        can exist depending on the flash memory package type.
        When [16]= 0
            [8]:  value for CE11 pin
            [9]:  set 1 to disable bit [8]
            [10]: value for CE12 pin
            [11]: set 1 to disable for bit [10]
            [12]: value for CE13 pin
            [13]: set 1 to disable for bit [12]
            [14]: value for CE14 pin
            [15]: set 1 to disable for bit [14]
        When [16]= 1 and [23]= 0
            [8]:  value for CLE1 pin
            [9]:  set 1 to disable bit [8]
            [10]: value for ALE1 pin
            [11]: set 1 to disable for bit [10]
            [12]: value for RE1 pin
            [13]: set 1 to disable for bit [12]
            [14]: value for WE1 pin
            [15]: set 1 to disable for bit [14]
        When [16]= 1 and [23]= 1
            [8]:  value for CLE2 pin
            [9]:  set 1 to disable bit [8]
            [10]: value for ALE2 pin
            [11]: set 1 to disable for bit [10]
            [12]: value for RE2 pin
            [13]: set 1 to disable for bit [12]
            [14]: value for WE2 pin
            [15]: set 1 to disable for bit [14]
        [21:16] 0 - Repeat Count [12:8]
[22] 0 - AUX Select
        0: NFC external data (main data)
        1: NFC internal data (auxiliary data)
[23] 0 - GROUP2 Select
        Select LUN 1 signals (CLE1, ALE1, RE1, WE1) or LUN 2 signals (CLE2, ALE2, RE2, WE2)
[24] 0 - LANE2 Select
        (Not used in current design)
*/
#define ONFI_OP_CMD               0
#define ONFI_OP_ADDR              1
#define ONFI_OP_GET               2
#define ONFI_OP_PUT               3
#define ONFI_OP_NOP               4
#define ONFI_OP_FORCE             6
#define ONFI_OP_SYNC_CMD          8
#define ONFI_OP_SYNC_ADDR         9
#define ONFI_OP_SYNC_GET         10
#define ONFI_OP_SYNC_PUT         11
#define ONFI_USE_DMA           0x10
#define ONFI_FROM_FLASH           0
#define ONFI_TO_FLASH          0x20
#define ONFI_WAIT              0x40
#define ONFI_REPEAT_COUNT      0x80
#define ONFI_FORCE_CARW    0x010000
#define ONFI_WAIT_RDY      0x020000
#define ONFI_WAIT_SRRDY    0x040000
#define ONFI_WAIT_SRARDY   0x080000
#define ONFI_AUX_DATA      0x400000
#define ONFI_SEL_GROUP2    0x800000
#define ONFI_SEL_LANE2    0x1000000

#define NFC_CMDFFRD 		((volatile uint32_t *)0x20034400)
/*
 * 1. CPU read RG_CMDFF_DSLOT
 * 2. CPU issues dummy write to this address and then issues a read to this address
 * 3. Repeat step 2 as necessary
 */


#define NFC_CMDPTR		    ((volatile uint32_t *)0x20034800)
/*
    From 0x20034800 to 0x2003483C, totally 16 entries
    CPU issues write with the intended address

[4:0] 0 - Point to starting location in the CMDTBL
            For example:
            ADDR command has OpCode=1 so it looks
            up entry 1 of the CMDPTR. In the entry 1 of
            CMDPTR it has the starting location in the of
            the ADDR microcode sequence in the CMDTBL. */

#define NFC_CMDPTRRD        ((volatile uint32_t *)0x20034C00)
/*
 * From 0x20034C00 to 0x20034C3C, totally 16 entries
 * CPU issues dummy write to the intended address and then issues a read to the address
 */


#define NFC_CMDTBL          ((volatile uint32_t *)0x20035000)
/*
    From 0x20035000 to 0x2003507C, totally 32 entries
    CPU issues write with the intended address

[0] 0 - Enable edge to toggle on CLE
[1] 0 - Enable edge to toggle on ALE
[2] 0 - Enable edge to toggle on RE
[3] 0 - Enable edge to toggle on WE
[4] 0 - Set Signal Value (used by FORCE command)
[5] 0 - Reserved
[6] 0 - To Flash Memory
[7] 0 - Last entry of a command
[15:0] Repeat count for current CMFTBL entry
        - After the repeat count then next CMDTBL
            entry will be executed. It effectively controls
            the signal pulse width and delays between
            signal edges.
        - Programmed with actual value -- 1
*/

#define NFC_CMDTBLRD        ((volatile uint32_t *)0x20035400)
/*
 * From 0x20035400 to 0x2003547C, totally 32 entries
 * CPU issues dummy write to the intended address and then issues a read to the address
 */


/* Reserved for 0x20035800 */

#define NFC_FMICFFRD        ((volatile uint32_t *)0x20035C00)
/*
 * 1. CPU reads FMICFF_DSLOT
 * 2. CPU issues dummy write to this address and then issues a read to this address
 * 3. Repeat step 2 as necessary
 */


#define NFC_DMAWFF          ((volatile uint32_t *)0x20036000)
/*
 * 1. CPU read RG_DMAWFF_DSLOT
 * 2. CPU issues write with the intended address
 * 3. Repeat step 2 as necessary
 */

#define NFC_DMAWFFRD        ((volatile uint32_t *)0x20036400)
/*
 * 0x20036400, 0x20036404, 0x20036408
 * 1. CPU reads DMAWFF_DSLOT
 * 2. CPU issues dummy write to 0x20036400
 *    and then issues a read to 0x20036404 to read lower 32-bit data
 *          OR issues a read to 0x20036408 to read upper 32-bit data
 * 3. Repeat step 2 as necessary
 */


#define NFC_DMARFF          ((volatile uint32_t *)0x20036800)
/*
 * 1. CPU reads DMARFF_DSLOT
 * 2. CPU issues write with the intended address
 * 3. Repeat step 2 as necessary
 */

#define NFC_DMARFFRD        ((volatile uint32_t *)0x20036C00)
/* 0x20036C00, 0x20036C04, 0x20036C08
 * 1. CPU reads DMARFF_DSLOT
 * 2. CPU issues dummy write to 0x20036400
 *    and then issues a read to 0x20036404 to read lower 32-bit data
 *          OR issues a read to 0x20036408 to read upper 32-bit data
 * 3. Repeat step 2 as necessary
 */


#define NFC_FTL          ((volatile uint32_t *)0x2003C000)
/* From 0x2003C000 to 0x2003CFFF
 * CPU issues write or read with the intended address
 */


/**
 * operators for accessing NFC_XXX_SLOT members
 */
#define get_slot(x)     (x & 0xffff)
#define get_count(x)    (x >> 16)

/**
 * CMDFF_SLOT_check(cnt) checks if NFC_CMDFF_SLOT is enough for comping ONFI commands.
 */
#if 0
static inline void CMDFF_SLOT_check(uint32_t cnt)
{
    while (get_slot(NFC_REG[NFC_CMDFF_SLOT]) < cnt)
        DBGPRINTF("slot not enough!\n");
}
#else
    #define CMDFF_SLOT_check(cnt)   {}
#endif

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

extern int32_t cmdff_log;
 
#define DBGCMDFF(x)   do { cmdff_log = x; } while(0) 

/**
 * Declarations for NFC_command
 */
static inline void NFC_command(uint32_t val)
{
    if (cmdff_log) DBGPRINTF("C.%8x\n", val);
    NFC_CMDFF[0] = val;
}

static inline void NFC_onfi(uint16_t cmd)
{
    NFC_command((uint32_t)(ONFI_OP_CMD | ONFI_TO_FLASH | ((cmd & 0xff) << 8)));
}

static inline void NFC_1addr(uint8_t addr)
{
    NFC_command((uint32_t)(ONFI_OP_ADDR | ONFI_TO_FLASH | (addr << 8)));
}

static inline void NFC_2addr(uint16_t addr)
{
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | ((addr & 0xff) << 8));       // LSB first
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | (addr & 0xff00));
}

static inline void NFC_3addr(uint32_t addr)
{
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | ((addr & 0xff) << 8));       // LSB first
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | (addr & 0xff00));
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | ((addr & 0xff0000) >> 8));
}

static inline void NFC_4addr(uint32_t addr)
{
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | ((addr & 0xff) << 8));       // LSB first
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH |  (addr & 0xff00));
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | ((addr & 0xff0000) >> 8));
    NFC_command(ONFI_OP_ADDR | ONFI_TO_FLASH | ((addr & 0xff000000) >> 16));
}

static inline void NFC_get(uint16_t cnt)
{
    NFC_command((uint32_t)(ONFI_OP_GET | ONFI_REPEAT_COUNT | ONFI_FROM_FLASH | ((cnt - 1) << 8)));
}

static inline void NFC_dma_get(uint16_t cnt, uint32_t flag)
{
    NFC_command((uint32_t)(ONFI_OP_GET | ONFI_REPEAT_COUNT | ONFI_FROM_FLASH | ONFI_USE_DMA | flag | ((cnt - 1) << 8)));
}

static inline void NFC_put(void *addr, int16_t cnt)
{
    uint8_t *adr8 = (uint8_t *)addr;

//    CMDFF_SLOT_check(cnt);
    while (cnt--) {
        NFC_command((uint32_t)(ONFI_OP_PUT | ONFI_TO_FLASH | (*adr8++ << 8)));
    }
}

static inline void NFC_dma_put(int16_t cnt, uint32_t flag)
{
    NFC_command((uint32_t)(ONFI_OP_PUT | ONFI_REPEAT_COUNT | ONFI_TO_FLASH | ONFI_USE_DMA | flag | ((cnt - 1) << 8)));
}

static inline void NFC_nop(int16_t cnt, int32_t flag)
{
    NFC_command((uint32_t)(ONFI_OP_NOP | ONFI_REPEAT_COUNT | flag | (cnt << 8)));
}

static inline void NFC_wait(uint32_t flag)
{
    NFC_command((uint32_t)(ONFI_OP_NOP | ONFI_WAIT | flag));
}

static inline void NFC_force(uint32_t val)
{
    NFC_command(ONFI_OP_FORCE | val);
}


/**
 * Declarations for NFC_DMAW
 */
static inline int32_t is_NFC_DMAW_in_use(void)
{
    return (NFC_REG[NFC_ENABLE] & NFC_ENABLE_DMAW); 
}

static inline void NFC_DMAW_enable(void)
{
    // enable BT_DMAW_ENB
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAW;
}

static inline void NFC_DMAW_request(uint32_t length)
{
    // enable BT_DMAW_ENB
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAW;

    // write in DMAW_ENDCNT ((length for 64bits) - 1)
    NFC_REG[NFC_DMAW_ENDCNT] = (length >> 3) - 1;
}

static inline void NFC_DMAW_disable(void)
{
    NFC_REG[NFC_INTR_SRC] &= NFC_INTR_SRC_DMAW_DONE;    // clear
    NFC_REG[NFC_ENABLE]   &= ~NFC_ENABLE_DMAW; 
}

static inline int32_t is_NFC_DMAW_done()
{
    return (NFC_REG[NFC_INTR_SRC] & NFC_INTR_SRC_DMAW_DONE);
}


/**
 * Declarations for NFC_DMAR
 */
static inline int32_t is_NFC_DMAR_in_use(void)
{
    return (NFC_REG[NFC_ENABLE] & NFC_ENABLE_DMAR); 
}

static inline void NFC_DMAR_enable(uint32_t length)
{
    // enable BT_DMAR_ENB
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAR;
}

static inline void NFC_DMAR_request(uint32_t length)
{
    // enable BT_DMAR_ENB
    NFC_REG[NFC_ENABLE] |= NFC_ENABLE_DMAR;

    // write in DMAR_ENDCNT ((length for 64bits) - 1)
    NFC_REG[NFC_DMAR_ENDCNT] = (length >> 3) - 1;
}

static inline void NFC_DMAR_disable(void)
{
    NFC_REG[NFC_INTR_SRC] &= NFC_INTR_SRC_DMAR_DONE;    // clear
    NFC_REG[NFC_ENABLE]   &= ~NFC_ENABLE_DMAR;
}

static inline int32_t is_NFC_DMAR_done()
{
    return (NFC_REG[NFC_INTR_SRC] & NFC_INTR_SRC_DMAR_DONE);
}


uint16_t NFC_FMICFF_read(void);
void NFC_init(void);


/**
 * FTLRAM should be an array for us to easily visit each entry.
 */
#define FTLRAM  ((volatile uint32_t *)0x20020000);

static inline void NFC_FTL_access_off(void)
{
    ICS[0] &= (unsigned)(~(0x20));
}

static inline void NFC_FTL_access_on(void)
{
    ICS[0] |= 0x20;
}


#endif  /* __INCLUDE_NFC_H */
