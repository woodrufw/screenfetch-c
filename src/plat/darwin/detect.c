/*	detect.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	The detection functions used by screenfetch-c on OS X (Darwin) are implemented here.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

/* OS X-specific includes */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/utsname.h>
#include <time.h>
#include <glob.h>
#include <Availability.h>
#include <mach/mach_time.h>
#if __MAC_OS_X_VERSION_MIN_REQUIRED <= 1070
	#include <CoreServices/CoreServices.h> /* for Gestalt */
#endif

/* program includes */
#include "../../misc.h"
#include "../../extern.h"
#include "../../prototypes.h"

/*	detect_distro
	detects the computer's distribution (OS X release)
*/
void detect_distro(void)
{
#if __MAC_OS_X_VERSION_MIN_REQUIRED <= 1070
	int ver_maj, ver_min, ver_bug;
#else
	FILE *distro_file;
#endif

	/*
		Use this:
		https://www.opensource.apple.com/source/DarwinTools/DarwinTools-1/sw_vers.c
	*/
#if __MAC_OS_X_VERSION_MIN_REQUIRED <= 1070
	Gestalt(gestaltSystemVersionMajor, (SInt32 *) &ver_maj);
	Gestalt(gestaltSystemVersionMinor, (SInt32 *) &ver_min);
	Gestalt(gestaltSystemVersionBugFix, (SInt32 *) &ver_bug);

	snprintf(distro_str, MAX_STRLEN, "Max OS X %d.%d.%d", ver_maj, ver_min, ver_bug);
#else
	distro_file = popen("sw_vers -productVersion | tr -d '\\n'", "r");
	fgets(distro_name_str, MAX_STRLEN, distro_file);
	pclose(distro_file);

	snprintf(distro_str, MAX_STRLEN, "Mac OS X %s", distro_name_str);
#endif

	return;
}

/*	detect_host
	detects the computer's hostname and active user and formats them
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
*/
void detect_kernel(void)
{
	struct utsname kern_info;

	uname(&kern_info);
	snprintf(kernel_str, MAX_STRLEN, "%s %s %s", kern_info.sysname,
			kern_info.release, kern_info.machine);

	return;
}

/*	detect_uptime
	detects the computer's uptime
*/
void detect_uptime(void)
{
	long uptime = 0;
	uint_least32_t secs = 0, mins = 0, hrs = 0, days = 0;

	/* three cheers for undocumented functions and structs */
	static mach_timebase_info_data_t timebase_info;

	if (timebase_info.denom == 0)
	{
		(void) mach_timebase_info(&timebase_info);
	}

	uptime = (long)((mach_absolute_time() * timebase_info.numer) /
				(1000* 1000 * timebase_info.denom));
	uptime /= 1000;

	split_uptime(uptime, &secs, &mins, &hrs, &days);

	if (days > 0)
		snprintf(uptime_str, MAX_STRLEN,
			"%"PRIuLEAST32"d %"PRIuLEAST32"h %"PRIuLEAST32"m %"PRIuLEAST32"s",
			days, hrs, mins, secs);
	else
		snprintf(uptime_str, MAX_STRLEN,
			"%"PRIuLEAST32"h %"PRIuLEAST32"m %"PRIuLEAST32"s",
			hrs, mins, secs);

	return;
}

/*	detect_pkgs
	detects the number of packages installed on the computer
*/
void detect_pkgs(void)
{
	uint_fast16_t packages = 0;
	glob_t gl;

	if (glob("/usr/local/Cellar/*", GLOB_NOSORT, NULL, &gl) == 0)
		packages = gl.gl_pathc;

	else if (error)
		ERR_REPORT(_("Failure while globbing packages."));

	globfree(&gl);

	snprintf(pkgs_str, MAX_STRLEN, "%"PRIuFAST16, packages);

	return;
}

/*	detect_cpu
	detects the computer's CPU brand/name-string
*/
void detect_cpu(void)
{
	FILE *cpu_file;

	/*
		something like:
		int len = MAX_STRLEN;
		sysctlbyname("machdep.cpu.brand_string", cpu_str, &len, NULL, 0);
	*/
	cpu_file = popen("sysctl -n machdep.cpu.brand_string | "
				"sed 's/(\\([Tt][Mm]\\))//g;s/(\\([Rr]\\))//g;s/^//g' | "
				"tr -d '\\n' | tr -s ' '", "r");
	fgets(cpu_str, MAX_STRLEN, cpu_file);
	pclose(cpu_file);

	remove_excess_cpu_txt();

	return;
}

/*	detect_gpu
	detects the computer's GPU brand/name-string
*/
void detect_gpu(void)
{
	FILE *gpu_file;

	gpu_file = popen("system_profiler SPDisplaysDataType | "
				"awk -F': ' '/^\\ *Chipset Model:/ {print $2}' | "
				"tr -d '\\n'", "r");
	fgets(gpu_str, MAX_STRLEN, gpu_file);
	pclose(gpu_file);

	return;
}

/*	detect_disk
	detects the computer's total disk capacity and usage
*/
void detect_disk(void)
{
	struct statfs disk_info;
	uintmax_t disk_total = 0, disk_used = 0, disk_percentage = 0;

	if (!(statfs(getenv("HOME"), &disk_info)))
	{
		disk_total = ((disk_info.f_blocks * disk_info.f_bsize) / GB);
		disk_used = (((disk_info.f_blocks - disk_info.f_bfree)
					* disk_info.f_bsize) / GB);
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
	FILE *mem_file;
	uintmax_t total_mem = 0, used_mem = 0, free_mem;

	mem_file = popen("sysctl -n hw.memsize", "r");
	fscanf(mem_file, "%"SCNuMAX, &total_mem);
	pclose(mem_file);

	mem_file = popen("vm_stat | head -2 | tail -1 | tr -d 'Pages free: .'", "r");
	fscanf(mem_file, "%"SCNuMAX, &free_mem);
	pclose(mem_file);

	total_mem /= (uintmax_t) MB;

	free_mem *= 4096; /* 4KiB is OS X's page size, only if 64-bit */
	free_mem /= (uintmax_t) MB;

	used_mem = total_mem - free_mem;

	snprintf(mem_str, MAX_STRLEN, "%"PRIuMAX"%s / %"PRIuMAX"%s", used_mem, "MB", total_mem, "MB");

	return;
}

/*	detect_res
	detects the combined resolution of all monitors attached to the computer
*/
void detect_res(void)
{
	FILE *res_file;

	res_file = popen("system_profiler SPDisplaysDataType | "
				"awk '/Resolution:/ {print $2\"x\"$4}' | tr -d '\\n'", "r");
	fgets(res_str, MAX_STRLEN, res_file);
	pclose(res_file);

	return;
}

/*	detect_de
	detects the desktop environment currently running on top of the OS.
	On OS X, this will always be Aqua.
*/
void detect_de(void)
{
	safe_strncpy(de_str, "Aqua", MAX_STRLEN);

	return;
}

/*	detect_wm
	detects the window manager currently running on top of the OS.
	On OS X, this will always be the Quartz Compositor.
*/
void detect_wm(void)
{
	safe_strncpy(wm_str, "Quartz Compositor", MAX_STRLEN);

	return;
}

/*	detect_wm_theme
	detects the theme associated with the WM detected in detect_wm().
	On OS X, this will always be Aqua.
*/
void detect_wm_theme(void)
{
	safe_strncpy(wm_theme_str, "Aqua", MAX_STRLEN);

	return;
}

/*	detect_gtk
	OS X doesn't use GTK, so this function fills str with "Not Applicable"
*/
void detect_gtk(void)
{
	safe_strncpy(gtk_str, "Not Applicable", MAX_STRLEN);

	return;
}