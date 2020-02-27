/**
 * @file system.h
 * @author Philip R. Simonson
 * @date 02/26/2020
 * @brief System functions header.
 *************************************************************
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/* --------------------- Inline ASM Functions -------------------- */

__REGPARM void timer(unsigned short high, unsigned short low);
__REGPARM void outb(unsigned short port, unsigned char val);
__REGPARM unsigned char inb(unsigned short port);
__REGPARM int connect_apm(void);
__REGPARM void set_apm_ver(unsigned short ver);
__REGPARM int init_shutdown(void);
__REGPARM void halt_cpu(void);

/* ------------------------- C Functions ------------------------- */

#endif
