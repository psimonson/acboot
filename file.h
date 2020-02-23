/**
 * @file file.h
 * @author Philip R. Simonson
 * @date 02/19/2020
 * @brief Starting sectors and number of sectors per image.
 *************************************************************
 */

#ifndef _FILE_H_
#define _FILE_H_

/* some defines for io sys */
#define IO_START	2		/* starting sector of operating system */
#define IO_SECTORS	13		/* total operating system sectors */
/* some defines for binary app */
#define SHELL_START (IO_START+IO_SECTORS)+1
#define SHELL_SECTORS 17
/* some defines for graph app */
#define GRAPH_START (SHELL_START+SHELL_SECTORS)+1
#define GRAPH_SECTORS 12
/* some defines for hello app */
#define HELLO_START (GRAPH_START+GRAPH_SECTORS)+1
#define HELLO_SECTORS 10

#endif
