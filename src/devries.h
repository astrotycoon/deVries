#ifndef COMMON_H_
#define COMMON_H_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef DEVRIES_INLINING
#define DV_INLINE inline
#else
#define DV_INLINE
#endif

#ifndef NULL
#define NULL 0
#endif

#endif
