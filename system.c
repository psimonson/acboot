/**
 * @file system.c
 * @author Philip R. Simonson
 * @date 02/26/2020
 * @brief System functions by me.
 *************************************************************
 */

asm(".code16gcc");

#include "attr.h"
#include "system.h"

/* Timer for waiting inside of code.
 */
__REGPARM void timer(unsigned short high, unsigned short low)
{
	asm volatile(
		"int $0x15"
		:
		: "a"(0x8600), "c"(high), "d"(low)
	);
}
/* Output val to port.
 */
__REGPARM void outb(unsigned short port, unsigned char val)
{
	asm volatile(
		"out %0, %1\n"
		:
		: "a"(val), "Nd"(port)
	);
}
/* Return val from port.
 */
__REGPARM unsigned char inb(unsigned short port)
{
	unsigned short output = 0;
	asm volatile(
		"in %1, %0\n"
		: "=a"(output)
		: "Nd"(port)
	);
	return output;
}
/* Connect to the APM API.
 */
__REGPARM int connect_apm(void)
{
	unsigned char failed = 0;
	asm volatile(
		"movb $0, %0\n"
		"int $0x15\n"
		"setcb %0\n"
		: "=r"(failed)
		: "a"(0x5301), "b"(0x0000)
		: "cc"
	);
	return !failed;
}
/* Set APM version.
 */
__REGPARM void set_apm_ver(unsigned short ver)
{
	asm volatile(
		"int $0x15"
		:
		: "a"(0x530e), "b"(0x0000), "c"(ver)
	);
}
/* Power off the machine.
 */
__REGPARM int init_shutdown(void)
{
	unsigned char failed = 0;
	asm volatile(
		"movb $0, %0\n"
		"int $0x15\n"
		"setcb %0\n"
		: "=r"(failed)
		: "a"(0x5307), "b"(0x0001), "c"(0x0003)
		: "cc"
	);
	return !failed;
}
/* Halting the system's CPU.
 */
__REGPARM void halt_cpu(void)
{
	asm volatile(
		"cli\n"
		"hlt\n"
	);
}
