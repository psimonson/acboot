asm(".code16gcc");
asm("jmpl $0x0000, $main");
/* Timer for waiting inside of code.
 */
void timer(unsigned short high, unsigned short low)
{
	asm volatile(
		"int $0x15"
		:
		: "a"(0x8600), "c"(high), "d"(low)
	);
}
/* Output val to port.
 */
void outb(unsigned short port, unsigned char val)
{
	asm volatile(
		"out %0, %1\n"
		:
		: "a"(val), "Nd"(port)
	);
}
/* Return val from port.
 */
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
/* Get character from keyboard.
 */
int getc(void)
{
	unsigned char ch = 0;
	asm("int $0x16\nmov %%al, %0" : "=m"(ch) : "a"(0x0000));
	return ch;
}
/* Put character to screen.
 */
void putc(int c)
{
	asm volatile(
		"int $0x10" : : "a"(0x0e00 | (c & 0x000000ff)),
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
	timer(0x0003, 0x0a20);
	outb(0x61, inb(0x61) & 0x3c);
	timer(0x0003, 0x0a20);
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
/* String length function.
 */
int strlen(const char *s)
{
	int i;
	for(i = 0; *s != '\0'; i++);
	return i;
}
/* This function is used for printf.
 */
static void print(const char *data)
{
	const unsigned char *bytes = (const unsigned char*)data;
	while(*bytes)
		putc(*bytes++);
}
/* Get input from user into a buffer.
 */
void gets(char *s, int size)
{
	int c;
	while(--size > 0 && (c = getc()) != '\r') {
		*s++ = c;
		putc(c);
	}
	print("\r\n");
	*s = '\0';
}
/* Entry point for boot sector.
 */
void main(void)
{
	char buf[32];
	print("Please enter your name: ");
	gets(buf, sizeof(buf));
	print("Hello, ");
	print(buf);
	print("!\r\n");
	type("Press any key to reboot...\r\n");
	(void)getc();
	asm volatile(
		"jmpl $0xffff, $0x0000"
	);
}
