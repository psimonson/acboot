/**
 * @file stdio.c
 * @author Philip R. Simonson
 * @date 02/07/2020
 * @brief Simple I/O implementation header.
 *************************************************************
 */

#ifndef _STDIO_H_
#define _STDIO_H_

#include "attr.h"

/* ---------------------- Inline ASM Functions ---------------------- */

__REGPARM void setup(void);
__REGPARM void getcur(unsigned char *x, unsigned char *y);
__REGPARM void mvcur(unsigned char x, unsigned char y);
__REGPARM void clrscr(unsigned char mode);
__REGPARM int getc(void);
__REGPARM void putc(int c);

/* --------------------------- C Functions -------------------------- */

int isspace(int c);
int strlen(const char *s);
void reverse(char *s);
void itoa(int n, char *s, int size);
void print(const char *data, int len);
int printf(const char *fmt, ...);
int gets(char s[], int size);
int memcmp(const void *s, const void *t, int size);
void *memset(void *bufp, int value, int size);
void *memcpy(void *dstp, const void *srcp, int size);
int strcmp(const char *s, const char *t);

#endif
