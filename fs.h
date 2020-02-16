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

/* Find file in system */
struct file *search_file(const unsigned char *ftable, const char *filename);
/* Execute file in system */
void exec_file(const drive_params_t *p, const struct file *entry);

#endif
