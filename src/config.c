#include "config.h"

#include <string.h>

uint32_t fw_page_bot;
uint32_t fw_page_top;

/* some standard C functions for the target only. */
#ifdef TARGET

void *memset(void *buff, int ch, size_t count)
{
    unsigned char *buff8 = (unsigned char *)buff;

    while (count--) {
        *buff8++ = ch;
    }

    return buff;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;
    
    while (count--) {
        *dst8++ = *src8++;
    }

    return dest;
}

#endif
