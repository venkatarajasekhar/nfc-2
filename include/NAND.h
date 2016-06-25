#ifndef __INCLUDE_NAND_H
#define __INCLUDE_NAND_H


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "config.h"
#include "pt/pt.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/


uint32_t fletcher32(uint16_t *data, uint32_t len);

/**
 * NAND_read_request function requests series of pages for reading
 *
 * @param start starting page
 * @param cnt   total page count
 * @return handle for this request
 */
int NAND_read_request(int start, int cnt);

/**
 * NAND_write_request function requests series of pages for writing
 *
 * @param start starting page
 * @param cnt   total page count
 * @return handle for this request
 */
int NAND_write_request(int start, int cnt);

/**
 * NAND_state function probe the processing state with the handle
 *
 * @param handle handle from request
 * @return state of executions <pre>
 *      0 - idle
 *      1 - processing
 * </pre>
 */
int NAND_state(int handle);

/**
 * NAND_release function releases the handle in use.
 *
 * @param handle handle from request
 * @return result <pre>
 *      0 - success
 *      others - states 
 * </pre>
 */
int NAND_state(int handle);

/**
 * NAND_processing function drives the NAND-related processes. Run it
 * frequently to make sure minimal waiting.
 */
PT_THREAD(NAND_processing(struct pt *pt));

int NAND_init(void);


#endif  /* __INCLUDE_NAND_H */
