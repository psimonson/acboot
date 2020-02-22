ENTRY(main);
SECTIONS
{
	.text : AT(0x0000)
	{
		*(.text);
	}
	.data :
	{
		*(.bss);
		*(.rodata);
		*(.data);
	}
	/DISCARD/ :
	{
		*(.note*);
		*(.iplt*);
		*(.igot*);
		*(.rel*);
		*(.comment*);
		*(.eh_frame*);
	}
}
