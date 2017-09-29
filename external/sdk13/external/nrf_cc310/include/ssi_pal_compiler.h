//-----------------------------------------------------------------------------------
//    The confidential and proprietary information contained in this file may 
//    only be used by a person authorised under and to the extent permitted
//    by a subsisting licensing agreement from ARM Limited or its affiliates 
//    or between you and a party authorised by ARM
//
//           (C) COPYRIGHT [2016] ARM Limited or its affiliates
//               ALL RIGHT RESERVED
//
//    This entire notice must be reproduced on all copies of this file
//    and copies of this files may only be made by a person if such person is
//    permitted to do so under the terms of a subsisting license agreement
//    from ARM Limited or its affiliates or between you and a party authorised by ARM
//-----------------------------------------------------------------------------------

#ifndef __SSI_PAL_COMPILER_H__
#define __SSI_PAL_COMPILER_H__

/*!
@file
@brief This file contains compiler related definitions.
*/

#ifdef __GNUC__

/************************ Defines ******************************/

/*! Associate a symbol with a link section. */
#define SASI_PAL_COMPILER_SECTION(sectionName)  __attribute__((section(sectionName)))

/*! Mark symbol as used, i.e., prevent garbage collector from dropping it. */
#define SASI_PAL_COMPILER_KEEP_SYMBOL __attribute__((used))

/*! Make given data item aligned (alignment in bytes). */
#define SASI_PAL_COMPILER_ALIGN(alignement)  __attribute__((aligned(alignement)))

/*! Mark function that never returns. */
#define SASI_PAL_COMPILER_FUNC_NEVER_RETURNS __attribute__((noreturn))

/* Prevent function from being inlined */
#define SASI_PAL_COMPILER_FUNC_DONT_INLINE __attribute__((noinline))

/*! Given data type may cast (alias) another data type pointer. */
/* (this is used for "superclass" struct casting)             */
#define SASI_PAL_COMPILER_TYPE_MAY_ALIAS __attribute__((__may_alias__))

/*! Get sizeof for a structure type member. */
#define SASI_PAL_COMPILER_SIZEOF_STRUCT_MEMBER(type_name, member_name) \
	sizeof(((type_name *)0)->member_name)

/*! Assertion. */
#define SASI_ASSERT_CONCAT_(a, b) a##b
#define SASI_ASSERT_CONCAT(a, b) SASI_ASSERT_CONCAT_(a, b)
#define SASI_PAL_COMPILER_ASSERT(cond, message) \
	enum { SASI_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(cond)) }

#elif defined(__ARM_DSM__) || defined(__CC_ARM)
#define inline
/*! Associate a symbol with a link section. */
#define SASI_PAL_COMPILER_SECTION(sectionName)  __attribute__((section(sectionName)))

/*! Mark symbol as used, i.e., prevent garbage collector from dropping it. */
#define SASI_PAL_COMPILER_KEEP_SYMBOL __attribute__((used))

/*! Make given data item aligned (alignment in bytes). */
#define SASI_PAL_COMPILER_ALIGN(alignement)  __attribute__((aligned(alignement)))

/*! Mark function that never returns. */
#define SASI_PAL_COMPILER_FUNC_NEVER_RETURNS __attribute__((noreturn))

/*! Prevent function from being inlined. */
#define SASI_PAL_COMPILER_FUNC_DONT_INLINE __attribute__((noinline))

/*! Given data type may cast (alias) another data type pointer. */
/* (this is used for "superclass" struct casting)             */
#define SASI_PAL_COMPILER_TYPE_MAY_ALIAS __attribute__((__may_alias__))

/*! Get sizeof for a structure type member. */
#define SASI_PAL_COMPILER_SIZEOF_STRUCT_MEMBER(type_name, member_name) \
	sizeof(((type_name *)0)->member_name)

/*! Assertion. */
#define SASI_ASSERT_CONCAT_(a, b) a##b
#define SASI_ASSERT_CONCAT(a, b) SASI_ASSERT_CONCAT_(a, b)
#define SASI_PAL_COMPILER_ASSERT(cond, message) \
	enum { SASI_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(cond)) }
#elif defined(__ARM_DS__)
#define inline
/*! Associate a symbol with a link section. */
#define SASI_PAL_COMPILER_SECTION(sectionName)  __attribute__((section(sectionName)))

/*! Mark symbol as used, i.e., prevent garbage collector from dropping it. */
#define SASI_PAL_COMPILER_KEEP_SYMBOL __attribute__((used))

/*! Make given data item aligned (alignment in bytes). */
#define SASI_PAL_COMPILER_ALIGN(alignement)  __attribute__((aligned(alignement)))

/*! Mark function that never returns. */
#define SASI_PAL_COMPILER_FUNC_NEVER_RETURNS __attribute__((noreturn))

/*! Prevent function from being inlined. */
#define SASI_PAL_COMPILER_FUNC_DONT_INLINE __attribute__((noinline))

/*! Given data type may cast (alias) another data type pointer. */
/* (this is used for "superclass" struct casting)             */
#define SASI_PAL_COMPILER_TYPE_MAY_ALIAS

/*! Get sizeof for a structure type member. */
#define SASI_PAL_COMPILER_SIZEOF_STRUCT_MEMBER(type_name, member_name) \
	sizeof(((type_name *)0)->member_name)

/*! Assertion. */
#define SASI_ASSERT_CONCAT_(a, b) a##b
#define SASI_ASSERT_CONCAT(a, b) SASI_ASSERT_CONCAT_(a, b)
#define SASI_PAL_COMPILER_ASSERT(cond, message) \
	enum { SASI_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(cond)) }
#else
#error Unsupported compiler.
#endif

#endif /*__SSI_PAL_COMPILER_H__*/
