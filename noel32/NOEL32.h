#ifndef __INCLUDE_REGISTER_H
#define __INCLUDE_REGISTER_H


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "config.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/**
 * Registers of LEON3
 */
#define ICR     ((volatile uint32_t *)0x08000020)
#define IRQREG  ((volatile uint32_t *)0x08000240)
#define ICS     ((volatile uint32_t *)0x08000520)
#define GMASK   ((volatile uint32_t *)0x08000218)
#define GCC     ((volatile uint32_t *)0x08000524)
#define GPOO    ((volatile uint32_t *)0x08000C00)
#define CLUE    ((volatile uint32_t *)0x20080000)
#define DSU     ((volatile uint32_t *)0x90000000)

#define DQSDLYSEL1  ((volatile uint32_t *)0x08000c00)
#define DQSDLYSEL2  ((volatile uint32_t *)0x08000c04)
 
/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

static inline void REGLOG_set(volatile uint32_t *addr, uint32_t val)
{
    ((volatile uint32_t *)addr)[0] = val;
    DBGPRINTF(" %8X => [%X]\n", val, (uint32_t)addr);
}

static inline uint32_t REGLOG_get(volatile uint32_t *addr)
{
    uint32_t result = ((volatile uint32_t *)addr)[0];
    DBGPRINTF("[%X]  => %8X\n", (uint32_t)addr,  result);
    return result;
}


#endif  /* __INCLUDE_REGISTER_H */
