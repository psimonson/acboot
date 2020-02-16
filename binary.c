/**
 * @file binary.c
 * @author Philip R. Simonson
 * @date 02/15/2020
 * @brief Simple command shell for my operating system.
 *********************************************************************
 */

asm(".code16gcc\n");
asm("jmpl $0, $main\n");

#include "stdio.h"
#include "disk.h"
#include "types.h"

/* Conversion macros */
#define CONV_STR(x) #x
#define STR(x) CONV_STR(x)
/* Version information about this OS */
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_INFO STR(VERSION_MINOR) "." STR(VERSION_MAJOR)
/* Type definition for command function prototype */
typedef void (*command_cb)(void);
/* Command structure */
struct cmd {
	const char *cmd;
	const char *help;
	const command_cb *func;
};
/* Command define */
#define INIT_CMD_ARRAY		static struct cmd commands[] = {
#define ADD_CMD_ARRAY(c,h,f)	{ STR(c), STR(h), (command_cb *)&f }
#define END_CMD_ARRAY		};
#define COUNT_COMMANDS static unsigned char _total_commands = ((sizeof(commands)/sizeof(struct cmd)));

/* -------------------------- Prototypes ----------------------------- */

void cmd_help(void);
void cmd_version(void);

/* ------------------------------------------------------------------- */

/* Make command array */
INIT_CMD_ARRAY
ADD_CMD_ARRAY("help", "Prints this information text.", cmd_help),
ADD_CMD_ARRAY("version", "Prints the version information.", cmd_version)
END_CMD_ARRAY
COUNT_COMMANDS

/* --------------------------- Commands ------------------------------ */

/* Prints the help information to the screen.
 */
void cmd_help(void)
{
	int i;

	for(i = 0; i < _total_commands; i++) {
		printf("%s\t\t- %s\r\n", commands[i].cmd, commands[i].help);
	}
}
/* Prints the version information to the screen.
 */
void cmd_version(void)
{
	printf("ASM/C Boot OS - Version %s", VERSION_INFO);
}

/* ------------------------------------------------------------------- */

/* Simple shell program for my OS.
 */
void main(void)
{
	unsigned char drive = -1;
	drive_params_t p;
	asm volatile("" : "=d"(drive));
	setup();
	get_drive_params(drive, &p);
	printf("I'm in the new program shell now.\r\n");
	while(1) {
		timer(0x00f0, 0x05a0);
		break;
	}
	asm volatile("" : : "d"(p.drive));
	asm volatile("jmpl $0, $0x1000\n");
}
