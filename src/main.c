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
		detect_distro();
		detect_host();
		detect_kernel();
		detect_uptime();
		detect_pkgs();
		detect_cpu();
		detect_gpu();
		detect_disk();
		detect_mem();
		detect_shell();
		detect_res();
		detect_de();
		detect_wm();
		detect_wm_theme();
		detect_gtk();

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
			take_screenshot();
	}
	return EXIT_SUCCESS;
}