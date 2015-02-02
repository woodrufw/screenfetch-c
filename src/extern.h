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

/* logos.c module */
extern char *oldarch_logo[];
extern char *arch_logo[];
extern char *mint_logo[];
extern char *lmde_logo[];
extern char *ubuntu_logo[];
extern char *debian_logo[];
extern char *crunchbang_logo[];
extern char *gentoo_logo[];
extern char *funtoo_logo[];
extern char *fedora_logo[];
extern char *freebsd_logo[];
extern char *openbsd_logo[];
extern char *dragonflybsd_logo[];
extern char *netbsd_logo[];
extern char *mandriva_mandrake_logo[];
extern char *opensuse_logo[];
extern char *slackware_logo[];
extern char *redhat_logo[];
extern char *frugalware_logo[];
extern char *peppermint_logo[];
extern char *solusos_logo[];
extern char *mageia_logo[];
extern char *parabolagnu_linuxlibre_logo[];
extern char *viperr_logo[];
extern char *linuxdeepin_logo[];
extern char *chakra_logo[];
extern char *fuduntu_logo[];
extern char *macosx_logo[];
extern char *windows_logo[];
extern char *trisquel_logo[];
extern char *manjaro_logo[];
extern char *elementaryos_logo[];
extern char *scientificlinux_logo[];
extern char *backtracklinux_logo[];
extern char *kalilinux_logo[];
extern char *sabayon_logo[];
extern char *android_logo[];
extern char *linux_logo[];
extern char *solaris_logo[];
extern char *angstrom_logo[];


/* flags.c module */
extern bool error;
extern bool process_logo_only;
extern bool verbose;
extern bool main_logo;
extern bool screenshot;
extern bool portrait;
extern bool logo_only_requested;


/* arrays.c module */
#define MAX_STR 128
extern char *detected_arr[];
extern char *detected_arr_names[];
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
extern char *mult_shell_arr[][2];
extern const size_t shells_num;
extern char given_distro_str[MAX_STR];

#endif /* SCREENFETCH_C_EXTERN_H */