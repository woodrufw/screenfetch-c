/*	misc.h
 *	Authors: William Woodruff and Aaron Caffrey
 *
 *	Miscellaneous macros used in screenfetch-c.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#ifndef SCREENFETCH_C_MISC_H
#define SCREENFETCH_C_MISC_H


#define KB (1024)
#define MB (KB * KB)
#define GB (MB * KB)
#define MAX_STRLEN 128
#define SET_GIVEN_DISTRO(str) (safe_strncpy(given_distro_str, str, MAX_STRLEN))
#define STREQ(x, y) (!strcmp(x, y))
#define FILE_EXISTS(file) (!access(file, F_OK))

/* two-dimensional arrays only */
#define ARR_LEN(array) (sizeof(array) / 16)


/* Text and logo colorization constants   */
#define TNRM "\x1B[0;0m"  /* normal       */
#define TBLK "\x1B[0;30m" /* black        */
#define TRED "\x1B[0;31m" /* red          */
#define TGRN "\x1B[0;32m" /* green        */
#define TBRN "\x1B[0;33m" /* brown        */
#define TBLU "\x1B[0;34m" /* blue         */
#define TPUR "\x1B[0;35m" /* purple       */
#define TCYN "\x1B[0;36m" /* cyan         */
#define TLGY "\x1B[0;37m" /* light gray   */
#define TDGY "\x1B[1;30m" /* dark gray    */
#define TLRD "\x1B[1;31m" /* light red    */
#define TLGN "\x1B[1;32m" /* light green  */
#define TYLW "\x1B[1;33m" /* yellow       */
#define TLBL "\x1B[1;34m" /* light blue   */
#define TLPR "\x1B[1;35m" /* light purple */
#define TLCY "\x1B[1;36m" /* light cyan   */
#define TWHT "\x1B[1;37m" /* white        */


/* internationalization macros */
#include "config.h" /* Auto generated from configure */
#ifdef _LIBC
# include <libintl.h>
# undef dgettext
# define dgettext(domain, msgid) \
   INTUSE(__dcgettext) (domain, msgid, LC_MESSAGES)
#else
# include "gettext.h"
#endif
#define N_(msgid) msgid
#define _(str) gettext(str)


/* feedback */
#define ERR_REPORT(str) (fprintf(stderr, TWHT "[[ " TLRD "!" TWHT " ]] " TNRM "%s: %s\n", _("Error"), str))
#define VERBOSE_OUT(str1, str2) (fprintf(stdout, TLRD ":: " TNRM "%s%s\n", str1, str2))


#endif /* SCREENFETCH_C_MISC_H */