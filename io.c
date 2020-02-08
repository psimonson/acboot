asm(".code16gcc");
asm("jmpl $0x0000, $main");
void timer(unsigned short high, unsigned short low)
{
	asm volatile(
		"int $0x15"
		:
		: "a"(0x8600), "c"(high), "d"(low)
	);
}
void outb(unsigned short port, unsigned char val)
{
	asm volatile(
		"out %0, %1\n"
		:
		: "a"(val), "Nd"(port)
	);
}
unsigned char inb(unsigned short port)
{
	unsigned short output = 0;
	asm volatile(
		"in %1, %0\n"
		: "=a"(output)
		: "Nd"(port)
	);
	return output;
}
int getc(void)
{
	unsigned char ch = 0;
	asm("int $0x16" : "=a"(ch) : "a"(0x0000));
	return ch;
}
void putc(int c)
{
	asm volatile(
		"int $0x10" : : "a"(0x0e00 | ((unsigned char)c)),
		"b"(0x0007), "c"(0x0001)
	);
}
/* Beep system.
 */
void beep(void)
{
	const unsigned short freq = 4560;
	outb(0x43, 182);
	outb(0x42, (unsigned char)(freq));
	outb(0x42, (unsigned char)(freq >> 8));
	outb(0x61, inb(0x61) | 0x03);
	timer(0x0005, 0x05a0);
	outb(0x61, inb(0x61) & 0x3f);
}
/* Print text like a typer writer.
 */
void _type(const char *s, unsigned short freq, unsigned short high,
	unsigned short low)
{
	while(*s) {
		putc(*s++);
		beep();
	}
}
#define type(m) _type(m, 4560, 0x0005, 0x05a0);
/* Entry point for boot sector.
 */
void main(void)
{
	asm volatile(
		"xor %ax, %ax\n"
		"mov %ax, %ds\n"
		"mov %ax, %es\n"
		"mov %ax, %ss\n"
		"mov $0x1000, %esp\n"
	);
	type("Press any key to reboot...\r\n");
	(void)getc();
	asm volatile(
		"jmpl $0xffff, $0x0000"
	);
}
