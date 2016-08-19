/*	detect.c
**	Author: William Woodruff
**	-------------
**
**	The detection functions used by screenfetch-c on Windows (Cygwin) are implemented here.
**	Like the rest of screenfetch-c, this file is licensed under the MIT license.
*/

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>
#include <glob.h>

/* Windows-specific includes */
#include <Windows.h>
/* Cygwin wets itself without an explicit external linkage to popen */
extern FILE *popen(const char *command, const char *type);
extern int pclose(FILE *stream);

/* program includes */
#include "../../arrays.h"
#include "../../colors.h"
#include "../../misc.h"
#include "../../disp.h"
#include "../../util.h"
#include "../../error_flag.h"

/*	detect_distro
	detects the computer's distribution (Windows version)
*/
void detect_distro(void)
{
	OSVERSIONINFO vers_info;
	int major, minor;

	vers_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&vers_info);
	major = vers_info.dwMajorVersion;
	minor = vers_info.dwMinorVersion;

	/* if distro_str was NOT set by the -D flag */
	if (STREQ(distro_str, "Unknown"))
	{
		switch (major)
		{
			case 10:
				safe_strncpy(distro_str, "Microsoft Windows 10", MAX_STRLEN);
				break;
			case 6:
				switch (minor)
				{
					case 3:
						safe_strncpy(distro_str, "Microsoft Windows 8.1", MAX_STRLEN);
						break;
					case 2:
						safe_strncpy(distro_str, "Microsoft Windows 8", MAX_STRLEN);
						break;
					case 1:
						safe_strncpy(distro_str, "Microsoft Windows 7", MAX_STRLEN);
						break;
					case 0:
						safe_strncpy(distro_str, "Microsoft Windows Vista", MAX_STRLEN);
						break;
				}
				break;
			case 5:
				switch (minor)
				{
					case 1:
						safe_strncpy(distro_str, "Microsoft Windows XP", MAX_STRLEN);
						break;
					case 0:
						safe_strncpy(distro_str, "Microsoft Windows 2000", MAX_STRLEN);
						break;
				}
				break;
			default:
				safe_strncpy(distro_str, "Microsoft Windows", MAX_STRLEN);
				break;
		}
	}

	safe_strncpy(host_color, TRED, MAX_STRLEN);

	return;
}

/*	detect_host
	detects the computer's hostname and active user and formats them
*/
void detect_host(void)
{
	char *given_user = "Unknown";
	char given_host[MAX_STRLEN] = "Unknown";

	given_user = malloc(sizeof(char) * MAX_STRLEN);
	if (!given_user)
	{
		ERR_REPORT("Memory allocation failed in detect_host.");
		exit(1);
	}

	unsigned long len = MAX_STRLEN;
	GetUserName(given_user, &len);
	gethostname(given_host, MAX_STRLEN);

	snprintf(host_str, MAX_STRLEN, "%s%s%s%s@%s%s%s%s",
		host_color, given_user, TNRM, TWHT, TNRM, host_color, given_host, TNRM);

	free(given_user);

	return;
}

/*	detect_kernel
	detects the computer's kernel
*/
void detect_kernel(void)
{
	OSVERSIONINFO kern_info;
	SYSTEM_INFO arch_info;
	char arch_str[MAX_STRLEN];

	ZeroMemory(&kern_info, sizeof(OSVERSIONINFO));
	kern_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&kern_info);

	GetNativeSystemInfo(&arch_info);

	if (arch_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	{
		safe_strncpy(arch_str, "AMD64", MAX_STRLEN);
	}
	else if (arch_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
	{
		safe_strncpy(arch_str, "ARM", MAX_STRLEN);
	}
	else if (arch_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		safe_strncpy(arch_str, "IA64", MAX_STRLEN);
	}
	else if (arch_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
	{
		safe_strncpy(arch_str, "x86", MAX_STRLEN);
	}
	else
	{
		safe_strncpy(arch_str, "Unknown", MAX_STRLEN);
	}

	snprintf(kernel_str, MAX_STRLEN, "Windows NT %d.%d build %d (%s)",
			(int) kern_info.dwMajorVersion, (int) kern_info.dwMinorVersion,
			(int) kern_info.dwBuildNumber, arch_str);

	return;
}


/*	detect_uptime
	detects the computer's uptime
*/
void detect_uptime(void)
{
	long uptime = 0;
	unsigned int secs = 0;
	unsigned int mins = 0;
	unsigned int hrs = 0;
	unsigned int days = 0;

	uptime = GetTickCount(); /* known problem: will rollover after 49.7 days */
	uptime /= 1000;

	split_uptime(uptime, &secs, &mins, &hrs, &days);

	if (days > 0)
		snprintf(uptime_str, MAX_STRLEN, "%dd %dh %dm %ds", days, hrs, mins, secs);
	else
		snprintf(uptime_str, MAX_STRLEN, "%dh %dm %ds", hrs, mins, secs);

	return;
}

/*	detect_pkgs
	detects the number of packages installed on the computer
*/
void detect_pkgs(void)
{
	const char *pattern;
	glob_t globbuf;

#if defined(__MSYS__)
	/* MSYS2 */
	/* ignores: ALPM_DB_VERSION */
	pattern = "/var/lib/pacman/local/*-*";
#else
	/* Cygwin */
	/* ignores: installed.db setup.rc timestamp */
	pattern = "/etc/setup/*.lst.*";
#endif

	globbuf.gl_offs = 1;
	glob(pattern, GLOB_DOOFFS, NULL, &globbuf);
	snprintf(pkgs_str, MAX_STRLEN, "%d", (int) globbuf.gl_pathc);
	globfree(&globbuf);

	return;
}

/*	detect_cpu
	detects the computer's CPU brand/name-string
*/
void detect_cpu(void)
{
	HKEY hkey;
	DWORD str_size = MAX_STRLEN;

	RegOpenKey(HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &hkey);
	RegQueryValueEx(hkey, "ProcessorNameString", 0, NULL, (BYTE *) cpu_str,
			&str_size);

	return;
}

/*	detect_gpu
	detects the computer's GPU brand/name-string
*/
void detect_gpu(void)
{
	HKEY hkey;
	DWORD str_size = MAX_STRLEN;

	RegOpenKey(HKEY_LOCAL_MACHINE,
			"SYSTEM\\ControlSet001\\Control\\Class\\"
			"{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000\\Settings", &hkey);
	RegQueryValueEx(hkey, "Device Description", 0, NULL, (BYTE *) gpu_str,
			&str_size);

	return;
}

/*	detect_disk
	detects the computer's total disk capacity and usage
*/
void detect_disk(void)
{
	FILE *disk_file;
	int disk_total = 0;
	int disk_used = 0;
	int disk_percentage = 0;

	/* GetDiskFreeSpaceEx? */

	disk_file = popen("df -H 2> /dev/null | grep -vE '^[A-Z]\\:\\/|File' | awk "
			"'{ print $2 }' | head -1 | tr -d '\\r\\n G'", "r");
	fscanf(disk_file, "%d", &disk_total);
	pclose(disk_file);

	disk_file = popen("df -H 2> /dev/null | grep -vE '^[A-Z]\\:\\/|File' | awk "
			"'{ print $3 }' | head -1 | tr -d '\\r\\n G'", "r");
	fscanf(disk_file, "%d", &disk_used);
	pclose(disk_file);

	if (disk_total > disk_used)
	{
		disk_percentage = (((float) disk_used / disk_total) * 100);

		snprintf(disk_str, MAX_STRLEN, "%dG / %dG (%d%%)", disk_used, disk_total,
				disk_percentage);
	}
	else /* when disk_used is in a smaller unit */
	{
		disk_percentage = ((float) disk_used / (disk_total * 1024) * 100);

		snprintf(disk_str, MAX_STRLEN, "%dM / %dG (%d%%)", disk_used, disk_total,
				disk_percentage);
	}

	return;
}

/*	detect_mem
	detects the computer's total and used RAM
*/
void detect_mem(void)
{
	long long total_mem = 0;
	long long used_mem = 0;
	MEMORYSTATUSEX mem_stat;

	mem_stat.dwLength = sizeof(mem_stat);
	GlobalMemoryStatusEx(&mem_stat);

	total_mem = (long long) mem_stat.ullTotalPhys / MB;
	used_mem = total_mem - ((long long) mem_stat.ullAvailPhys / MB);

	snprintf(mem_str, MAX_STRLEN, "%lld%s / %lld%s", used_mem, "MB", total_mem, "MB");

	return;
}

/*	detect_shell
	detects the shell currently running on the computer
	--
	CAVEAT: shell version detection relies on the standard versioning format for
	each shell. If any shell's older (or newer versions) suddenly begin to use a new
	scheme, the version may be displayed incorrectly.
	--
*/
void detect_shell(void)
{
	FILE *shell_file;
	char *shell_name;
	char vers_str[MAX_STRLEN];

	shell_name = getenv("SHELL");

	if (shell_name == NULL)
	{
		if (error)
			ERR_REPORT("Could not detect a shell.");

		return;
	}

	if (STREQ(shell_name, "/bin/sh"))
	{
		safe_strncpy(shell_str, "POSIX sh", MAX_STRLEN);
	}
	else if (strstr(shell_name, "bash"))
	{
		shell_file = popen("bash --version | head -1", "r");
		fgets(vers_str, MAX_STRLEN, shell_file);
		snprintf(shell_str, MAX_STRLEN, "bash %.*s", 17, vers_str + 10);
		pclose(shell_file);
	}
	else if (strstr(shell_name, "zsh"))
	{
		shell_file = popen("zsh --version", "r");
		fgets(vers_str, MAX_STRLEN, shell_file);
		snprintf(shell_str, MAX_STRLEN, "zsh %.*s", 5, vers_str + 4);
		pclose(shell_file);
	}
	else if (strstr(shell_name, "csh"))
	{
		shell_file = popen("csh --version | head -1", "r");
		fgets(vers_str, MAX_STRLEN, shell_file);
		snprintf(shell_str, MAX_STRLEN, "csh %.*s", 7, vers_str + 5);
		pclose(shell_file);
	}
	else if (strstr(shell_name, "fish"))
	{
		shell_file = popen("fish --version", "r");
		fgets(vers_str, MAX_STRLEN, shell_file);
		snprintf(shell_str, MAX_STRLEN, "fish %.*s", 13, vers_str + 6);
		pclose(shell_file);
	}
	else if (strstr(shell_name, "dash") || strstr(shell_name, "ash")
			|| strstr(shell_name, "ksh"))
	{
		/* i don't have a version detection system for these, yet */
		safe_strncpy(shell_str, shell_name, MAX_STRLEN);
	}

	return;
}

/*	detect_res
	detects the combined resolution of all monitors attached to the computer
*/
void detect_res(void)
{
	int width = 0;
	int height = 0;

	width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	snprintf(res_str, MAX_STRLEN, "%dx%d", width, height);

	return;
}

/*	detect_de
	detects the desktop environment currently running on top of the OS.
	On Windows, this will always be either Luna, Aero, or Metro.
*/
void detect_de(void)
{
	OSVERSIONINFO vers_info;
	int major, minor;

	vers_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&vers_info);
	major = vers_info.dwMajorVersion;
	minor = vers_info.dwMinorVersion;

	if (major == 10)
		safe_strncpy(de_str, "Modern UI/Metro", MAX_STRLEN);
	else if (major == 6 && minor >= 2)
		safe_strncpy(de_str, "Metro", MAX_STRLEN);
	else if (major == 6 && minor <= 1)
		safe_strncpy(de_str, "Aero", MAX_STRLEN);
	else if (major == 5)
		safe_strncpy(de_str, "Luna", MAX_STRLEN);

	return;
}

/*	detect_wm
	detects the window manager currently running on top of the OS.
	On Windows, this will always be DWM/Explorer.
*/
void detect_wm(void)
{
	safe_strncpy(wm_str, "DWM/Explorer", MAX_STRLEN);

	return;
}

/*	detect_wm_theme
	On Windows, detects the current theme running on DWM.
*/
void detect_wm_theme(void)
{
	char tmp_theme[MAX_STRLEN] = "Unknown";
	char *suffix;
	HKEY hkey;
	DWORD str_size = MAX_STRLEN;

	RegOpenKey(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Themes", &hkey);
	RegQueryValueEx(hkey, "CurrentTheme", 0, NULL, (BYTE *) tmp_theme,
		&str_size);

	/* if we successfully retrieved a theme from the registry */
	if (!STREQ(tmp_theme, "Unknown"))
	{
		safe_strncpy(tmp_theme, basename(tmp_theme), MAX_STRLEN);
		suffix = tmp_theme + strlen(tmp_theme) - 6;

		/* if our retrieved theme ends with .theme, truncate it */
		if (STREQ(suffix, ".theme"))
		{
			tmp_theme[strlen(tmp_theme) - 6] = '\0';
		}
	}

	safe_strncpy(wm_theme_str, tmp_theme, MAX_STRLEN);

	return;
}

/*	detect_gtk
	On Windows, detects the font associated with Cygwin's terminal (mintty)
*/
void detect_gtk(void)
{
	FILE *gtk_file;
	char font_str[MAX_STRLEN] = "Unknown";

	gtk_file = popen("grep '^Font=.*' < $HOME/.minttyrc | "
			"grep -o '[0-9A-z ]*$' | tr -d '\\r\\n'", "r");
	fgets(font_str, MAX_STRLEN, gtk_file);
	pclose(gtk_file);

	snprintf(gtk_str, MAX_STRLEN, "%s (Font)", font_str);

	return;
}
