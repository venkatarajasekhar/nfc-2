#ifndef _INCLUDE_LEON3_h
#define _INCLUDE_LEON3_h


#define ASI_LEON3_CACHEMISS 1
#define ASI_LEON3_SYSCTRL   0x02
#define ASI_LEON3_DFLUSH    0x11

#define ASI_LEON3_SYSCTRL_ICFG		0x08
#define ASI_LEON3_SYSCTRL_DCFG		0x0c
#define ASI_LEON3_SYSCTRL_CFG_SNOOPING (1<<27)
#define ASI_LEON3_SYSCTRL_CFG_SSIZE(c) (1<<((c>>20)&0xf))

  

 
#endif /* !_INCLUDE_LEON3_h */
/* end of include file */


