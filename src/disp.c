/*	disp.c
 *	Author: William Woodruff
 *	-------------
 *  Edited by: Aaron Caffrey (trimmed 816 lines down to 155,
 *  while preserving the same screenfetch-c functionalities)
 *
 *	The functions that output the detected data are defined here.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/* program includes */
#include "misc.h"
#include "extern.h"
#include "prototypes.h"
#include "structs.h"

#define DETECTED_ARR_LEN 17

/* process_data
   print the detected and passed distro arguments
*/
void process_data(const char *logo[], uint_fast16_t num, char *colour)
{
	uint_fast16_t x = 0;

	if (process_logo_only)
		for (x = 0; x < num; x++)
			printf("%s\n", logo[x]);
	else
	{
		snprintf(host_str, MAX_STRLEN, "%s%s%s%s@%s%s%s%s",
			colour, UseR, TNRM, TWHT, TNRM, colour, HosT, TNRM);

		for (x = 0; x < num; x++)
			if (x < DETECTED_ARR_LEN)
				printf("%s %s%s%s%s%s\n", logo[x], TNRM, colour,
					_(detected_arr_names[x]), TNRM, _(detected_arr[x]));
			else
				printf("%s\n", logo[x]);
	}

	return;
}

/*	main_text_output
	the secondary output for screenfetch-c - all info WITHOUT ASCII art
	arguments bool verbose
*/
void main_text_output(bool verbose)
{
	uint_fast16_t i = 0;

	sprintf(host_str, "%s@%s", UseR, HosT);

	if (verbose)
		for (i = 0; i < DETECTED_ARR_LEN; i++)
			VERBOSE_OUT(_(detected_arr_names[i]), _(detected_arr[i]));
	else
		for (i = 0; i < DETECTED_ARR_LEN; i++)
			printf("%s %s%s\n", _(detected_arr_names[i]), TNRM, _(detected_arr[i]));

	return;
}

/*	main_ascii_output
	the primary output for screenfetch-c
*/
void main_ascii_output(char *passed_distro)
{
	bool distro_match = false;
	uint_fast16_t x = 0;
	char distro[MAX_STRLEN];

	process_logo_only = (STREQ(passed_distro, "") ? false : true);

	sprintf(distro, "%s", (STREQ(passed_distro, "") ? detected_arr[1] : passed_distro));

	for (x = 0; x < MAD_DOG_LEN; x++)
		if (STREQ(distro, opt[x].distro))
		{
			distro_match = true;
			break;
		}

	if (distro_match)
		process_data(opt[x].logo, opt[x].len, opt[x].colour);

	else
	{
		if (strstr(distro, "Microsoft"))
			process_data(windows_logo, MICROSOFT_LEN, TRED);

		else if (strstr(distro, "OS X"))
			process_data(macosx_logo, MACOSX_LEN, TLBL);

		else if (STREQ(distro, "Ubuntu") || STREQ(distro, "Lubuntu")
				|| STREQ(distro, "Xubuntu"))
			process_data(ubuntu_logo, UBUNTU_LEN, TLRD);

		else if (STREQ(distro, "Mandriva") || STREQ(distro, "Mandrake"))
			process_data(mandriva_mandrake_logo, MANDRAKE_LEN, TLBL);

		else
			ERR_REPORT(_("Could not find a logo for the distro."));
	}

	return;
}