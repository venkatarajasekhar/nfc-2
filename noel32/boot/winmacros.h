#ifndef _WINMACRO_
#define _WINMACRO_

#include "leon.h"
#include "leonstack.h"
#include "asmmacro.h"
        
#define PT_LOAD_ALL(base_reg, pt_psr, pt_pc, pt_npc) \
		ld      [%base_reg + PT_PSR], %pt_psr; \
        ld      [%base_reg + PT_PC], %pt_pc; \
        ld      [%base_reg + PT_NPC], %pt_npc;
 
    
      

#define PT_STORE_ALL(base_reg, pt_psr, pt_pc, pt_npc) \
        st      %pt_psr, [%base_reg + PT_PSR]; \
        st      %pt_pc,  [%base_reg + PT_PC]; \
        st      %pt_npc, [%base_reg + PT_NPC];

#endif

