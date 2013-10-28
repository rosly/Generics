/* 
 * This file is a part of Generics project
 * Copyright (c) 2013, Radoslaw Biernaki <radoslaw.biernacki@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3) No personal names or organizations' names associated with the 'Generics' project
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE GENERICS PROJET AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __GMACROS_H__
#define __GMACROS_H__ 1

/** Branch predition macros */
#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

/**
* Macro returns number of table elements
*
* @param _table Table pinter
* @return Number of table elements, the return type is size_t
*/
#define table_size(_table) (sizeof((_table)) / sizeof((_table)[0]))

/**
   Macro mask the "unused parameter" warning when using -Wall -Wextra compiler
   options. Those compiler options should be used to prevent typical C coding
   mistakes while unused macro allows to silent the waring when we really what
   to not use all of function parameters
 */
#if defined(__GNUC__)
# define unused(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define unused(x) /*@unused@*/ x
#else
# define unused(x) x
#endif

/**
   Common macro that allows to calculate the offset of field in structure

   @param _type Type of parent
   @param _member Name of member inside parent

   @return Offset in bytes (size_t) of member from the beginning of parent.
 */
#ifdef __compiler_offsetof
#define offsetof(_type,_member) __compiler_offsetof(_type, _member)
#else
#define offsetof(_type, _member) ((size_t) &(((_type *)NULL)->_member))
#endif

/** Common macro that allows to get size of member in structure or union */
#define sizeoffield(_type, _member) (sizeof(((_type *)NULL)->_member))

/**
   Common macro that allows to calculate pointer to parent,
   from pointer to member, name of the member inside parent and parent object type
   Using of temporary pointer _mptr is necessary to prevent macro side effects for
   operands like pointer++

   @param _prt Pointer to member
   @param _type Parent object type
   @param _member Name of the member inside parent object

   @return Pointer to parent
 */
#define container_of(_ptr, _type, _member) ({ \
    const typeof( ((_type *)0)->_member ) *_mptr = (_ptr); \
    (_type *)( (char *)_mptr - offsetof(_type,_member) );})

/**
   Common min macro with strict type-checking,
   returns the smaller value from two operands

   Strict type checking in important aspect of secure code,
   (sign type mixed checking is common source of exploitable bugs).
   Using of temporary values is necessary to prevent macro side effects for
   operands like variable++

   @param _x First value
   @param _y Second value

   @return Smaller of two passed values
 */
#define min(_x, _y) ({ \
    typeof(_x) _min1 = (_x); \
    typeof(_y) _min2 = (_y); \
    (void) (&_min1 == &_min2); \
    _min1 < _min2 ? _min1 : _min2; })

/**
   Common max macro with strict type-checking,
   returns the greater value from two operands

   Strict type checking in important aspect of secure code,
   (sign type mixed checking is common source of exploitable bugs).
   Using of temporary values is necessary to prevent macro side effects for
   operands like variable++

   @param _x First value
   @param _y Second value

   @return Greater of two passed values
 */
#define max(_x, _y) ({ \
    typeof(_x) _max1 = (_x); \
    typeof(_y) _max2 = (_y); \
    (void) (&_max1 == &_max2); \
    _max1 > _max2 ? _max1 : _max2; })

/**
   Common abs macro with strict type-checking,
   returns the absolute value of operand

   Strict type checking in important aspect of secure code,
   (sign type mixed checking is common source of exploitable bugs).
   Using of temporary values is necessary to prevent macro side effects for
   operands like variable++

   @param _x Operand

   @return Absolute value of operand
 */
#define abs(_x) ({\
    typeof(_x) _abs = (_x); \
    _abs > 0 ? _abs : -_abs; })

/**
   Macro used to calculate the ceiling(x/y), macro is type sensitive.
   Macro cannot be used with floating point types,
   for those please use ceil function from math.h

   @param _x dividend
   @param _y divisor

   @return ceil(_x/_y) with the same type as _x operand
 */
#define ceil_div(_x, _y) ({ \
  typeof(_x) __x = (_x); \
  typeof(_y) __y = (_y); \
  (void) (&__x == &__y); \
  typeof(_x) _rem = __x % __y; \
  typeof(_x) _div = __x / __y; \
  (_rem > 0) ? (_div + 1) : _div; })

/**
   Version of ceil_div without strict type checking, use with care
   Warning this version has common macro side effects for operands which use
   ++ or --
 */
#define ceil_div_nocheck(x, y) (((x) / (y)) + (((x) % (y)) ? 1 : 0))

/**
   Version of min without strict type checking, use with care
   Warning this version has common macro side effects for operands which use
   ++ or --
 */
#define min_nocheck(x, y) (((x) < (y)) ? (x) : (y))

/**
   Version of cmax without strict type checking, use with care
   Warning this version has common macro side effects for operands which use
   ++ or --
 */
#define max_nocheck(x, y) (((x) > (y)) ? (x) : (y))

/**
   Macro clamps the value to the given range, macro performs strict type checking.

   Strict type checking in important aspect of secure code,
   (sign type mixed checking is common source of exploitable bugs).
   Using of temporary values is necessary to prevent macro side effects for
   operands like variable++

   @param _val Clamped value
   @param _min Lower bound
   @param _max Upper boud
   @return Clamped value
 */
#define clamp(_val, _min, _max) ({ \
  typeof(_val) __val = (_val); \
  typeof(_min) __min = (_min); \
  typeof(_max) __max = (_max); \
  (void) (&__val == &__min); \
  (void) (&__val == &__max); \
  __val = __val < __min ? __min: __val; \
  __val > __max ? __max: __val; })

#endif /* __GMACROS_H__ */

