#include "leonstack.h"
#include "winmacros.h"
        
/* Registers to not touch at all. */
#define t_psr        l0 /* Set by caller */
#define t_pc         l1 /* Set by caller */
#define t_npc        l2 /* Set by caller */
#define t_wim        l3 /* Set by caller */
#define t_twinmask   l4 /* Set at beginning of this entry routine. */
#define t_kstack     l5 /* Set right before pt_regs frame is built */
#define t_retpc      l6 /* If you change this, change winmacro.h header file */
#define t_systable   l7 /* Never touch this, could be the syscall table ptr. */
#define curptr       g6 /* Set after pt_regs frame is built */

    .text
	.align 4
	.globl	leonbare_trapsetup

leonbare_trapsetup:
	PT_STORE_ALL(t_kstack, t_psr, t_pc, t_npc)

	/* Trap from kernel with a window available.
	 * Just do it...
	 */
	jmpl	%t_retpc + 0x8, %g0	! return to caller
	 mov	%t_kstack, %sp		! jump onto new stack
        
        
