/*
    Copyright (C) 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef FLINT_H
#define FLINT_H

#ifndef FLINT_NOSTDIO
# include <stdio.h>
#endif

#ifndef FLINT_NOSTDARG
# include <stdarg.h>
#endif

#if defined(__CYGWIN__)
# include <sys/param.h>
#endif

#include <limits.h>
#include <gmp.h>
#ifdef BUILDING_FLINT
# include "config.h"
#else
# include "flint-config.h"
#endif

#if FLINT_USES_GC
# include "gc.h"
#endif

#ifdef FLINT_WANT_ASSERT
# include <assert.h>
#endif

#ifndef FLINT_DLL
# define FLINT_DLL
#endif

#ifndef alloca
# ifdef __GNUC__
#  define alloca __builtin_alloca
# else
#  ifdef _MSC_VER
#   include <malloc.h>
#   define alloca _alloca
#  else
#   include <alloca.h> /* We assume then that you have alloca.h */
#  endif
# endif
#endif

#ifndef __GNUC__
# define __asm__     asm
# define __inline__  inline
#endif

#ifdef va_start
# define FLINT_HAVE_VA_LIST
#endif

#if defined(FILE)       || defined(_FILE_DEFINED) || defined(__DEFINED_FILE) \
 || defined(H_STDIO)    || defined(_H_STDIO)      || defined(_STDIO_H)       \
 || defined(_STDIO_H_)  || defined(__STDIO_H)     || defined(__STDIO_H__)    \
 || defined(__STDIO__)
# define FLINT_HAVE_FILE
#endif

#ifdef FLINT_HAVE_CONSTANT_P
# define FLINT_CONSTANT_P __builtin_constant_p
#else
# define FLINT_CONSTANT_P(x) 0
#endif

#ifdef FLINT_INLINES_C
# define FLINT_INLINE
#else
# define FLINT_INLINE static __inline__
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* flint version number */

#define __FLINT_VERSION 3
#define __FLINT_VERSION_MINOR 0
#define __FLINT_VERSION_PATCHLEVEL 0
#define FLINT_VERSION "3.0.0-dev"
#define __FLINT_RELEASE_NUM(a,b,c) ((a)*10000 + (b)*100 + (c))
#define __FLINT_RELEASE __FLINT_RELEASE_NUM(__FLINT_VERSION, __FLINT_VERSION_MINOR, __FLINT_VERSION_PATCHLEVEL)

#if __GNU_MP_VERSION < 5
#error GMP 5.0.0 or MPIR 2.6.0 or later are required
#endif

FLINT_DLL extern char flint_version[];

struct __FLINT_FILE;
typedef struct __FLINT_FILE FLINT_FILE;

#define ulong mp_limb_t
#define slong mp_limb_signed_t
#define flint_bitcnt_t ulong

#ifdef FLINT_WANT_ASSERT
#define FLINT_ASSERT(param) assert(param)
#else
#define FLINT_ASSERT(param)
#endif

#include "longlong.h"

#if defined(__GNUC__)
# define FLINT_FORCE_INLINE static __attribute__((always_inline)) __inline__
# define FLINT_STATIC_NOINLINE static __attribute__((noinline))
# define FLINT_UNUSED(x) UNUSED_ ## x __attribute__((unused))
# define FLINT_SET_BUT_UNUSED(x) x __attribute__((unused))
# define FLINT_NORETURN __attribute__ ((noreturn))
# define FLINT_CONST __attribute__ ((const))
# define FLINT_WARN_UNUSED __attribute__((warn_unused_result))
# define FLINT_UNLIKELY(x) __builtin_expect((x),0)
# define FLINT_LIKELY(x) __builtin_expect((x),1)
# define FLINT_PUSH_OPTIONS _Pragma("GCC push_options")
# define FLINT_POP_OPTIONS _Pragma("GCC pop_options")
# define FLINT_OPTIMIZE_NESTED_3(part) _Pragma(#part)
# define FLINT_OPTIMIZE_NESTED_2(part) FLINT_OPTIMIZE_NESTED_3(GCC optimize part)
# define FLINT_OPTIMIZE_NESTED_1(part) FLINT_OPTIMIZE_NESTED_2(#part)
# define FLINT_OPTIMIZE(x) FLINT_OPTIMIZE_NESTED_1(x)
#else
# define __attribute__(x)
# define FLINT_FORCE_INLINE static __inline__
# define FLINT_STATIC_NOINLINE static
# define FLINT_UNUSED(x) x
# define FLINT_SET_BUT_UNUSED(x) x
# define FLINT_WARN_UNUSED
# define FLINT_NORETURN
# define FLINT_CONST
# define FLINT_UNLIKELY(x) (x)
# define FLINT_LIKELY(x) (x)
# define FLINT_PUSH_OPTIONS
# define FLINT_POP_OPTIONS
# define FLINT_OPTIMIZE(x)
#endif

#if FLINT_USES_TLS
# if defined(__GNUC__)
#  define FLINT_TLS_PREFIX __thread
# elif __STDC_VERSION__ >= 201112L
#  define FLINT_TLS_PREFIX _Thread_local
# elif defined(_MSC_VER)
#  define FLINT_TLS_PREFIX __declspec(thread)
# else
#  error "thread local prefix defined in C11 or later"
# endif
#else
# define FLINT_TLS_PREFIX
#endif

#if defined(_LONG_LONG_LIMB)
# define WORD_FMT "%ll"
# define WORD_WIDTH_FMT "%*ll"
# define WORD(xx) (xx##LL)
# define UWORD(xx) (xx##ULL)
# ifndef FLINT_NO_WORDMAC
#  define UWORD_MAX ULLONG_MAX
#  define UWORD_MIN ULLONG_MIN
#  define WORD_MAX LLONG_MAX
#  define WORD_MIN LLONG_MIN
# endif
#else
# define WORD_FMT "%l"
# define WORD_WIDTH_FMT "%*l"
# define WORD(xx) (xx##L)
# define UWORD(xx) (xx##UL)
# ifndef FLINT_NO_WORDMAC
#  define UWORD_MAX ULONG_MAX
#  define UWORD_MIN ULONG_MIN
#  define WORD_MAX LONG_MAX
#  define WORD_MIN LONG_MIN
# endif
#endif

#if GMP_LIMB_BITS == 64
# define FLINT_BITS 64
# define FLINT_D_BITS 53
# define FLINT64 1
#else
# define FLINT_BITS 32
# define FLINT_D_BITS 31
#endif

void * flint_malloc(size_t size);
void * flint_realloc(void * ptr, size_t size);
void * flint_calloc(size_t num, size_t size);
void flint_free(void * ptr);

typedef void (*flint_cleanup_function_t)(void);
void flint_register_cleanup_function(flint_cleanup_function_t cleanup_function);
void flint_cleanup(void);
void flint_cleanup_master(void);

void __flint_set_memory_functions(void *(*alloc_func) (size_t),
     void *(*calloc_func) (size_t, size_t), void *(*realloc_func) (void *, size_t),
                                                              void (*free_func) (void *));

void __flint_get_memory_functions(void *(**alloc_func) (size_t),
     void *(**calloc_func) (size_t, size_t), void *(**realloc_func) (void *, size_t),
                                                              void (**free_func) (void *));

FLINT_NORETURN void flint_abort(void);

/* If user want to set change abort function from `abort' */
void flint_set_abort(FLINT_NORETURN void (*func)(void));

int flint_get_num_threads(void);
void flint_set_num_threads(int num_threads);
void _flint_set_num_workers(int num_workers);
int flint_set_num_workers(int num_workers);
void flint_reset_num_workers(int max_workers);
int flint_set_thread_affinity(int * cpus, slong length);
int flint_restore_thread_affinity(void);

FLINT_CONST double flint_test_multiplier(void);

typedef struct
{
    gmp_randstate_t gmp_state;
    int gmp_init;
    mp_limb_t __randval;
    mp_limb_t __randval2;
} flint_rand_s;

typedef flint_rand_s flint_rand_t[1];

FLINT_INLINE
void flint_randinit(flint_rand_t state)
{
   state->gmp_init = 0;
#if FLINT64
    state->__randval = UWORD(13845646450878251009);
    state->__randval2 = UWORD(13142370077570254774);
#else
    state->__randval = UWORD(4187301858);
    state->__randval2 = UWORD(3721271368);
#endif
}

FLINT_INLINE
void flint_randseed(flint_rand_t state, ulong seed1, ulong seed2)
{
   state->__randval = seed1;
   state->__randval2 = seed2;
}

FLINT_INLINE
void flint_get_randseed(ulong * seed1, ulong * seed2, flint_rand_t state)
{
   *seed1 = state->__randval;
   *seed2 = state->__randval2;
}


FLINT_INLINE
void _flint_rand_init_gmp(flint_rand_t state)
{
    if (!state->gmp_init)
    {
        gmp_randinit_default(state->gmp_state);
        state->gmp_init = 1;
    }
}

FLINT_INLINE
void flint_randclear(flint_rand_t state)
{
    if (state->gmp_init)
        gmp_randclear(state->gmp_state);
}

FLINT_INLINE
flint_rand_s * flint_rand_alloc(void)
{
    return (flint_rand_s *) flint_malloc(sizeof(flint_rand_s));
}

FLINT_INLINE
void flint_rand_free(flint_rand_s * state)
{
    flint_free(state);
}

/* defined in ulong_extras, but used throughout the test code */
ulong n_randint(flint_rand_t, ulong);
ulong n_randtest(flint_rand_t);
ulong n_randtest_not_zero(flint_rand_t);

#if FLINT_USES_GC
#define FLINT_GC_INIT() GC_init()
#else
#define FLINT_GC_INIT()
#endif

#define FLINT_TEST_INIT(xxx) \
   flint_rand_t xxx; \
   FLINT_GC_INIT(); \
   flint_randinit(xxx)

#define FLINT_TEST_CLEANUP(xxx) \
   flint_randclear(xxx); \
   flint_cleanup_master();

#define FLINT_MAX(x, y) ((x) > (y) ? (x) : (y))
#define FLINT_MIN(x, y) ((x) > (y) ? (y) : (x))
#define FLINT_ABS(x) ((slong)(x) < 0 ? (-(x)) : (x))
#define FLINT_SIGN_EXT(x) (-(ulong)((slong)(x) < 0))
#define FLINT_SGN(x) ((0 < (slong)(x)) - ((slong)(x) < 0))

#define MP_PTR_SWAP(x, y) \
    do { \
        mp_limb_t * __txxx; \
        __txxx = x; \
        x = y; \
        y = __txxx; \
    } while (0)

#define SLONG_SWAP(A, B)    \
    do {                    \
        slong __t_m_p_ = A; \
        A = B;              \
        B = __t_m_p_;       \
    } while (0)

#define ULONG_SWAP(A, B)    \
    do {                    \
        ulong __t_m_p_ = A; \
        A = B;              \
        B = __t_m_p_;       \
    } while (0)

#define MP_LIMB_SWAP(A, B)      \
    do {                        \
        mp_limb_t __t_m_p_ = A; \
        A = B;                  \
        B = __t_m_p_;           \
    } while (0)

#define DOUBLE_SWAP(A, B)    \
    do {                     \
        double __t_m_p_ = A; \
        A = B;               \
        B = __t_m_p_;        \
    } while (0)

#define PTR_SWAP(T, A, B)    \
    do {                    \
        T* __t_m_p_ = A; \
        A = B;              \
        B = __t_m_p_;       \
    } while (0)

#define r_shift(in, shift) \
    ((shift == FLINT_BITS) ? WORD(0) : ((in) >> (shift)))

#define l_shift(in, shift) \
    ((shift == FLINT_BITS) ? WORD(0) : ((in) << (shift)))

/* Beware when using the unsigned return value in signed arithmetic */
static __inline__
mp_limb_t FLINT_BIT_COUNT(mp_limb_t x)
{
   mp_limb_t zeros = FLINT_BITS;
   if (x) zeros = flint_clz(x);
   return FLINT_BITS - zeros;
}

#define FLINT_FLOG2(k)  (FLINT_BIT_COUNT(k) - 1)

#define FLINT_CLOG2(k)  FLINT_BIT_COUNT((k) - 1)

#define flint_mpn_zero(xxx, nnn) \
    do \
    { \
        slong ixxx; \
        for (ixxx = 0; ixxx < (nnn); ixxx++) \
            (xxx)[ixxx] = UWORD(0); \
    } while (0)

#define flint_mpn_copyi(xxx, yyy, nnn) \
   do { \
      slong ixxx; \
      for (ixxx = 0; ixxx < (nnn); ixxx++) \
         (xxx)[ixxx] = (yyy)[ixxx]; \
   } while (0)

#define flint_mpn_copyd(xxx, yyy, nnn) \
   do { \
      slong ixxx; \
      for (ixxx = nnn - 1; ixxx >= 0; ixxx--) \
         (xxx)[ixxx] = (yyy)[ixxx]; \
   } while (0)

#define flint_mpn_store(xxx, nnn, yyy) \
   do \
   { \
      slong ixxx; \
      for (ixxx = 0; ixxx < nnn; ixxx++) \
         (xxx)[ixxx] = yyy; \
   } while (0)

/* common usage of flint_malloc */
#define FLINT_ARRAY_ALLOC(n, T) (T *) flint_malloc((n)*sizeof(T))
#define FLINT_ARRAY_REALLOC(p, n, T) (T *) flint_realloc(p, (n)*sizeof(T))

/* temporary allocation */
#define TMP_INIT \
   typedef struct __tmp_struct { \
      void * block; \
      struct __tmp_struct * next; \
   } __tmp_t; \
   __tmp_t * __tmp_root; \
   __tmp_t * __tpx

#define TMP_START \
   __tmp_root = NULL

#ifdef FLINT_WANT_ASSERT
#define TMP_ALLOC(size) \
   (__tpx = (__tmp_t *) alloca(sizeof(__tmp_t)), \
       __tpx->next = __tmp_root, \
       __tmp_root = __tpx, \
       __tpx->block = flint_malloc(size))
#else
#define TMP_ALLOC(size) \
   (((size) > 8192) ? \
      (__tpx = (__tmp_t *) alloca(sizeof(__tmp_t)), \
       __tpx->next = __tmp_root, \
       __tmp_root = __tpx, \
       __tpx->block = flint_malloc(size)) : \
      alloca(size))
#endif

#define TMP_ARRAY_ALLOC(n, T) (T *) TMP_ALLOC((n)*sizeof(T))

#define TMP_END \
   while (__tmp_root) { \
      flint_free(__tmp_root->block); \
      __tmp_root = __tmp_root->next; \
   }

/* compatibility between gmp and mpir */
#ifndef mpn_com_n
#define mpn_com_n mpn_com
#endif

#ifndef mpn_neg_n
#define mpn_neg_n mpn_neg
#endif

int parse_fmt(int * floating, const char * fmt);

int flint_printf(const char * str, ...); /* flint version of printf */
int flint_sprintf(char * s, const char * str, ...); /* flint version of sprintf */
int flint_scanf(const char * str, ...); /* flint version of scanf */
int flint_sscanf(const char * s, const char * str, ...); /* flint version of sscanf */

#ifdef FLINT_HAVE_VA_LIST
int flint_vprintf(const char * str, va_list ap); /* va_list version of flint_printf */
#endif

#ifdef FLINT_HAVE_FILE
int flint_fprintf(FILE * f, const char * str, ...); /* flint version of fprintf */
int flint_fscanf(FILE * f, const char * str, ...); /* flint version of fscanf */
#endif

FLINT_INLINE slong flint_mul_sizes(slong x, slong y)
{
    ulong hi, lo;

    umul_ppmm(hi, lo, (ulong) x, (ulong) y);
    if (hi != 0 || lo > WORD_MAX)
    {
        flint_printf("Exception (flint). Overflow creating size %wd x %wd object.\n", x, y);
        flint_abort();
    }
    return lo;
}

#include "exception.h"

/* Defined ahead of nmod.h, fmpz.h and fmpq.h so that the types are usable in
 * prototypes without importing those headers */

typedef struct
{
   mp_limb_t n;
   mp_limb_t ninv;
   flint_bitcnt_t norm;
}
nmod_t;

typedef slong fmpz;
typedef fmpz fmpz_t[1];

typedef struct
{
    fmpz num;
    fmpz den;
}
fmpq;

typedef fmpq fmpq_t[1];

#define fmpq_numref(__x) (&(__x)->num)
#define fmpq_denref(__y) (&(__y)->den)

#ifdef __cplusplus
}
#endif

#endif
