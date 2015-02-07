/*  structs.h
 *  Author: Aaron Caffrey
 *  -------------
 *  
 *  All structures and their members are declared, initialized (and externalized)
 *  here for use by other screenfetch-c modules.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#ifndef SCREENFETCH_C_STRUCTS_H
#define SCREENFETCH_C_STRUCTS_H


/* see "uint_fast16_t len" */
#define MICROSOFT_LEN     17
#define MACOSX_LEN        17
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
#define ANDROID_LEN       17
#define ANGSTROM_LEN      17
#define LINUX_LEN         18
#define FREEBSD_LEN       18
#define OPENBSD_LEN       23
#define DRAGONFLYBSD_LEN  18
#define NETBSD_LEN        19
#define SUNOS_LEN         17


typedef struct
{
    const char **logo;              /* the ascii logo in arrays.c */
    uint_fast16_t len;              /* controls the process_data() 'for' loop iteration cycles */
    char *colour;                   /* colorize detected_arr_names[x] and "user@hostname" with the given colour */
    char *distro;                   /* the distro name to match in main_ascii_output() */
} mad_dog_struct;


#define MAD_DOG_LEN 36
/*  keep in mind to change the 'MAD_DOG_LEN'
    value whenever you add/remove distro/OS
*/
const mad_dog_struct opt[] = 
{
    {.logo = oldarch_logo, .len = ARCH_OLD_LEN, .colour = TLBL, .distro = "Arch Linux - Old"},

    {.logo = arch_logo, .len = ARCH_LEN, .colour = TLCY, .distro = "Arch Linux"},

    {.logo = mint_logo, .len = MINT_LEN, .colour = TLGN, .distro = "LinuxMint"},

    {.logo = lmde_logo, .len = LMDE_LEN, .colour = TLGN, .distro = "LMDE"},

    {.logo = debian_logo, .len = DEBIAN_LEN, .colour = TLRD, .distro = "Debian"},

    {.logo = crunchbang_logo, .len = CRUNCHBANG_LEN, .colour = TDGY, .distro = "CrunchBang"},

    {.logo = gentoo_logo, .len = GENTOO_LEN, .colour = TLPR, .distro = "Gentoo"},

    {.logo = funtoo_logo, .len = FUNTOO_LEN, .colour = TLPR, .distro = "Funtoo"},

    {.logo = fedora_logo, .len = FEDORA_LEN, .colour = TLBL, .distro = "Fedora"},

    {.logo = opensuse_logo, .len = OPENSUSE_LEN, .colour = TLGN, .distro = "OpenSUSE"},

    {.logo = slackware_logo, .len = SLACKWARE_LEN, .colour = TLBL, .distro = "Slackware"},

    {.logo = redhat_logo, .len = REDHAT_LEN, .colour = TRED, .distro = "Red Hat Linux"},

    {.logo = frugalware_logo, .len = FRUGALWARE_LEN, .colour = TLBL, .distro = "Frugalware"},

    {.logo = peppermint_logo, .len = PEPPERMINT_LEN, .colour = TLRD, .distro = "Peppermint"},

    {.logo = solusos_logo, .len = SOLUSOS_LEN, .colour = TDGY, .distro = "SolusOS"},

    {.logo = mageia_logo, .len = MAGEIA_LEN, .colour = TLCY, .distro = "Mageia"},

    {.logo = parabolagnu_linuxlibre_logo, .len = PARABOLA_LEN, .colour = TLPR, .distro = "ParabolaGNU/Linux-libre"},

    {.logo = viperr_logo, .len = VIPERR_LEN, .colour = TDGY, .distro = "Viperr"},

    {.logo = linuxdeepin_logo, .len = DEEPIN_LEN, .colour = TLGN, .distro = "LinuxDeepin"},

    {.logo = chakra_logo, .len = CHAKRA_LEN, .colour = TLBL, .distro = "Chakra"},

    {.logo = fuduntu_logo, .len = FUDUNTU_LEN, .colour = TLRD, .distro = "Fuduntu"},

    {.logo = trisquel_logo, .len = TRISQUEL_LEN, .colour = TLBL, .distro = "Trisquel"},

    {.logo = manjaro_logo, .len = MANJARO_LEN, .colour = TLGN, .distro = "Manjaro"},

    {.logo = elementaryos_logo, .len = ELEMENTARY_LEN, .colour = TDGY, .distro = "elementary OS"},

    {.logo = scientificlinux_logo, .len = SCIENTIFIC_LEN, .colour = TLBL, .distro = "Scientific Linux"},

    {.logo = backtracklinux_logo, .len = BACKTRACK_LEN, .colour = TLRD, .distro = "Backtrack Linux"},

    {.logo = kalilinux_logo, .len = KALI_LEN, .colour = TLBL, .distro = "Kali Linux"},

    {.logo = sabayon_logo, .len = SABAYON_LEN, .colour = TLBL, .distro = "Sabayon"},

    {.logo = android_logo, .len = ANDROID_LEN, .colour = TLGN, .distro = "Android"},

    {.logo = angstrom_logo, .len = ANGSTROM_LEN, .colour = "", .distro = "Angstrom"},

    {.logo = linux_logo, .len = LINUX_LEN, .colour = TDGY, .distro = "Linux"},

    {.logo = freebsd_logo, .len = FREEBSD_LEN, .colour = TLRD, .distro = "FreeBSD"},

    {.logo = openbsd_logo, .len = OPENBSD_LEN, .colour = TYLW, .distro = "OpenBSD"},

    {.logo = netbsd_logo, .len = NETBSD_LEN, .colour = TLRD, .distro = "NetBSD"},

    {.logo = dragonflybsd_logo, .len = DRAGONFLYBSD_LEN, .colour = TLRD, .distro = "DragonFly BSD"},

    {.logo = solaris_logo, .len = SUNOS_LEN, .colour = TBLU, .distro = "SunOS"}

};


#endif /* SCREENFETCH_C_STRUCTS_H */