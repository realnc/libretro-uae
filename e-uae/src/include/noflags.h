#ifndef NOFLAGS_H
#define NOFLAGS_H

/* Undefine everything that will *set* flags. Note: Leave *reading*
   flags alone ;-). We assume that nobody does something like 
   SET_ZFLG(a=b+c), i.e. expect side effects of the macros. That would 
   be a stupid thing to do when using macros.
*/

/* Gwenole Beauchesne pointed out that CAS and CAS2 use flag_cmp to set
   flags that are then used internally, and that thus the noflags versions
   of those instructions were broken. Oops! 
   Easy fix: Leave flag_cmp alone. It is only used by CMP* and CAS* 
   instructions. For CAS*, noflags is a bad idea. For CMP*, which has
   setting flags as its only function, the noflags version is kinda pointless,
   anyway. 
   Note that this will only work while using the optflag_* routines ---
   as we do on all (one ;-) platforms that will ever use the noflags
   versions, anyway.
   However, if you try to compile without optimized flags, the "SET_ZFLAG"
   macro will be left unchanged, to make CAS and CAS2 work right. Of course,
   this is contrary to the whole idea of noflags, but better be right than
   be fast.

   Another problem exists with one of the bitfield operations. Once again,
   one of the operations sets a flag, and looks at it later. And the CHK2
   instruction does so as well. For those, a different solution is possible.
   the *_ALWAYS versions of the SET_?FLG macros shall remain untouched by 
   the redefinitions in this file.
   Unfortunately, they are defined in terms of the macros we *do* redefine.
   So here comes a bit of trickery....
*/
#define NOFLAGS_CMP 0


#undef SET_CFLG
#define SET_CFLG(x, y) do {uae_u32 dummy=(y); } while (0)
#undef SET_VFLG
#define SET_VFLG(x, y) do {uae_u32 dummy=(y); } while (0)
#undef SET_NFLG
#define SET_NFLG(x, y) do {uae_u32 dummy=(y); } while (0)
#undef SET_XFLG
#define SET_XFLG(x, y) do {uae_u32 dummy=(y); } while (0)


#undef SET_NFLG_ALWAYS
static __inline__ void SET_NFLG_ALWAYS(void* a, uae_u32 x)
{
    SET_NFLG(a, x);  /* This has not yet been redefined */
}

#undef SET_CFLG_ALWAYS
static __inline__ void SET_CFLG_ALWAYS(void* a, uae_u32 x)
{
    SET_CFLG(a, x);  /* This has not yet been redefined */
}

#undef CPUFUNC
#define CPUFUNC(x) x##_nf

#ifndef OPTIMIZED_FLAGS
#undef SET_ZFLG
#define SET_ZFLG(x, y) do {uae_u32 dummy=(y); } while (0)
#endif

#undef CLEAR_CZNV
#define CLEAR_CZNV
#undef IOR_CZNV
#define IOR_CZNV(x, y) do {uae_u32 dummy=(y); } while (0)
#undef SET_CZNV
#define SET_CZNV(x, y) do {uae_u32 dummy=(y); } while (0)
#undef COPY_CARRY
#define COPY_CARRY 

#ifdef  optflag_testl
#undef  optflag_testl
#endif

#ifdef  optflag_testw
#undef  optflag_testw
#endif

#ifdef  optflag_testb
#undef  optflag_testb
#endif

#ifdef  optflag_addl
#undef  optflag_addl
#endif

#ifdef  optflag_addw
#undef  optflag_addw
#endif

#ifdef  optflag_addb
#undef  optflag_addb
#endif

#ifdef  optflag_subl
#undef  optflag_subl
#endif

#ifdef  optflag_subw
#undef  optflag_subw
#endif

#ifdef  optflag_subb
#undef  optflag_subb
#endif

#if NOFLAGS_CMP
#ifdef  optflag_cmpl
#undef  optflag_cmpl
#endif

#ifdef  optflag_cmpw
#undef  optflag_cmpw
#endif

#ifdef  optflag_cmpb
#undef  optflag_cmpb
#endif
#endif


#define optflag_testl(v,x) \
do { } while(0)

#define optflag_testw(v,x) \
do { } while(0)

#define optflag_testb(v,x) \
do { } while(0)

//  __asm__ __volatile__ ("addl %k1,%k0\n\t" \
//			: "=r" (v) : "rmi" (s), "0" (d) : "cc"); \

#define optflag_addl(x,v, s, d) do { \
	v = s + d; \
    } while (0)

//  __asm__ __volatile__ ("addw %w1,%w0\n\t" \
//			: "=r" (v) : "rmi" (s), "0" (d) : "cc"); \

#define optflag_addw(x,v, s, d) do { \
	v = s + d; \
    } while (0)

//  __asm__ __volatile__ ("addb %b1,%b0\n\t" \
//			: "=q" (v) : "qmi" (s), "0" (d) : "cc"); \

#define optflag_addb(x,v, s, d) do { \
	v = s +  d; \
    } while (0)

//  __asm__ __volatile__ ("subl %k1,%k0\n\t" \
//			: "=r" (v) : "rmi" (s), "0" (d) : "cc"); \

#define optflag_subl(regs, v, s, d) do { \
	(v) = (d) - (s); \
    } while (0)


#define optflag_subw(regs, v, s, d) do { optflag_subl(regs, v, s, d); } while (0)
#define optflag_subb(regs, v, s, d) do { optflag_subl(regs, v, s, d);} while (0)

//#define optflag_subw(x,v, s, d) do { \
//  __asm__ __volatile__ ("subw %w1,%w0\n\t" \
//			: "=r" (v) : "rmi" (s), "0" (d) : "cc"); \
//    } while (0)

//#define optflag_subb(x,v, s, d) do { \
//  __asm__ __volatile__ ("subb %b1,%b0\n\t" \
//			: "=q" (v) : "qmi" (s), "0" (d) : "cc"); \
//    } while (0)

#if NOFLAGS_CMP
/* These are just for completeness sake */
#define optflag_cmpl(s, d) \
do {  __asm__ __volatile__ ("cmpl %k0,%k1\n\t" \
			:: "rmi" (s), "r" (d) : "cc"); } while(0)

#define optflag_cmpw(s, d) \
do {  __asm__ __volatile__ ("cmpw %w0,%w1\n\t" \
			:: "rmi" (s), "r" (d) : "cc");  } while(0)

#define optflag_cmpb(s, d) \
do {  __asm__ __volatile__ ("cmpb %b0,%b1\n\t" \
			:: "qmi" (s), "q" (d) : "cc"); } while(0)
#endif

#endif
