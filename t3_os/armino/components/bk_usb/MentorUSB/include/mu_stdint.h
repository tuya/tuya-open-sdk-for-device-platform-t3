

/* Copyright (C) ARM Ltd., 1999 */
/* All rights reserved */

/*
 * RCS $Revision: 1.1 $
 * Checkin $Date: 2007/03/22 17:10:51 $
 * Revising $Author: richardv $
 */

/* Based on WG14/N843 (C9X) Committee Draft August 3, 1998 */

#ifndef __MU_STDINT_H__
#define __MU_STDINT_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MUSB_DPRINTF(fmt...)
#define MUSB_DPRINTF1(fmt...)
#define MUSB_DPRINTF2(fmt...)
#define MUSB_ERR_PRINTF


#define memory_usage_show();
// extern uint32_t memory_usage_show(void);


    /* minimum-width signed integer types */
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
//typedef   signed           int int_least32_t;
/*typedef   signed       __int64 int_least64_t;*/
/*typedef   signed       long long int_least64_t;*/

    /* minimum-width unsigned integer types */
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
//typedef unsigned           int uint_least32_t;
/*typedef unsigned       __int64 uint_least64_t;*/
/*typedef unsigned       long long uint_least64_t;*/
    /* 7.18.1.3 */

    /* fastest minimum-width signed integer types */
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
/*typedef   signed       __int64 int_fast64_t;*/
/*typedef   signed       long long int_fast64_t;*/

    /* fastest minimum-width unsigned integer types */
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
/*typedef unsigned       __int64 uint_fast64_t;*/
/*typedef unsigned       long long uint_fast64_t;*/

    /* 7.18.1.4 integer types capable of holding object pointers */
typedef   signed           int intptr_t;
//typedef unsigned           int uintptr_t;

    /* 7.18.1.5 greatest-width integer types */
/*typedef   signed       __int64 intmax_t;
typedef unsigned       __int64 uintmax_t;*/
/*typedef   signed       long long intmax_t;*/
/*typedef unsigned       long long uintmax_t;*/



#if 0

//#include "os_port.h"
  #ifndef __STDINT_DECLS
  #define __STDINT_DECLS

    #undef __CLIBNS

     #ifdef __cplusplus
      #ifdef __EDG_RUNTIME_USES_NAMESPACES
      namespace std {
          #define __CLIBNS std::
      #else
        #define __CLIBNS ::
      #endif /* ifdef __EDG_RUNTIME_USES_NAMESPACES */

    extern "C" {
    #else
      #define __CLIBNS
    #endif  /* __cplusplus */


/*
 * 'signed' is redundant below, except for 'signed char' and if
 * the typedef is used to declare a bitfield.
 * '__int64' is used instead of 'long long' so that this header
 * can be used in -strict mode.
 */

    /* 7.18.1.1 */
#if 0
    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
/*typedef   signed       __int64 int64_t;*/
/*typedef   signed     long long int64_t;*/

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
/*typedef unsigned       __int64 uint64_t;*/
/*typedef unsigned      long long uint64_t;*/
    /* 7.18.1.2 */
#endif
    /* smallest type of at least n bits */
    /* minimum-width signed integer types */
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
/*typedef   signed       __int64 int_least64_t;*/
/*typedef   signed       long long int_least64_t;*/

    /* minimum-width unsigned integer types */
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
/*typedef unsigned       __int64 uint_least64_t;*/
/*typedef unsigned       long long uint_least64_t;*/
    /* 7.18.1.3 */

    /* fastest minimum-width signed integer types */
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
/*typedef   signed       __int64 int_fast64_t;*/
/*typedef   signed       long long int_fast64_t;*/

    /* fastest minimum-width unsigned integer types */
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
/*typedef unsigned       __int64 uint_fast64_t;*/
/*typedef unsigned       long long uint_fast64_t;*/

    /* 7.18.1.4 integer types capable of holding object pointers */
typedef   signed           int intptr_t;
//typedef unsigned           int uintptr_t;

    /* 7.18.1.5 greatest-width integer types */
/*typedef   signed       __int64 intmax_t;
typedef unsigned       __int64 uintmax_t;*/
/*typedef   signed       long long intmax_t;*/
/*typedef unsigned       long long uintmax_t;*/

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

    /* 7.18.2.1 */

    /* minimum values of exact-width signed integer types */
#define INT8_MIN                   -128
#define INT16_MIN                -32768
#define INT32_MIN          (~0x7fffffff)   /* -2147483648 is unsigned */
#define INT64_MIN  (~0x7fffffffffffffffll) /* -9223372036854775808 is unsigned */

    /* maximum values of exact-width signed integer types */
#define INT8_MAX                    127
#define INT16_MAX                 32767
#define INT32_MAX            2147483647
#define INT64_MAX   9223372036854775807ll

    /* maximum values of exact-width unsigned integer types */
#define UINT8_MAX                   255
#define UINT16_MAX                65535
#define UINT32_MAX           4294967295u
#define UINT64_MAX 18446744073709551615ull

    /* 7.18.2.2 */

    /* minimum values of minimum-width signed integer types */
#define INT_LEAST8_MIN                   -128
#define INT_LEAST16_MIN                -32768
#define INT_LEAST32_MIN          (~0x7fffffff)
#define INT_LEAST64_MIN  (~0x7fffffffffffffffll)

    /* maximum values of minimum-width signed integer types */
#define INT_LEAST8_MAX                    127
#define INT_LEAST16_MAX                 32767
#define INT_LEAST32_MAX            2147483647
#define INT_LEAST64_MAX   9223372036854775807ll

    /* maximum values of minimum-width unsigned integer types */
#define UINT_LEAST8_MAX                   255
#define UINT_LEAST16_MAX                65535
#define UINT_LEAST32_MAX           4294967295u
#define UINT_LEAST64_MAX 18446744073709551615ull

    /* 7.18.2.3 */

    /* minimum values of fastest minimum-width signed integer types */
#define INT_FAST8_MIN           (~0x7fffffff)
#define INT_FAST16_MIN          (~0x7fffffff)
#define INT_FAST32_MIN          (~0x7fffffff)
#define INT_FAST64_MIN  (~0x7fffffffffffffffll)

    /* maximum values of fastest minimum-width signed integer types */
#define INT_FAST8_MAX             2147483647
#define INT_FAST16_MAX            2147483647
#define INT_FAST32_MAX            2147483647
#define INT_FAST64_MAX   9223372036854775807ll

    /* maximum values of fastest minimum-width unsigned integer types */
#define UINT_FAST8_MAX            4294967295u
#define UINT_FAST16_MAX           4294967295u
#define UINT_FAST32_MAX           4294967295u
#define UINT_FAST64_MAX 18446744073709551615ull

    /* 7.18.2.4 */

    /* minimum value of pointer-holding signed integer type */
#define INTPTR_MIN (~0x7fffffff)

    /* maximum value of pointer-holding signed integer type */
#define INTPTR_MAX   2147483647

    /* maximum value of pointer-holding unsigned integer type */
#define UINTPTR_MAX  4294967295u

    /* 7.18.2.5 */

    /* minimum value of greatest-width signed integer type */
#define INTMAX_MIN  (~0x7fffffffffffffffll)

    /* maximum value of greatest-width signed integer type */
#define INTMAX_MAX   9223372036854775807ll

    /* maximum value of greatest-width unsigned integer type */
#define UINTMAX_MAX 18446744073709551615ull

    /* 7.18.3 */

    /* limits of ptrdiff_t */
#define PTRDIFF_MIN (~0x7fffffff)
#define PTRDIFF_MAX   2147483647

    /* limits of sig_atomic_t */
#define SIG_ATOMIC_MIN (~0x7fffffff)
#define SIG_ATOMIC_MAX   2147483647

    /* limit of size_t */
//#define SIZE_MAX 4294967295u

    /* limits of wchar_t */
//#define WCHAR_MIN   0
//#define WCHAR_MAX   65535

#if 0 /* wint_t not yet supported */
    /* limits of wint_t */
#define WINT_MIN ?
#define WINT_MAX ?
#endif

#endif /* __STDC_LIMIT_MACROS */

#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

    /* 7.18.4.1 macros for minimum-width integer constants */
/*#define INT8_C(x)   (x)
#define INT16_C(x)  (x)
#define INT32_C(x)  (x)
#define INT64_C(x)  (x ## ll)

#define UINT8_C(x)  (x ## u)
#define UINT16_C(x) (x ## u)
#define UINT32_C(x) (x ## u)
#define UINT64_C(x) (x ## ull)*/

    /* 7.18.4.2 macros for greatest-width integer constants */
//#define INTMAX_C(x)  (x ## ll)
//#define UINTMAX_C(x) (x ## ull)

#endif /* __STDC_CONSTANT_MACROS */

    #ifdef __cplusplus
      }  /* extern "C" */
    #endif

    #ifdef __EDG_RUNTIME_USES_NAMESPACES
      }  /* namespace std */
    #endif
  #endif /* __STDINT_DECLS */

  #ifdef __EDG_RUNTIME_USES_NAMESPACES
    #ifndef __STDINT_NO_EXPORTS
      using std::int8_t;
      using std::int16_t;
      using std::int32_t;
      using std::int64_t;
      using std::uint8_t;
      using std::uint16_t;
      using std::uint32_t;
      using std::uint64_t;
      using std::int_least8_t;
      using std::int_least16_t;
      using std::int_least32_t;
      using std::int_least64_t;
      using std::uint_least8_t;
      using std::uint_least16_t;
      using std::uint_least32_t;
      using std::uint_least64_t;
      using std::int_fast8_t;
      using std::int_fast16_t;
      using std::int_fast32_t;
      using std::int_fast64_t;
      using std::uint_fast8_t;
      using std::uint_fast16_t;
      using std::uint_fast32_t;
      using std::uint_fast64_t;
      using std::intptr_t;
      using std::uintptr_t;
      using std::intmax_t;
      using std::uintmax_t;
    #endif
  #endif

#endif /* __MU_STDINT_H__ */

#endif

/* end of mu_stdint.h */


