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
#include <inttypes.h>

/* program includes */
#include "misc.h"
#include "extern.h"
#include "prototypes.h"

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	textdomain(GETTEXT_PACKAGE);
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");

	if (argc > 1)
		Init_parseR(argc, argv);

	if (!break_requested)
	{
		detect_distro(distro_str);
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

		if (!STREQ(given_distro_str, "Unknown"))
			safe_strncpy(distro_str, given_distro_str, MAX_STRLEN);

		if (verbose)
			main_text_output(true);

		if (portrait)
		{
			main_ascii_output(distro_str);
			main_text_output(false);
		}
		else if (main_logo)
			main_ascii_output("");
		else
			main_text_output(false);

		if (screenshot)
			take_screenshot(verbose, res_str);
	}
	return EXIT_SUCCESS;
}