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
#include "unused.h"
#include "fs.h"

/* Conversion macros */
#define CONV_STR(x) #x
#define STR(x) CONV_STR(x)
/* Version information about this OS */
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_INFO STR(VERSION_MAJOR) "." STR(VERSION_MINOR)
/* Command structure */
struct cmd {
	const char *cmd;
	const char *help;
	int (*func)(const drive_params_t *p);
};
/* Command define */
#define INIT_CMD_ARRAY		static struct cmd commands[] = {
#define ADD_CMD_ARRAY(c,h,f)	{ c, h, f }
#define END_CMD_ARRAY		};
#define COUNT_COMMANDS static unsigned char _total_commands = ((sizeof(commands)/sizeof(struct cmd)));

/* -------------------------- Prototypes ----------------------------- */

int cmd_help(const drive_params_t *p);
int cmd_list(const drive_params_t *p);
int cmd_find(const drive_params_t *p);
int cmd_version(const drive_params_t *p);
int cmd_exit(const drive_params_t *p);

/* ------------------------------------------------------------------- */

/* Make command array */
INIT_CMD_ARRAY
ADD_CMD_ARRAY("help", "Prints this information text.", cmd_help),
ADD_CMD_ARRAY("list", "Prints all the files in the drive.", cmd_list),
ADD_CMD_ARRAY("find", "Search the drive for a file.", cmd_find),
ADD_CMD_ARRAY("version", "Prints the version information.", cmd_version),
ADD_CMD_ARRAY("exit", "Exits back to the end of the OS.", cmd_exit)
END_CMD_ARRAY
COUNT_COMMANDS

/* --------------------------- Commands ------------------------------ */

/* Prints the help information to the screen.
 */
int cmd_help(const drive_params_t *UNUSED(p))
{
	int i;

	for(i = 0; i < _total_commands; i++) {
		printf("%s - %s\r\n", commands[i].cmd, commands[i].help);
	}
	return 0;
}
/* List all files in the root of the drive.
 */
int cmd_list(const drive_params_t *p)
{
	list_files(p);
	return 0;
}
/* Search for a file in the root of the drive.
 */
int cmd_find(const drive_params_t *p)
{
	const unsigned char *table = get_ftable(p);
	char buf[32];
	printf("Enter file name: ");
	gets(buf, sizeof(buf));
	if(search_file(table, buf) != NULL)
		printf("File %s found.\r\n", buf);
	else
		printf("File %s not found.\r\n", buf);
	return 0;
}
/* Prints the version information to the screen.
 */
int cmd_version(const drive_params_t *UNUSED(p))
{
	printf("ASM/C Boot OS - Version %s\r\n", VERSION_INFO);
	return 0;
}
/* Exits the shell program.
 */
int cmd_exit(const drive_params_t *UNUSED(p))
{
	return 1;
}

/* ------------------------------------------------------------------- */

/* Execute a command in the shell.
 */
int exec_cmd(const char *cmd, const drive_params_t *p)
{
	int i;

	for(i = 0; i < _total_commands; i++)
		if(strcmp(commands[i].cmd, cmd) == 0)
			return commands[i].func(p);

	printf("Bad command or filename.\r\n");
	return 0;
}
/* Main loop for shell program.
 */
void loop(const drive_params_t *p)
{
	char buf[32];
	char done = 0;

	printf("Welcome to my command shell. Please type 'help'\r\n"
		"for a list of available commands.\r\n");
	while(!done) {
		printf("> ");
		gets(buf, sizeof(buf));
		done = exec_cmd(buf, p);
	}
	printf("Shell quit.\r\n");
}
/* Simple shell program for my OS.
 */
void main(void)
{
	unsigned char drive = -1;
	drive_params_t p;
	asm volatile("" : "=d"(drive));
	setup();
	get_drive_params(drive, &p);
	loop(&p);
	asm volatile("jmpl $0x0000, $0x1000\n" : : "d"(p.drive));
}
