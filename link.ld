ENTRY(main);
SECTIONS
{
	. = 0x7e00;
	.text :
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
