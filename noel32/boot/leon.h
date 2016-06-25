#ifndef _INCLUDE_LEON_h
#define _INCLUDE_LEON_h
 
/* psr defines */
#define SPARC_PSR_WIN_MASK  0x0000001f   /* bit   0-4 */
#define SPARC_PSR_ET_MASK   0x00000020   /* bit   5 */
#define SPARC_PSR_PS_MASK   0x00000040   /* bit   6 */
#define SPARC_PSR_S_MASK    0x00000080   /* bit   7 */
#define SPARC_PSR_PIL_MASK  0x00000F00   /* bits  8 - 11 */
#define SPARC_PSR_EF_MASK   0x00001000   /* bit  12 */
#define SPARC_PSR_EC_MASK   0x00002000   /* bit  13 */
#define SPARC_PSR_ICC_MASK  0x00F00000   /* bits 20 - 23 */
#define SPARC_PSR_VER_MASK  0x0F000000   /* bits 24 - 27 */
#define SPARC_PSR_IMPL_MASK 0xF0000000   /* bits 28 - 31 */
#define SPARC_PSR_PIL_SHIFT 8
  
#endif /* !_INCLUDE_LEON_h */
/* end of include file */

