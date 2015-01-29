/*	main.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	screenfetch-c is a rewrite of screenFetch.sh in C.
 *	This is primarily an experiment borne out of an awareness of the
 *	slow execution time on the screenfetch-dev.sh script. 
 *	Hopefully this rewrite will execute faster, although it's more for
 *	self education than anything else.
 *
 *	If you have any questions, please feel free to contact me via:
 *		- github: http://www.github.com/woodruffw/screenfetch-c
 *		- email: william @ tuffbizz.com
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

/* program includes */
#include "detect.h"
#include "disp.h"
#include "colors.h"
#include "misc.h"
#include "util.h"
#include "flags.h"
#include "arrays.h"


int main(int argc, char **argv)
{
	char given_distro_str[MAX_STRLEN] = "Unknown";

	bool manual = false, logo = true, portrait = false;
	bool verbose = false, screenshot = false;

	static struct option options[] =
	{
		{ "manual", no_argument, 0, 'm' },
		{ "verbose", no_argument, 0, 'v' },
		{ "no-logo", no_argument, 0, 'n' },
		{ "screenshot", no_argument, 0, 's' },
		{ "distro", required_argument, 0, 'D' },
		{ "suppress-errors", no_argument, 0, 'E' },
		{ "portrait", no_argument, 0, 'p'},
		{ "version", no_argument, 0, 'V' },
		{ "help", no_argument, 0, 'h' },
		{ "logo-only", required_argument, 0, 'L' },
		{ 0, 0, 0, 0 }
	};

	signed char c;
	int index = 0;
	while ((c = getopt_long(argc, argv, "mvnsD:EpVhL:", options, &index)) != -1)
	{
		switch (c)
		{
			case 'm': manual = true;
					  break;
			case 'v': verbose = true;
					  break;
			case 'n': logo = false;
					  break;
			case 's': screenshot = true;
					  break;
			case 'D': SET_GIVEN_DISTRO(optarg);
					  break;
			case 'E': error = false;
					  break;
			case 'p': portrait = true;
					  break;
			case 'V': display_version();
					  return EXIT_SUCCESS;
			case 'h': display_help();
					  return EXIT_SUCCESS;
			case 'L': main_ascii_output(optarg);
					  return EXIT_SUCCESS;
			default:
					return EXIT_FAILURE;
		}
	}

	if (manual) /* triggered by -m (--manual) flag */
	{
		int stat = manual_input(detected_arr, verbose);

		if (stat == EXIT_SUCCESS)
		{
			/* these sections are ALWAYS detected */
			detect_uptime(uptime_str);
			detect_pkgs(pkgs_str, distro_str);
			detect_disk(disk_str);
			detect_mem(mem_str);

			/* if the user specifies an asterisk, fill the data in for them */
			if (STREQ(distro_str, "*"))
				detect_distro(distro_str);
			/*if (STREQ(arch_str, "*"))
				detect_arch(arch_str);*/
			if (STREQ(host_str, "*"))
				detect_host();
			if (STREQ(kernel_str, "*"))
				detect_kernel(kernel_str);
			if (STREQ(cpu_str, "*"))
				detect_cpu(cpu_str);
			if (STREQ(gpu_str, "*"))
				detect_gpu(gpu_str);
			if (STREQ(shell_str, "*"))
				detect_shell(shell_str);
			if (STREQ(res_str, "*"))
				detect_res(res_str);
			if (STREQ(de_str, "*"))
				detect_de(de_str);
			if (STREQ(wm_str, "*"))
				detect_wm(wm_str);
			if (STREQ(wm_theme_str, "*"))
				detect_wm_theme(wm_theme_str, wm_str);
			if (STREQ(gtk_str, "*"))
				detect_gtk(gtk_str, icon_str, font_str);
		}

		else /* if the user decided to leave manual mode without input */
			return EXIT_SUCCESS;
	}
	else /* each string is filled by its respective function */
	{
		detect_distro(distro_str);
		/*detect_arch(arch_str);*/
		detect_host();
		detect_kernel(kernel_str);
		detect_uptime(uptime_str);
		detect_pkgs(pkgs_str, distro_str);
		detect_cpu(cpu_str);
		detect_gpu(gpu_str);
		detect_disk(disk_str);
		detect_mem(mem_str);
		detect_shell(shell_str);
		detect_res(res_str);
		detect_de(de_str);
		detect_wm(wm_str);
		detect_wm_theme(wm_theme_str, wm_str);
		detect_gtk(gtk_str, icon_str, font_str);
	}

	/* if the user specified a different OS to display, set distro_set to it */
	if (!STREQ(given_distro_str, "Unknown"))
		safe_strncpy(distro_str, given_distro_str, MAX_STRLEN);

	if (verbose)
		main_text_output(true);

	if (portrait)
	{
		main_ascii_output(distro_str);
		main_text_output(false);
	}
	else if (logo)
		main_ascii_output("");
	else
		main_text_output(false);

	if (screenshot)
		take_screenshot(verbose, res_str);

	return EXIT_SUCCESS;
}