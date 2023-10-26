/**
 * @file common.h
 * @brief Macros to facilitate interoperability between C and C++ in varying
 * compiler environments.
 */
#ifndef TYPES_COMMON_H
#define TYPES_COMMON_H

#include <stdint.h>

#define __hidden
#define __cppobj

#undef THIS_
#define THIS_ ECX, void* EDX
#define this  THIS_

#if defined(_MSC_VER)
#define CC_CDECL    __cdecl
#define CC_STDCALL  __stdcall
#define CC_FASTCALL __fastcall
#define CC_THISCALL __fastcall

#elif defined(__GNUC__) || defined(__clang__)
#define CC_CDECL    __attribute__((cdecl))
#define CC_STDCALL  __attribute__((stdcall))
#define CC_FASTCALL __attribute__((fastcall))
#define CC_THISCALL __attribute__((fastcall))

#else
#error "Unknown compiler"
#endif /* _MSC_VER */

#endif /* TYPES_COMMON_H */
