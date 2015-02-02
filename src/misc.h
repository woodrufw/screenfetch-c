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


/* disp.c and detect.c modules */
#define ERR_REPORT(str) (fprintf(stderr, TWHT "[[ " TLRD "!" TWHT " ]] " TNRM "Error: %s\n", str))
#define VERBOSE_OUT(str1, str2) (fprintf(stdout, TLRD ":: " TNRM "%s%s\n", str1, str2))


/*  Manifest constants used to control the 'for'
    loop iteration cycles in some of the disp.c
    functions, they also appear in structs.h
*/
#define MICROSOFT_LEN     16
#define MACOSX_LEN        16
#define ARCH_OLD_LEN      18
#define ARCH_LEN          19
#define MINT_LEN          18
#define LMDE_LEN          18
#define UBUNTU_LEN        18
#define DEBIAN_LEN        18
#define CRUNCHBANG_LEN    18
#define GENTOO_LEN        18
#define FUNTOO_LEN        18
#define FEDORA_LEN        18
#define MANDRAKE_LEN      18
#define OPENSUSE_LEN      18
#define SLACKWARE_LEN     21
#define REDHAT_LEN        18
#define FRUGALWARE_LEN    23
#define PEPPERMINT_LEN    18
#define SOLUSOS_LEN       18
#define MAGEIA_LEN        18
#define PARABOLA_LEN      18
#define VIPERR_LEN        18
#define DEEPIN_LEN        18
#define CHAKRA_LEN        18
#define FUDUNTU_LEN       21
#define TRISQUEL_LEN      18
#define MANJARO_LEN       18
#define ELEMENTARY_LEN    18
#define SCIENTIFIC_LEN    20
#define BACKTRACK_LEN     21
#define KALI_LEN          21
#define SABAYON_LEN       18
#define ANDROID_LEN       16
#define ANGSTROM_LEN      16
#define LINUX_LEN         18
#define FREEBSD_LEN       18
#define OPENBSD_LEN       23
#define DRAGONFLYBSD_LEN  18
#define NETBSD_LEN        19
#define SUNOS_LEN         17


/* internationalization macros */
#ifdef _LIBC
# include <libintl.h>
# undef dgettext
# define dgettext(domain, msgid) \
   INTUSE(__dcgettext) (domain, msgid, LC_MESSAGES)
#else
# include "gettext.h"
#endif
#define N_(msgid) msgid


#endif /* SCREENFETCH_C_MISC_H */