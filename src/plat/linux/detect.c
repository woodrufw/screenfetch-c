/*	detect.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	The detection functions used by screenfetch-c on Linux are implemented here.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

/* linux-specific includes */
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <pwd.h>
#include <glob.h>

/* program includes */
#include "../../misc.h"
#include "../../extern.h"
#include "../../prototypes.h"

#if defined (HAVE_X11_XLIB_H)
#	include <X11/Xlib.h>
#	include <X11/Xatom.h>
#endif

#if defined (HAVE_GL_GLX_H)
#	include <GL/gl.h>
#	include <GL/glx.h>
#endif

/*	detect_distro
	detects the computer's distribution (really only relevant on Linux)
*/
void detect_distro(void)
{
	/* if distro_str was NOT set by the -D flag */
	if (STREQ(distro_str, "Unknown") || STREQ(distro_str, "*"))
	{
		if (FILE_EXISTS("/system/bin/getprop"))
			safe_strncpy(distro_str, "Android", MAX_STRLEN);

		else
		{
			bool detected = false;
			uint_fast16_t x = 0;

			const char *distro_arr[][2] =
			{
				{"Kali", "Kali Linux"},
				{"Back", "Backtrack Linux"},
				{"Crun", "CrunchBang"},
				{"LMDE", "LMDE"},
				{"Debi", "Debian"},
				{"Rasp", "Debian"}
			};
			const size_t len_distro_arr = ARR_LEN(distro_arr);

			char distro_name_str[MAX_STRLEN];

			/* Bad solution, as /etc/issue contains junk on some distros */
			FILE *distro_file = fopen("/etc/issue", "r");
			if (distro_file != NULL)
			{
				/* get the first 4 chars, that's all we need */
				fscanf(distro_file, "%4s", distro_name_str);
				fclose(distro_file);

				for (x = 0; x < len_distro_arr; x++)
					if (STREQ(distro_name_str, distro_arr[x][0]))
					{
						safe_strncpy(distro_str, distro_arr[x][1], MAX_STRLEN);
						detected = true;
						break;
					}
			}

			if (!detected)
			{
				bool found_distro = false;
				const char *distro_file_arr[][2] =
				{
					{"/etc/fedora-release", "Fedora"},
					{"/etc/SuSE-release", "OpenSUSE"},
					{"/etc/arch-release", "Arch Linux"},
					{"/etc/gentoo-release", "Gentoo"},
					{"/etc/angstrom-version", "Angstrom"},
					{"/etc/manjaro-release", "Manjaro"}
				};
				const size_t len_distro_file_arr = ARR_LEN(distro_file_arr);
				
				for (x = 0; x < len_distro_file_arr; x++)
					if (FILE_EXISTS(distro_file_arr[x][0]))
					{
						safe_strncpy(distro_str, distro_file_arr[x][1], MAX_STRLEN);
						found_distro = true;
						break;
					}

				if (!found_distro)
				{
					if (FILE_EXISTS("/etc/lsb-release"))
					{
						distro_file = fopen("/etc/lsb-release", "r");
						fscanf(distro_file, "%s ", distro_name_str);
						fclose(distro_file);

						snprintf(distro_str, MAX_STRLEN, "%s", distro_name_str + 11);
					}
					else if (FILE_EXISTS("/etc/os-release"))
					{
						/*
							TODO: Parse NAME or PRETTY_NAME from os-release
							Until then, spit out an error message.
						*/
						if (error)
							ERR_REPORT(_("Failed to detect a Linux distro (1)."));
					}
					else
					{
						safe_strncpy(distro_str, "Linux", MAX_STRLEN);

						if (error)
							ERR_REPORT(_("Failed to detect a Linux distro (2)."));
					}
				}
			}
		}
	}

	return;
}

/*	detect_host
	detects the computer's hostname and active user and formats them
*/
void detect_host(void)
{
	char given_user[MAX_STRLEN] = "Unknown";
	char given_host[MAX_STRLEN] = "Unknown";
	struct passwd *user_info;
	struct utsname host_info;

	if ((user_info = getpwuid(geteuid())))
		safe_strncpy(given_user, user_info->pw_name, MAX_STRLEN);

	else if (error)
		ERR_REPORT(_("Could not detect username."));
	
	if (!(uname(&host_info)))
		safe_strncpy(given_host, host_info.nodename, MAX_STRLEN);

	else if (error)
		ERR_REPORT(_("Could not detect hostname."));

	snprintf(UseR, MAX_STRLEN, "%s", given_user);
	snprintf(HosT, MAX_STRLEN, "%s", given_host);

	return;
}

/*	detect_kernel
	detects the computer's kernel
*/
void detect_kernel(void)
{
	struct utsname kern_info;

	if (!(uname(&kern_info)))
		snprintf(kernel_str, MAX_STRLEN, "%s %s %s",
			kern_info.sysname, kern_info.release, kern_info.machine);

	else if (error)
		ERR_REPORT(_("Could not detect kernel information."));

	return;
}

/*	detect_uptime
	detects the computer's uptime
*/
void detect_uptime(void)
{
	uint_least32_t secs = 0, mins = 0, hrs = 0, days = 0;
	struct sysinfo si_upt;

	if (!(sysinfo(&si_upt)))
	{
		split_uptime(si_upt.uptime, &secs, &mins, &hrs, &days);

		if (days > 0)
			snprintf(uptime_str, MAX_STRLEN,
				"%"PRIuLEAST32"d %"PRIuLEAST32"h %"PRIuLEAST32"m %"PRIuLEAST32"s",
				days, hrs, mins, secs);
		else
			snprintf(uptime_str, MAX_STRLEN,
				"%"PRIuLEAST32"h %"PRIuLEAST32"m %"PRIuLEAST32"s",
				hrs, mins, secs);
	}
	else
		ERR_REPORT(_("Could not detect system uptime."));

	return;
}

static uint_fast16_t glob_packages(char *str1);

static uint_fast16_t glob_packages(char *str1)
{
	uint_fast16_t packs_num = 0;
	glob_t gl;

	if (!(glob(str1, GLOB_NOSORT, NULL, &gl)))
		packs_num = gl.gl_pathc;

	else if (error)
		ERR_REPORT(_("Failure while globbing packages."));

	globfree(&gl);

	return packs_num;
}

/*	detect_pkgs
	detects the number of packages installed on the computer
*/
void detect_pkgs(void)
{
	FILE *pkgs_file;
	uint_fast16_t packages = 0;

	if (STREQ(distro_str, "Arch Linux")
		|| STREQ(distro_str, "ParabolaGNU/Linux-libre")
		|| STREQ(distro_str, "Chakra") || STREQ(distro_str, "Manjaro"))

		packages = glob_packages("/var/lib/pacman/local/*");

	else if (STREQ(distro_str, "Frugalware"))
	{
		pkgs_file = popen("pacman-g2 -Q 2> /dev/null | wc -l", "r");
		fscanf(pkgs_file, "%"SCNuFAST16, &packages);
		pclose(pkgs_file);
	}
	else if (STREQ(distro_str, "Ubuntu") || STREQ(distro_str, "Lubuntu")
			|| STREQ(distro_str, "Xubuntu") || STREQ(distro_str, "LinuxMint")
			|| STREQ(distro_str, "SolusOS") || STREQ(distro_str, "Debian")
			|| STREQ(distro_str, "LMDE") || STREQ(distro_str, "CrunchBang")
			|| STREQ(distro_str, "Peppermint")
			|| STREQ(distro_str, "LinuxDeepin")	|| STREQ(distro_str, "Trisquel")
			|| STREQ(distro_str, "elementary OS")
			|| STREQ(distro_str, "Backtrack Linux")
			|| STREQ(distro_str, "Kali Linux"))
		packages = glob_packages("/var/lib/dpkg/info/*.list");

	else if (STREQ(distro_str, "Slackware"))
		packages = glob_packages("/var/log/packages/*");

	else if (STREQ(distro_str, "Gentoo") || STREQ(distro_str, "Sabayon") 
			|| STREQ(distro_str, "Funtoo"))
		packages = glob_packages("/var/db/pkg/*/*");

	else if (STREQ(distro_str, "Fuduntu") || STREQ(distro_str, "Fedora")
			|| STREQ(distro_str, "OpenSUSE")
			|| STREQ(distro_str, "Red Hat Linux")
			|| STREQ(distro_str, "Mandriva") || STREQ(distro_str, "Mandrake")
			|| STREQ(distro_str, "Mageia") || STREQ(distro_str, "Viperr"))
	{
		/* RPM uses Berkeley DBs internally, so this won't change soon */
		pkgs_file = popen("rpm -qa 2> /dev/null | wc -l", "r");
		fscanf(pkgs_file, "%"SCNuFAST16, &packages);
		pclose(pkgs_file);
	}
	else if (STREQ(distro_str, "Angstrom"))
	{
		pkgs_file = popen("opkg list-installed 2> /dev/null | wc -l", "r");
		fscanf(pkgs_file, "%"SCNuFAST16, &packages);
		pclose(pkgs_file);
	}
	else   /* if linux disto detection failed */
	{
		safe_strncpy(pkgs_str, _("Not Found"), MAX_STRLEN);

		if (error)
			ERR_REPORT(_("Packages cannot be detected on an unknown Linux distro."));

		return;
	}

	snprintf(pkgs_str, MAX_STRLEN, "%"PRIuFAST16, packages);

	return;
}

/*	detect_cpu
	detects the computer's CPU brand/name-string
*/
void detect_cpu(void)
{
	FILE *cpu_file;
	char cpuinfo_buf[MAX_STRLEN];
	char *cpuinfo_line;
	size_t end;
	uint_fast16_t i = 0;

	if ((cpu_file = fopen("/proc/cpuinfo", "r")))
	{
		/* read past the first 4 lines (#5 is model name) */
		for (i = 0; i < 5; i++)
		{
			if (!(fgets(cpuinfo_buf, MAX_STRLEN, cpu_file)))
			{
				ERR_REPORT(_("Fatal error while reading /proc/cpuinfo"));
				return;
			}
		}

		/* fail to match a colon. this should never happen, but check anyways */
		if (!(cpuinfo_line = strchr(cpuinfo_buf, ':')))
		{
			ERR_REPORT(_("Fatal error matching in /proc/cpuinfo"));
			return;
		}

		cpuinfo_line += 2;
		end = strlen(cpuinfo_line);

		if (cpuinfo_line[end - 1] == '\n')
			cpuinfo_line[end - 1] = '\0';

		if (STREQ(cpuinfo_line, "ARMv6-compatible processor rev 7 (v6l)"))
			safe_strncpy(cpu_str, "BCM2708 (Raspberry Pi)", MAX_STRLEN);

		else
			safe_strncpy(cpu_str, cpuinfo_line, MAX_STRLEN);

		remove_excess_cpu_txt();
	}
	else if (error)
		ERR_REPORT(_("Failed to open /proc/cpuinfo. Ancient Linux kernel?"));

	return;
}

/*	detect_gpu
	detects the computer's GPU brand/name-string
*/
void detect_gpu(void)
{
#if defined (HAVE_X11_XLIB_H) && defined (HAVE_GL_GLX_H)

	Display *disp = NULL;
	Window wind;
	GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo *visual_info = NULL;
	GLXContext context = NULL;

	if ((disp = XOpenDisplay(NULL)))
	{
		wind = DefaultRootWindow(disp);

		if ((visual_info = glXChooseVisual(disp, 0, attr)))
		{
			if ((context = glXCreateContext(disp, visual_info, NULL, GL_TRUE)))
			{
				glXMakeCurrent(disp, wind, context);
				safe_strncpy(gpu_str, (const char *) glGetString(GL_RENDERER),
						MAX_STRLEN);

				glXDestroyContext(disp, context);
			}
			else if (error)
				ERR_REPORT(_("Failed to create OpenGL context."));

			XFree((void *) visual_info);
		}
		else if (error)
			ERR_REPORT(_("Failed to select a proper X visual."));

		XCloseDisplay(disp);
	}
	else
	{
		safe_strncpy(gpu_str, _("No X Server"), MAX_STRLEN);

		if (error)
			ERR_REPORT(_("Could not open an X display (detect_gpu)."));
	}

#else	/* fallback to the detectgpu bash script */

	FILE *gpu_file = popen("detectgpu 2> /dev/null", "r");
	fgets(gpu_str, MAX_STRLEN, gpu_file);
	pclose(gpu_file);

#endif

	return;
}

/*	detect_disk
	detects the computer's total disk capacity and usage
*/
void detect_disk(void)
{
	struct statvfs disk_info;
	uintmax_t disk_total = 0, disk_used = 0, disk_percentage = 0;

	if (!(statvfs(getenv("HOME"), &disk_info)))
	{
		disk_total = ((disk_info.f_blocks * disk_info.f_bsize) / GB);
		disk_used = (((disk_info.f_blocks - disk_info.f_bfree) * disk_info.f_bsize) / GB);
		disk_percentage = (((float) disk_used / disk_total) * 100);
		snprintf(disk_str, MAX_STRLEN, "%"PRIuMAX"G / %"PRIuMAX"G (%"PRIuMAX"%%)",
									disk_used, disk_total, disk_percentage);
	}
	else if (error)
		ERR_REPORT(_("Could not stat $HOME for filesystem statistics."));

	return;
}

/*	detect_mem
	detects the computer's total and used RAM
*/
void detect_mem(void)
{
	uintmax_t total_mem = 0, used_mem = 0;
	struct sysinfo si_mem;

	sysinfo(&si_mem);

	total_mem = (uintmax_t) (si_mem.totalram * si_mem.mem_unit) / MB;
	used_mem  = (uintmax_t) ((si_mem.totalram - si_mem.freeram - si_mem.bufferram
									  - si_mem.sharedram) * si_mem.mem_unit) / MB;

	snprintf(mem_str, MAX_STRLEN, "%"PRIuMAX"%s / %"PRIuMAX"%s",
							used_mem, "MB", total_mem, "MB");

	return;
}

/*	detect_res
	detects the combined resolution of all monitors attached to the computer
*/
void detect_res(void)
{
#if defined (HAVE_X11_XLIB_H)

	uint_fast16_t width = 0, height = 0;
	Display *disp;
	Screen *screen;

	if ((disp = XOpenDisplay(NULL)))
	{
		screen = XDefaultScreenOfDisplay(disp);
		width = WidthOfScreen(screen);
		height = HeightOfScreen(screen);

		snprintf(res_str, MAX_STRLEN, "%"PRIuFAST16"x%"PRIuFAST16, width, height);

		XCloseDisplay(disp);
	}
	else
	{
		safe_strncpy(res_str, _("No X Server"), MAX_STRLEN);

		if (error)
			ERR_REPORT(_("Could not open an X display (detect_res)"));
	}

#else	/* fallback to `xdpyinfo' */

	FILE *res_file = popen("xdpyinfo | awk '/dimension/ {printf \"%s\",$2}'", "r");
	fgets(res_str, MAX_STRLEN, res_file);
	pclose(res_file);

	if (STREQ(res_str, "Unknown"))
	{
		safe_strncpy(res_str, _("No X Server"), MAX_STRLEN);

		if (error)
			ERR_REPORT(_("Could not open an X display (detect_res)"));
	}

#endif

	return;
}

/*	detect_de
	detects the desktop environment currently running on top of the OS
*/
void detect_de(void)
{
	char *curr_de;

	if ((curr_de = getenv("XDG_CURRENT_DESKTOP")))
		safe_strncpy(de_str, curr_de, MAX_STRLEN);

	else
	{
		if (getenv("GNOME_DESKTOP_SESSION_ID"))
			safe_strncpy(de_str, "Gnome", MAX_STRLEN);

		else if (getenv("MATE_DESKTOP_SESSION_ID"))
			safe_strncpy(de_str, "MATE", MAX_STRLEN);

		else if (getenv("KDE_FULL_SESSION"))
			/*	KDE_SESSION_VERSION only exists on KDE4+, so 
				getenv will return NULL on KDE <= 3.
			 */
			snprintf(de_str, MAX_STRLEN, "KDE%s", getenv("KDE_SESSION_VERSION"));

		else if (error)
			ERR_REPORT(_("No desktop environment found."));
	}

	return;
}

/*	detect_wm
	detects the window manager currently running on top of the OS
*/
void detect_wm(void)
{
#if defined (HAVE_X11_XLIB_H)

	Display *disp;
	Atom actual_type;
	int actual_format;
	unsigned long nitems, bytes;
	char *wm_name = '\0';
	Window *wm_check_window;

	if ((disp = XOpenDisplay(NULL)))
	{
		if (!(XGetWindowProperty(disp, DefaultRootWindow(disp),
			XInternAtom(disp, "_NET_SUPPORTING_WM_CHECK", true),
			0, KB, false,	XA_WINDOW, &actual_type, &actual_format, &nitems,
			&bytes, (unsigned char **) &wm_check_window)))
		{
			if (!(XGetWindowProperty(disp, *wm_check_window,
				XInternAtom(disp, "_NET_WM_NAME", true), 0, KB, false,
				XInternAtom(disp, "UTF8_STRING", true),	&actual_type,
				&actual_format, &nitems, &bytes, (unsigned char **) &wm_name)))
			{
				safe_strncpy(wm_str, wm_name, MAX_STRLEN);
				XFree(wm_name);
			}
			else if (error)
				ERR_REPORT(_("No _NET_WM_NAME property found."));

			XFree(wm_check_window);
		}
		else if (error)
			ERR_REPORT(_("No WM detected (non-EWMH compliant?)"));

		XCloseDisplay(disp);
	}
	else 
	{
		safe_strncpy(wm_str, _("No X Server"), MAX_STRLEN);

		if (error)
			ERR_REPORT(_("Could not open an X display. (detect_wm)"));
	}
#else	/* fallback to the detectwm bash script */

	FILE *wm_file;

	wm_file = popen("detectwm 2> /dev/null", "r");
	fgets(wm_str, MAX_STRLEN, wm_file);
	pclose(wm_file);

#endif

	return;
}

/*	detect_wm_theme
	detects the theme associated with the WM detected in detect_wm()
	--
	CAVEAT: This function relies on the presence of 'detectwmtheme', a shell script. 
	If it isn't present somewhere in the PATH, the WM Theme will be set as 'Unknown'
	--
*/
void detect_wm_theme(void)
{
	char exec_str[MAX_STRLEN];
	FILE *wm_theme_file;

	snprintf(exec_str, MAX_STRLEN, "detectwmtheme %s 2> /dev/null", wm_str);

	wm_theme_file = popen(exec_str, "r");
	fgets(wm_theme_str, MAX_STRLEN, wm_theme_file);
	pclose(wm_theme_file);

	return;
}

/*	detect_gtk
	detects the theme, icon(s), and font(s) associated with a GTK DE (if present)
	--
	CAVEAT: This function relies on the presence of 'detectgtk', a shell script.
	If it isn't present somewhere in the PATH, the GTK will be set as 'Unknown'
	--
*/
void detect_gtk(void)
{
	FILE *gtk_file;
	char gtk2_str[MAX_STRLEN]  = "Unknown";
	char gtk3_str[MAX_STRLEN]  = "Unknown";
	char gtk_icons[MAX_STRLEN] = "Unknown";
	char fonts[MAX_STRLEN]	   = "Unknown";

	gtk_file = popen("detectgtk 2> /dev/null", "r");
	fscanf(gtk_file, "%s%s%s%s", gtk2_str, gtk3_str, gtk_icons, fonts);
	pclose(gtk_file);

	if (STREQ(gtk3_str, "Unknown"))
		snprintf(gtk_str, MAX_STRLEN, "%s (%s2), %s (%s)", gtk2_str,
							_("GTK"), gtk_icons, _("Icons"));

	else if (STREQ(gtk2_str, "Unknown"))
		snprintf(gtk_str, MAX_STRLEN, "%s (%s3), %s (%s)", gtk3_str,
							_("GTK"), gtk_icons, _("Icons"));
	else
		snprintf(gtk_str, MAX_STRLEN, "%s (%s2), %s (%s3)", gtk2_str,
									 _("GTK"), gtk3_str, _("GTK"));

	snprintf(icon_str, MAX_STRLEN, "%s", gtk_icons);
	snprintf(font_str, MAX_STRLEN, "%s", fonts);

	return;
}