/**
 * @file unused.h
 * @author Philip R. Simonson
 * @date 02/16/2020
 * @brief Simple implementation for unused functions/parameters.
 **********************************************************************
 */

#ifndef _UNUSED_H_
#define _UNUSED_H_

#ifdef _GNU_C_
#define UNUSED(x) UNUSED_ ##x __attribute__((unused))
#else
#define UNUSED(x) x
#endif

#ifdef _GNU_C_
#define UNUSED_FUNC(x) __attribute__((unused)) UNUSED_ ##x
#else
#define UNUSED_FUNC(x) x
#endif

#endif
