#ifndef __INCLUDE_CONFIG_H
#define __INCLUDE_CONFIG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "stdint.h"

#ifdef CONFIG_TARGET
    #ifdef CONFIG_DEBUG
        #include "opt/printf.h"
        #define DBGPRINTF   printf
    #else
        #define DBGPRINTF(fmt, ...)  do {} while (0)
    #endif
    #include "../noel32/NOEL32.h"
#else
    #include <stdio.h>
    #define DBGPRINTF   printf
#endif

#include <assert.h>


/****************************************************************************
 * Definitions
 ****************************************************************************/

#define kPAGE_BYTES 8192
#define kPAGE_PACK  8640

#if 0   // T=12
#define kT_DATA     12
#define kSEED_DATA  0x03F7  //  lookup (8192-4256)
#define kKLEN_DATA  256
#define kNLEN_DATA  266
#else   // T=16
#define kT_DATA     16
#define kSEED_DATA  0x01315 // lookup (8192-4304)
#define kKLEN_DATA  256
#define kNLEN_DATA  269
#endif

#define kT_AUX      12
#define kSEED_AUX   0x1141  // lookup (8192-256)
#define kKLEN_AUX   6
#define kNLEN_AUX   16

/****************************************************************************
 * Included Files
 ****************************************************************************/


/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/


#endif  /* __INCLUDE_CONFIG_H */
