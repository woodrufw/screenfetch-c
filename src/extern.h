/*  extern.h
 *  Authors: William Woodruff and Aaron Caffrey
 *  -------------
 *  
 *  All variables that will be accessed by the screenfetch-c modules are
 *  externalized here, think of them as 'globals'.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */
#ifndef SCREENFETCH_C_EXTERN_H
#define SCREENFETCH_C_EXTERN_H


/* flags.c module */
extern bool error;
extern bool process_logo_only;
extern bool verbose;
extern bool main_logo;
extern bool screenshot;
extern bool portrait;
extern bool break_requested;


/* arrays.c module */
#define MAX_STR 128
extern char *detected_arr[];
extern const char *detected_arr_names[];
extern char host_str[MAX_STR];
extern char distro_str[MAX_STR];
extern char kernel_str[MAX_STR];
extern char uptime_str[MAX_STR];
extern char pkgs_str[MAX_STR];
extern char cpu_str[MAX_STR];
extern char gpu_str[MAX_STR];
extern char disk_str[MAX_STR];
extern char mem_str[MAX_STR];
extern char shell_str[MAX_STR];
extern char res_str[MAX_STR];
extern char de_str[MAX_STR];
extern char wm_str[MAX_STR];
extern char wm_theme_str[MAX_STR];
extern char gtk_str[MAX_STR];
extern char icon_str[MAX_STR];
extern char font_str[MAX_STR];
extern char UseR[MAX_STR];
extern char HosT[MAX_STR];
extern char given_distro_str[MAX_STR];
extern const char *oldarch_logo[];
extern const char *arch_logo[];
extern const char *mint_logo[];
extern const char *lmde_logo[];
extern const char *ubuntu_logo[];
extern const char *debian_logo[];
extern const char *crunchbang_logo[];
extern const char *gentoo_logo[];
extern const char *funtoo_logo[];
extern const char *fedora_logo[];
extern const char *freebsd_logo[];
extern const char *openbsd_logo[];
extern const char *dragonflybsd_logo[];
extern const char *netbsd_logo[];
extern const char *mandriva_mandrake_logo[];
extern const char *opensuse_logo[];
extern const char *slackware_logo[];
extern const char *redhat_logo[];
extern const char *frugalware_logo[];
extern const char *peppermint_logo[];
extern const char *solusos_logo[];
extern const char *mageia_logo[];
extern const char *parabolagnu_linuxlibre_logo[];
extern const char *viperr_logo[];
extern const char *linuxdeepin_logo[];
extern const char *chakra_logo[];
extern const char *fuduntu_logo[];
extern const char *macosx_logo[];
extern const char *windows_logo[];
extern const char *trisquel_logo[];
extern const char *manjaro_logo[];
extern const char *elementaryos_logo[];
extern const char *scientificlinux_logo[];
extern const char *backtracklinux_logo[];
extern const char *kalilinux_logo[];
extern const char *sabayon_logo[];
extern const char *android_logo[];
extern const char *linux_logo[];
extern const char *solaris_logo[];
extern const char *angstrom_logo[];


#endif /* SCREENFETCH_C_EXTERN_H */