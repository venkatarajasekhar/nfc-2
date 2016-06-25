#include "leonstack.h"
#include "asmmacro.h"
#include "leon.h"

.global start, main, isr_handler

.section .text.start
start:
	flush
	set 0x10e0, %g1		! init UI
	mov %g1, %psr
	mov %g0, %wim
	mov %g0, %tbr
	mov %g0, %y
	nop
	nop

	mov 2, %g1
	mov %g1, %wim
	set 0x10e0, %g1
	mov %g1, %psr
	nop
	nop
	nop
	
	mov	0x1c6, %g1
	sethi %hi(0x90000000), %o0
	st	%g1, [%o0]
	nop
	nop
	nop

/*
	set 0x387, %g1
	sethi %hi(0x08000000), %o0
	add	%o0, 0x520, %o0
	st  %g1, [%o0]
*/

	set 0xffffffff, %g1
	sethi %hi(0x08000000), %o0
	add	%o0, 0x524, %o0
	st  %g1, [%o0]


	set 0x1, %g1
	sethi %hi(0x08000000), %o0
	add	%o0, 0x218, %o0
	st  %g1, [%o0]

	set 0x0001A000, %g1
	mov %g1, %asr17   
	ba 5f
	nop

_isr_ep: 

	rd %psr, %l0
	rd %tbr, %l6
	srl %l6, 4 ,%l6
	sub %l6, 0x10, %l7
	and %l7, 0xf, %l7
	rd %wim, %l3
	
	sub	%sp, PT_REGS_SZ, %l5
	SAVE_ALL
	sll %l7, SPARC_PSR_PIL_SHIFT,%o1
	or	%l0, %o1, %o1
    or	%o1, SPARC_PSR_PIL_MASK, %o1       
	wr	%o1, SPARC_PSR_ET_MASK, %psr

	mov	%l7, %o0
	
	call isr_handler
	nop
	nop
	nop	
	add	%sp, 0 , %o1		   ! pt_regs ptr
	
	RESTORE_ALL
	nop
	nop

_sys_ep: 
//	ta 1
	nop
	nop
	nop
	nop
	nop
	nop
	jmp %l2
	rett %l2+4
	jmp %l1
	rett %l2

_wp_ep: 
	ta 1
	nop
	nop
	jmp %l1
	rett %l2

_soft_ep: 
//	ta 0
	nop
	nop
	
	jmp %l2
	rett %l2+4
	jmp %l1
	rett %l2

5:	
	set _isr_ep, %g1
	mov %g1, %asr20

	set _sys_ep, %g1
	mov %g1, %asr21

	set _soft_ep, %g1
	mov %g1, %asr22

	set _wp_ep, %g1
	mov %g1, %asr23
	

clearram:	
    sethi %hi(__bss_start), %g2
    or    %g2, %lo(__bss_start), %g2  
    sethi %hi(_end), %g3
    or    %g3, %lo(_end), %g3        
    mov   %g0, %g1                  
	sub   %g3, %g2, %g3
zerobss:
	subcc  %g3, 4, %g3
	bge,a  zerobss
	st    %g0, [%g2+%g3]


7:	set STACK_RAM_END, %fp
	set STACK_RAM_END, %sp
	nop
	nop
	nop

	call main
	nop
	nop
	nop
	nop
	nop
	nop
	
	set 0x0800053c, %g1
	st %o0, [ %g1 ]
	nop
	nop
	
	set 3000, %g1
_b9:
	add %g1, -1, %g1
	cmp %g1, -1
	beq,a _b9
	nop
	
	set 0x08000218, %g1
	clr [ %g1 ]

	mov %g0, %asr19
	nop
	nop


