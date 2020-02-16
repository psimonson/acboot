ENTRY(main);
SECTIONS
{
	. = 0x8000;
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
