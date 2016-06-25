/**
 * @file NAND.c
 *
 * @author fox.chao@gmail.com
 */

#include "NAND.h"
// FIXME: #include "ONFI.h"
#include "NFC.h"
#include "crypto.h"

struct cryptodma_t dmardma;

int NAND_read_request(int start, int cnt)
{
//    dmardma.data = 
    return 1;
}

int NAND_write_request(int start, int cnt)
{
    return 2;
}

int NAND_poll(int handle)
{
    switch(handle)
    {
        case 1:
            return 0;
        case 2:
            return 0;
    }
    return 0;
}

static unsigned int *dmaw_ptr;

/* A general routine for DMAW */
static PT_THREAD(NFC_DMAW_cpu(struct pt *pt))
{
    static uint32_t tmp;
    PT_BEGIN(pt);

    while (1)
    {
        PT_WAIT_UNTIL(pt, get_count(NFC_REG[NFC_DMAWFF_SLOT]));

        NFC_DMAWFFRD[0] = 2;    // pop
        NFC_DMAWFFRD[0] = 0;    // lower DOWRD
        tmp = NFC_DMAWFFRD[0];
        NFC_DMAWFFRD[0] = 1;    // upper DWORD
        tmp = NFC_DMAWFFRD[0];
    }

    PT_END(pt);
}

PT_THREAD(NAND_writing(struct pt *pt))
{
    static struct pt pt1;

    PT_BEGIN(pt);

//    dmardma.data = i_buff;
//    dmardma.length = cnt >> 3;
//    NFC_DMAR_request(cnt);

    PT_INIT(&pt1);

//    do {
//        NFC_DMAW_cpu(&pt1);
//    } while (dmawdma.length || !is_NFC_DMAW_done());

    NFC_DMAW_disable();

    PT_END(pt);
}

int NAND_init(void)
{
    ONFI_init();

    return 0;
}
