
#include "leonstack.h"
#include "winmacros.h"
                        
/* Registers to not touch at all. */
#define t_psr     l0
#define t_pc      l1
#define t_npc     l2
#define t_wim     l3
#define twin_tmp1 l4
#define glob_tmp  g4
#define curptr    g6

        .text
	.align 4
	.globl	leonbare_trapreturn

leonbare_trapreturn:
     
 	PT_LOAD_ALL(sp, t_psr, t_pc, t_npc)
	wr	%t_psr, 0x0, %psr ! enable nesting again, clear ET
	nop; nop; nop  

	jmp	%t_pc
	rett	%t_npc

