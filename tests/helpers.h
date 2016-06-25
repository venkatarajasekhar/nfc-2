#ifndef __INCLUDE_HELPERS_H
#define __INCLUDE_HELPERS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "config.h"


/****************************************************************************
 * Definitions
 ****************************************************************************/


/****************************************************************************
 * Included Files
 ****************************************************************************/


/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

uint32_t fletcher32(uint16_t *data, int len);
void dump(void *addr, int cnt);


#endif  /* __INCLUDE_HELPERS_H */
