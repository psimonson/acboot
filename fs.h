/**
 * @file fs.h
 * @author Philip R. Simonson
 * @date 02/15/2020
 * @brief File handling functions are in here.
 ********************************************************************
 */

#ifndef _FS_H_
#define _FS_H_

#include "types.h"

/* Memory address for shell entry point */
#define SHELL_ENTRY 0x8000

/* Get file name from system. */
char *get_filename(const struct file *entry);
/* Get file name from user. */
char *get_filename_user(const char *filename);
/* Find file in system */
struct file *search_file(const unsigned char *ftable, const char *filename);
/* Execute file in system */
void exec_file(const drive_params_t *p, const struct file *entry);
/* List files in root directory */
void list_files(const drive_params_t *p);

#endif
