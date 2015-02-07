/*  flags.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *	
 *	screenfetch-c's flags are declared (and initialized) in this file.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#include <stdbool.h>

bool error = true;
bool process_logo_only;
bool main_logo = true;
bool portrait = false;
bool verbose = false;
bool screenshot = false;
bool break_requested = false;