/**
 * Generic defines, helpers, and utilities.
 *
 * @file   xyz.h
 * @author Matthew Hagerty
 * @date   April 5, 2020
 */

#ifndef XYZ_H_
#define XYZ_H_

#include <stdlib.h>     // malloc, calloc, realloc, free
#include <stdint.h>		// uintXX_t, intXX_t, UINTXX_MAX, INTXX_MAX, etc.

// Avoid C++ name-mangling for C functions.
#ifdef __cplusplus
extern "C" {
#endif


// Nice to have short-name types.  These types break the XYZ_ prefix rule in
// exchange for a shorter type name (which is half the reason for using them).
//
// The 'char' type in C is undefined as to whether it is signed or unsigned, and
// is left up to the implementation.  This is unfortunate, especially when
// interfacing with libc string functions.
//
// Specifically left unprotected (no #ifndef) so any redefinition will cause
// a pre-compiler error if these are already defined elsewhere.

#define c8  char        ///< Implementation-defined 8-bit type.
#define s8  int8_t      ///< Signed 8-bit type.
#define s16 int16_t     ///< Signed 16-bit type.
#define s32 int32_t     ///< Signed 32-bit type.
#define s64 int64_t     ///< Signed 64-bit type.
#define u8  uint8_t     ///< Unsigned 8-bit type.
#define u16 uint16_t    ///< Unsigned 16-bit type.
#define u32 uint32_t    ///< Unsigned 32-bit type.
#define u64 uint64_t    ///< Unsigned 64-bit type.
#define f32 float       ///< 32-bit binary floating point type.
#define f64 double      ///< 64-bit binary floating point type.


/// Error Block support.
#define XYZ_BLOCK do {
#define XYZ_BREAK break;
#define XYZ_RESTART continue;
#define XYZ_END break; } while(1);

/// Return for OK and Error.  Follows a common convention where a 0 (zero)
/// indicates success, greater-than 0 is additional information, and less-than
/// 0 is an error code.
#define XYZ_OK       0
#define XYZ_ERR     -1

/// Flags for making code more readable.
#define XYZ_TRUE     1
#define XYZ_YES      1
#define XYZ_ON       1

#define XYZ_FALSE    0
#define XYZ_NO       0
#define XYZ_OFF      0

/// string terminator (to avoid confusion because NUL != NULL (look it up)).
#define XYZ_NTERM '\0'

/// Current File and Line.
#define XYZ_CFL xyz_path_lastpart(__FILE__), __LINE__

/// Windows path separator.
#define XYZ_WIN_PSEP '\\'

/// Unix path separator.
#define XYZ_UNIX_PSEP '/'

// May have to be determined at runtime.
#if defined(_MSC_VER)
/// System path separator.
#define XYZ_SYS_PSEP XYZ_WIN_PSEP
#else
/// System path separator.
#define XYZ_SYS_PSEP XYZ_UNIX_PSEP
#endif


/// TODO placeholder, replace with allocator wrapper functions.
#define xyz_malloc(sz) malloc(sz)
#define xyz_calloc(num,sz) calloc(num,sz)
#define xyz_realloc(ptr,sz) realloc(ptr,sz)
#define xyz_free(ptr) free(ptr)

// TODO comments for meta, and initializer.
// TODO finish the currently incomplete metadata system.

/// Data-format of the buffer.
enum XYZ_META_FORMAT
   { XYZ_META_F_NOTVALID = 0     ///< Data buffer is not valid.
   , XYZ_META_F_POINTER  = 1<<0  ///< Pointer to the data.
   , XYZ_META_F_SINT     = 1<<1  ///< Signed integer data.
   , XYZ_META_F_BINFP    = 1<<2  ///< Binary FP data.
};


/// Buffer allocation for the pointer-type.
enum XYZ_META_POINTER_DATA
   { XYZ_META_P_STATIC = 0    ///< Read-only static data.
   , XYZ_META_P_DYNAMIC       ///< Read/write dynamic-array (malloc-allocated) data.
   , XYZ_META_P_FIXED         ///< Read/write fixed-array data.
};


/// Meta-data types.
enum XYZ_META_TYPE
   { XYZ_META_T_UNDEF = 0     ///< Undefined.
   , XYZ_META_T_ASCII_CHAR    ///< ASCII padded character.
   , XYZ_META_T_ASCII_VARCHAR ///< ASCII variable character.
   , XYZ_META_T_UTF8_CHAR     ///< UTF8 padded character.
   , XYZ_META_T_UTF8_VARCHAR  ///< UTF8 variable character.
   , XYZ_META_T_INTEGER_S4    ///< Signed integer with 1..4 digits.
   , XYZ_META_T_INTEGER_S9    ///< Signed integer with 5..9 digits.
   , XYZ_META_T_INTEGER_S19   ///< Signed integer with 10..19 digits.
   , XYZ_META_T_DECIMAL       ///< Decimal FP, unlimited digits.
   , XYZ_META_T_DECIMAL_128   ///< Decimal FP, 34-digits.
   , XYZ_META_T_DECIMAL_64    ///< Decimal FP, ??-digits.
   , XYZ_META_T_DECIMAL_32    ///< Decimal FP, ??-digits.
   , XYZ_META_T_DECFP_QUAD    ///< Decimal FP, compact format, 34-digits.
   , XYZ_META_T_DECFP_DBL     ///< Decimal FP, compact format, ??-digits.
   , XYZ_META_T_DECFP_SNGL    ///< Decimal FP, compact format, ??-digits.
   , XYZ_META_T_DECFP_BCD     ///< Decimal FP, BCD.
   , XYZ_META_T_DECFP_BCDP    ///< Decimal FP, Packed BCD.
   , XYZ_META_T_BINFP         ///< Binary FP, double, 16-digits.
};


typedef struct xyz_meta_unused_tag
{
   union {              ///< Local storage for types that fit in 64-bits.
   void    *vp;         ///< Pointer to the data.
   s64      si;         ///< Signed integer data.
   double   bfp;        ///< Binary floating point, 16-digits.
   }        buf;        ///< Data buffer.
   u16      format;     ///< Buffer format.
   u16      alloc;      ///< Buffer memory-allocation for the pointer-format.
   u16      type;       ///< Meta-data type.
   u16      reserved;   ///< Reserved for future use.
   u32      unit_dim;   ///< Dimension in units.
   u32      unit_len;   ///< Length in units.
   u32      byte_dim;   ///< Dimension in bytes.
   u32      byte_len;   ///< Length in bytes.
} xyz_meta;



const c8 * xyz_str_lastseg(const c8 *filepath, c8 sep);
const c8 * xyz_path_lastpart(const c8 *filepath);

/// TODO Implement.
//s32 xyz_meta_init(xyz_meta *mt, u32 type, u32 alloc);



// ==========================================================================
//
// Single reader-writer lock-free ring buffer access manager (RBAM)
//
// Note: The 'used' and 'free' fields are for informational status only and may
// be inaccurate if the reader and writer use different threads.
//
// ==========================================================================


/// Ring Buffer Access Manager (RBAM) structure.
typedef struct unused_tag_xyz_rbam {
   u32   dim;     ///< Dimension (total number) of elements in the buffer.
   u32   rd;      ///< Index for reading.
   u32   wr;      ///< Index for writing.
   u32   next;    ///< Next write index.
   u32   used;    ///< Number of used elements in the buffer.
   u32   free;    ///< Number of free elements in the buffer.
} xyz_rbam;


u32 xyz_rbam_init(xyz_rbam *rbam, u32 dim);
u32 xyz_rbam_next(xyz_rbam *rbam, u32 index);
u32 xyz_rbam_prev(xyz_rbam *rbam, u32 index);
u32 xyz_rbam_is_full(xyz_rbam *rbam);
u32 xyz_rbam_write(xyz_rbam *rbam);
u32 xyz_rbam_is_empty(xyz_rbam *rbam);
u32 xyz_rbam_read(xyz_rbam *rbam);
u32 xyz_rbam_drain(xyz_rbam *rbam);



#ifdef __cplusplus
}
#endif
#endif /* XYZ_H_ */

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2020 Matthew Hagerty
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
