/*	detect.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	The detection functions used by screenfetch-c on Solaris are implemented here.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

/* Solaris-specific includes */
#include <utmpx.h>
#include <time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

/* program includes */
#include "../../misc.h"
#include "../../extern.h"
#include "../../prototypes.h"

/*	detect_distro
	detects the computer's distribution
	argument char *str: the char array to be filled with the distro name
*/
void detect_distro(char *str1)
{
	struct utsname distro_info;

	uname(&distro_info);
	snprintf(str1, MAX_STRLEN, "%s", distro_info.sysname);

	return;
}

/*	detect_host
	detects the computer's hostname and active user and formats them
	argument char *str: the char array to be filled with the host info
*/
void detect_host(void)
{
	char *given_user = "Unknown";
	char given_host[MAX_STRLEN] = "Unknown";
	struct utsname host_info;

	given_user = getlogin();
	uname(&host_info);
	safe_strncpy(given_host, host_info.nodename, MAX_STRLEN);

	snprintf(UseR, MAX_STRLEN, "%s", given_user);
	snprintf(HosT, MAX_STRLEN, "%s", given_host);

	return;
}

/*	detect_kernel
	detects the computer's kernel
	argument char *str: the char array to be filled with the kernel name
*/
void detect_kernel(char *str)
{
	struct utsname kern_info;

	uname(&kern_info);
	snprintf(str, MAX_STRLEN, "%s", kern_info.release);

	return;
}

/*	detect_uptime
	detects the computer's uptime
	argument char *str: the char array to be filled with the uptime
*/
void detect_uptime(char *str)
{
	long uptime = 0, currtime = 0, boottime = 0;
	uint_least32_t secs = 0, mins = 0, hrs = 0, days = 0;
	struct utmpx *ent;

	currtime = time(NULL);

	while ((ent = getutxent()))
	{
		if (STREQ("system boot", ent->ut_line))
			boottime = ent->ut_tv.tv_sec;
	}

	uptime = currtime - boottime;

	split_uptime(uptime, &secs, &mins, &hrs, &days);

	if (days > 0)
		snprintf(str, MAX_STRLEN,
			"%"PRIuLEAST32"d %"PRIuLEAST32"h %"PRIuLEAST32"m %"PRIuLEAST32"s",
			days, hrs, mins, secs);
	else
		snprintf(str, MAX_STRLEN,
			"%"PRIuLEAST32"h %"PRIuLEAST32"m %"PRIuLEAST32"s",
			hrs, mins, secs);

	return;
}

/*	detect_pkgs
	detects the number of packages installed on the computer
	argument char *str: the char array to be filled with the number of packages
*/
void detect_pkgs(char *str, const char *distro_str)
{
	FILE *pkgs_file;
	uint_fast16_t packages = 0;

	pkgs_file = popen("pkg list | wc -l", "r");
	fscanf(pkgs_file, "%"SCNuFAST16, &packages);
	pclose(pkgs_file);

	snprintf(str, MAX_STRLEN, "%"PRIuFAST16, packages);

	return;
}

/*	detect_cpu
	detects the computer's CPU brand/name-string
	argument char *str: the char array to be filled with the CPU name
*/
void detect_cpu(char *str)
{
	FILE *cpu_file;

	cpu_file = popen("psrinfo -pv | tail -1 | tr -d '\\t\\n'", "r");
	fgets(str, MAX_STRLEN, cpu_file);
	pclose(cpu_file);

	remove_excess_cpu_txt(str);

	return;
}

/*	detect_gpu
	detects the computer's GPU brand/name-string
	argument char *str: the char array to be filled with the GPU name
*/
void detect_gpu(char *str)
{
	FILE *gpu_file;

	gpu_file = popen("detectgpu 2>/dev/null", "r");
	fgets(str, MAX_STRLEN, gpu_file);
	pclose(gpu_file);

	return;
}

/*	detect_disk
	detects the computer's total disk capacity and usage
	argument char *str: the char array to be filled with the disk data
*/
void detect_disk(char *str)
{
	struct statvfs disk_info;
	uintmax_t disk_total = 0, disk_used = 0, disk_percentage = 0;

	if (!(statvfs(getenv("HOME"), &disk_info)))
	{
		disk_total = ((disk_info.f_blocks * disk_info.f_bsize) / GB);
		disk_used = (((disk_info.f_blocks - disk_info.f_bfree) * disk_info.f_bsize) / GB);
		disk_percentage = (((float) disk_used / disk_total) * 100);
		snprintf(str, MAX_STRLEN, "%"PRIuMAX"G / %"PRIuMAX"G (%"PRIuMAX"%%)",
			disk_used, disk_total, disk_percentage);
	}
	else if (error)
		ERR_REPORT(_("Could not stat $HOME for filesystem statistics."));

	return;
}

/*	detect_mem
	detects the computer's total and used RAM
	argument char *str: the char array to be filled with the memory data
*/
void detect_mem(char *str)
{
	uintmax_t total_mem = 0;

	total_mem = (uintmax_t) (sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE));
	total_mem /= (uintmax_t) MB;

	/* sar -r 1 to get free pages? */

	snprintf(str, MAX_STRLEN, "%"PRIuMAX"%s", total_mem, "MB");

	return;
}

/*	detect_res
	detects the combined resolution of all monitors attached to the computer
	argument char *str: the char array to be filled with the resolution
*/
void detect_res(char *str)
{
	int width = 0;
	int height = 0;
	Display *disp;
	Screen *screen;

	if ((disp = XOpenDisplay(NULL)))
	{
		screen = XDefaultScreenOfDisplay(disp);
		width = WidthOfScreen(screen);
		height = HeightOfScreen(screen);
		snprintf(str, MAX_STRLEN, "%dx%d", width, height);
	}
	else
	{
		safe_strncpy(str, "No X Server", MAX_STRLEN);

		if (error)
			ERR_REPORT(_("Could not open an X display (detect_res)"));
	}

	XCloseDisplay(disp);

	return;
}

/*	detect_de
	detects the desktop environment currently running on top of the OS
	argument char *str: the char array to be filled with the DE name
*/
void detect_de(char *str)
{
	char *curr_de;

	if ((curr_de = getenv("XDG_CURRENT_DESKTOP")))
		safe_strncpy(str, curr_de, MAX_STRLEN);

	else
	{
		if (getenv("GNOME_DESKTOP_SESSION_ID"))
			safe_strncpy(str, "Gnome", MAX_STRLEN);

		else if (getenv("MATE_DESKTOP_SESSION_ID"))
			safe_strncpy(str, "MATE", MAX_STRLEN);

		else if (getenv("KDE_FULL_SESSION"))
			/*	KDE_SESSION_VERSION only exists on KDE4+, so 
				getenv will return NULL on KDE <= 3.
			 */
			snprintf(str, MAX_STRLEN, "KDE%s", getenv("KDE_SESSION_VERSION"));

		else if (error)
			ERR_REPORT(_("No desktop environment found."));
	}

	return;
}

/*	detect_wm
	detects the window manager currently running on top of the OS
	argument char *str: the char array to be filled with the WM name
	--
	CAVEAT: This function relies on the presence of 'detectwm', a shell script. 
	If it isn't present somewhere in the PATH, the WM Theme will be set as 'Unknown'
	--
*/
void detect_wm(char *str)
{
	Display *disp;
	Atom actual_type;
	int actual_format;
	unsigned long nitems;
	unsigned long bytes;
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
				safe_strncpy(str, wm_name, MAX_STRLEN);
				XFree(wm_name);
			}
			else if (error)
				ERR_REPORT(_("No _NET_WM_NAME property found."));

			XFree(wm_check_window);
		}
		else if (error)
			ERR_REPORT(_("WM cannot be detected without EWMH compliance."));

		XCloseDisplay(disp);
	}
	else 
	{
		safe_strncpy(str, _("No X Server"), MAX_STRLEN);

		if (error)
			ERR_REPORT(_("Could not open an X display. (detect_wm)"));
	}

	return;
}

/*	detect_wm_theme
	detects the theme associated with the WM detected in detect_wm()
	argument char *str: the char array to be filled with the WM Theme name
	--
	CAVEAT: This function relies on the presence of 'detectwmtheme', a shell script. 
	If it isn't present somewhere in the PATH, the WM Theme will be set as 'Unknown'
	--
*/
void detect_wm_theme(char *str, const char *wm_str)
{
	char exec_str[MAX_STRLEN];
	FILE *wm_theme_file;

	snprintf(exec_str, MAX_STRLEN, "detectwmtheme %s 2> /dev/null", wm_str);

	wm_theme_file = popen(exec_str, "r");
	fgets(str, MAX_STRLEN, wm_theme_file);
	pclose(wm_theme_file);

	return;
}

/*	detect_gtk
	detects the theme, icon(s), and font(s) associated with a GTK DE (if present)
	argument char *str: the char array to be filled with the GTK info
	--
	CAVEAT: This function relies on the presence of 'detectgtk', a shell script. 
	If it isn't present somewhere in the PATH, the WM Theme will be set as 'Unknown'
	--
*/
void detect_gtk(char *str1, char *str2, char *str3)
{
	safe_strncpy(str, "Not implemented", MAX_STRLEN);

	return;
}
