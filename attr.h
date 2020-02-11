/**
 * @file attr.h
 * @author Philip R. Simonson
 * @date 02/11/2020
 * @brief Just a few macros to simplify attributes for GCC.
 ********************************************************************
 */

#ifndef _ATTR_H_
#define _ATTR_H_

#define __NOINLINE __attribute__((noinline))
#define __REGPARM __attribute__((regparm(3)))
#define __PACKED __attribute__((packed))
#define __NORETURN __attribute__((noreturn))

#endif
