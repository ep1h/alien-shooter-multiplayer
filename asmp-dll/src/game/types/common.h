#ifndef TYPES_COMMON_H
#define TYPES_COMMON_H

#include <stdint.h>

#define __hidden
#define __cppobj

#undef THIS_
#define THIS_ ECX, void* EDX
#define this  THIS_

#ifndef __stdcall
#define __stdcall __attribute__((__stdcall__))
#endif /* __stdcall */

#ifndef __fastcall
#define __fastcall __attribute__((__fastcall__))
#endif /* __fastcall */

#define __THISCALL __attribute__((__fastcall__))
#undef __thiscall
#define __thiscall __THISCALL

#endif /* TYPES_COMMON_H */
