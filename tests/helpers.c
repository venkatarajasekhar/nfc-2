/**
 * @file misc.c
 *
 * Miscellaneous functions to support testing.
 *
 * @author fox.chao@gmail.com
 */

#include "helpers.h"

uint32_t fletcher32(uint16_t *data, int len)
{
     uint32_t sum1 = 0xffff, sum2 = 0xffff;

     while (len) {
         unsigned tlen = len > 360 ? 360 : len;
         len -= tlen;
         do {
             sum1 += *data++;
             sum2 += sum1;
         } while (--tlen);
         sum1 = (sum1 & 0xffff) + (sum1 >> 16);
         sum2 = (sum2 & 0xffff) + (sum2 >> 16);
     }
     /* Second reduction step to reduce sums to 16 bits */
     sum1 = (sum1 & 0xffff) + (sum1 >> 16);
     sum2 = (sum2 & 0xffff) + (sum2 >> 16);
     return sum2 << 16 | sum1;
}

void dump(void *addr, int cnt)
{
    for (int i = 0; i < (cnt >> 2); i++)
    {
    	if ((i % 4) == 0)
    	{
    	    DBGPRINTF("\n");
    	}
        DBGPRINTF("%08x ", ((uint32_t *)addr)[i]);
    }
    DBGPRINTF("\n");
}
