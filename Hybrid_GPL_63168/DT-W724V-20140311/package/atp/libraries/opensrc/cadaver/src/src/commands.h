/* 
   cadaver, command-line DAV client
   Copyright (C) 1999-2001, Joe Orton <joe@orton.demon.co.uk>
                                                                     
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include "cadaver.h"
#include "atptypes.h"


extern int child_running; /* true when we have a child running */

#define CMD_VARY 9999

/* Returns the command structure for the command of given name. */
const struct command *get_command(const char *name);

/* Returns absolute path which is 'filename' relative to 'path' 
 * (which must already be an absolute path). e.g.
 *    resolve_path("/dav/", "asda") == "/dav/asda"
 *    resolve_path("/dav/", "/asda") == "/asda"
 * Also removes '..' segments, e.g.
 *    resolve_path("/dav/foobar/", "../fish") == "/dav/fish"
 * If isdir is true, ensures the return value has a trailing slash.
 */
char *resolve_path(const char *dir, const char *filename, int isdir);

/* Displays cadaver version details. */
void execute_about(void);

/* Returns owner href. */
char *getowner(void);

void out_success(void);
void out_start(const char *verb, const char *noun);
void out_result(int ret);
int out_handle(int ret);

VOS_UINT32  ATP_NAS_TranslateError(struct session session);

VOS_UINT32 ATP_NAS_Execute_mkcol(VOS_CHAR *DirectoryFullPath);

VOS_UINT32 ATP_NAS_Execute_rmcol(VOS_CHAR *DirectoryPath);

VOS_UINT32 ATP_NAS_Execute_propget(VOS_CHAR *res, VOS_CHAR *name, VOS_CHAR **value);

VOS_UINT32 ATP_NAS_Execute_MultiPropget(VOS_CHAR *res, VOS_CHAR **name, VOS_CHAR **value);

VOS_UINT32 ATP_NAS_Execute_put(VOS_CHAR *localName, VOS_CHAR *remoteName);

VOS_UINT32 ATP_NAS_Execute_get(VOS_CHAR *remoteName, VOS_CHAR *localName);

VOS_UINT32 ATP_NAS_Execute_set(VOS_CHAR *remoteName, VOS_CHAR *property_name, VOS_CHAR *property_value);

VOS_UINT32 ATP_NAS_Execute_delete(VOS_CHAR *filename);

VOS_UINT32 ATP_NAS_Execute_move(VOS_CHAR *srcFile, VOS_CHAR *dstFile);

VOS_UINT32 ATP_NAS_Execute_copy(VOS_CHAR *srcFile, VOS_CHAR *dstFile);

VOS_CHAR *ATP_NAS_Escape_path(const char *p);

#endif /* COMMANDS_H */
