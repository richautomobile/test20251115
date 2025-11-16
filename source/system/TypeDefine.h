/**
 * \file
 *
 * \brief Platform types.
 *
 * Copyright (C) 2018 Innolux Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Corporation.
 *
 * MISRA-C:2012 Deviation List
 *
 * MISRA-2:
 * Deviated Rule:
 *
 */


#ifndef _TYPEDEFINE_H_
#define _TYPEDEFINE_H_

/* ==[ Includes ]============================================================ */

/* ==[ Macros ]============================================================== */

/** \brief no operation code */
#define nop()  __asm("nop");

/** \brief Linked value for the macro TRUE. */
#ifndef TRUE
#define TRUE        (1u)
#endif

/** \brief Linked value for the macro FALSE. */
#ifndef FALSE
#define FALSE       (0u)
#endif

/** \brief Pointer value for the macro NULL_PTR. */
#ifndef NULL_PTR
#define NULL_PTR    ((void *)0)
#endif

/* Unit_Test for testing static functions. By declaring the attribute unused,
   the declared static functions can be disabled. When Unit testing, these
   'static' functions can be made external non-static functions for full test
   coverage. */
#if defined(C_UNIT_TEST)
/** \brief For making declared internal variables external for unit testing. */
#define STATIC_VAR
/** \brief For allowing to modify constant variables for unit testing. */
#define CONST_VAR
/** \brief For making variables used in Osmain tasks static for unit testing. */
#define STATIC_FUNC
/** \brief For deleting declared internal functions for unit testing. */
#define STATIC_FUNC_PROTOTYPE __attribute__ ((unused))
#else
/** \brief For making declared internal variables external for unit testing. */
#define STATIC_VAR static
/** \brief For allowing to modify constant variables for unit testing. */
#define CONST_VAR    const
/** \brief For making variables used in Osmain tasks static for unit testing. */
#define STATIC_FUNC    static
/** \brief For deleting declared internal functions for unit testing. */
#define STATIC_FUNC_PROTOTYPE static
#endif

/* ==[ Types ]=============================================================== */
/** \brief linked const char* to strPtr. */
typedef const char*         strPtr;
/** \brief linked signed char to sint8. */
typedef signed char         sint8;
/** \brief linked unsigned char to uint8. */
typedef unsigned char       uint8;
/** \brief linked signed short to sint16. */
typedef signed short        sint16;
/** \brief linked unsigned short to uint16. */
typedef unsigned short      uint16;
/** \brief linked signed long to sint32. */
typedef signed long         sint32;
/** \brief linked unsigned long to uint32. */
typedef unsigned long       uint32;
/** \brief linked float to float32. */
typedef float               float32;
/** \brief linked unsigned char to boolean. */
typedef unsigned char       boolean;

/*
 * Definition of return values for unifying return variables.
 */
typedef uint8 t_FUNC_RETURN;
#define  FUNC_RETURN_OK 0u
#define  FUNC_RETURN_NG 1u

#define NormalHiLevel   1U
#define NormalLowLevel  0U

/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with external linkage ]====================== */





#endif /* TYPEDEFINE_H_ */
