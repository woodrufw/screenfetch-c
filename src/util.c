/*	util.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	Utility functions used by screenfetch-c.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <regex.h>
#include <inttypes.h>
#include <malloc.h>

/* program includes */
#include "misc.h"
#include "extern.h"
#include "prototypes.h"

#if defined(__CYGWIN__)
	#include <Windows.h>
#endif

#if defined (HAVE_X11_XLIB_H) && defined (HAVE_JPEGLIB_H)
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#	include <jpeglib.h>
#endif

/*	popen_raw_shell_version
	get the raw shell version line and workaround
	the problem when the shell version has:
	GNU bash, version x.x.x(x)-release (x86_64-unknown-linux-gnu)
	Even if the shell version is x.x.xx(x) it will be handled properly
*/
void popen_raw_shell_version(const char *str1, const char *str2)
{
	FILE *shell_file;
	char vers_arr[MAX_STRLEN], temp_arr[MAX_STRLEN];
	uint_fast16_t x = 0, z = 0;

	shell_file = popen(str1, "r");
	fgets(vers_arr, MAX_STRLEN, shell_file);
	pclose(shell_file);

	size_t str_len = strlen(vers_arr);

	for (z = 0, x = 0; x < str_len; x++)
	{
		if (isdigit(vers_arr[x]) || vers_arr[x] == '.') /* ispunct() includes comma */
			temp_arr[z++] = vers_arr[x];

		if (isspace(vers_arr[x]) && z > 0)
			break;
	}

	if (STREQ(str2, "bash"))
		temp_arr[z-1] = '\0'; /* we don't want (x) in x.x.x(x) */
	else
		temp_arr[z] = '\0';

	snprintf(shell_str, MAX_STRLEN, "%s %s", str2, temp_arr);

}

/*	remove_excess_cpu_txt
	Remove unnecessary cpu text like: (tm), (TM), Processor
	AMD Athlon(tm) II X3 455 Processor -> AMD Athlon II X3 455
*/
void remove_excess_cpu_txt(void)
{
	char str1_copy[MAX_STRLEN];

	safe_strncpy(str1_copy, cpu_str, MAX_STRLEN);

	bool tm_bool = false, processor_bool = false;
	const char tokseps[] = " ()"; /* (TM), (tm) */
	char *sep_str = strtok(str1_copy, tokseps);

	/* verify that str1_copy has 'TM' or 'tm' and 'Processor' */
	while (sep_str != NULL)
	{
		if (STREQ(sep_str, "tm") || STREQ(sep_str, "TM"))
			tm_bool = true;

		if (STREQ(sep_str, "Processor"))
			processor_bool = true;

		/* retrieve the next separated string (if any) */
		sep_str = strtok(NULL, tokseps);
	}

	if (tm_bool && processor_bool)
	{
		regex_t PatterN;
		regmatch_t match[3];
		char temp_arr[MAX_STRLEN];
		int x = 0, z = 0;

		regcomp(&PatterN, "(.*)[^-](tm|TM)", REG_EXTENDED);
		regexec(&PatterN, cpu_str, 3, match, 0);

		int first_end = match[1].rm_eo;

		for (x = match[1].rm_so; x < first_end; x++)
			temp_arr[z++] = cpu_str[x];

		regcomp(&PatterN, "(.*) Processor", REG_EXTENDED);
		regexec(&PatterN, cpu_str, 3, match, 0);

		for (x = (first_end+4); x < match[1].rm_eo; x++)
			temp_arr[z++] = cpu_str[x];

		temp_arr[z] = '\0';
		snprintf(cpu_str, MAX_STRLEN, "%s", temp_arr);

		regfree(&PatterN);
	}
}

/*	safe_strncpy
	calls strncpy with the given params, then inserts a terminating NULL
	returns a pointer to a string containing the copied data
*/
char *safe_strncpy(char *destination, const char *source, size_t num)
{
	char *ret = strncpy(destination, source, num);
	ret[num - 1] = '\0';
	return ret;
}

/*	split_uptime
	splits param uptime into individual time-units
	argument long uptime: the uptime, in seconds, to be split
	arguments int *secs..*days: pointers to unsigned ints
	for storing the uptime
	--
	CAVEAT: uptime MUST be in seconds
	--
*/
void split_uptime(long uptime, uint_least32_t *secs, uint_least32_t *mins, uint_least32_t *hrs, uint_least32_t *days)
{
	*secs = (uint_least32_t) uptime % 60;
	*mins = (uint_least32_t) (uptime / 60) % 60;
	*hrs  = (uint_least32_t) (uptime / 3600) % 24;
	*days = (uint_least32_t) (uptime / 86400);

	return;
}

/*	take_screenshot
	takes a screenshot and saves it to $HOME/screenfetch_screenshot.png
*/
void take_screenshot(void)
{
	char time_str[MAX_STRLEN], shot_location[MAX_STRLEN];
	time_t t = time(NULL);
	strftime(time_str, MAX_STRLEN, "%Y-%m-%d-%H%M%S", localtime(&t));

#if defined (HAVE_X11_XLIB_H) && defined (HAVE_JPEGLIB_H)

	Display *disp = XOpenDisplay(NULL);

	if (disp)
	{
		printf("%s", "Taking shot in 3..");
		fflush(stdout);
		sleep(1);
		printf("%s", "2..");
		fflush(stdout);
		sleep(1);
		printf("%s", "1..");
		fflush(stdout);
		sleep(1);
		printf("%s\n", "0");

		uint_least32_t pixel;
		uint_fast16_t height, width, x, y;
		struct jpeg_compress_struct instance;
		struct jpeg_error_mgr jerr;

		FILE *file;
		JSAMPROW row_ptr;
		Screen *screen = ScreenOfDisplay(disp, DefaultScreen(disp));
		Window root    = RootWindow(disp, XScreenNumberOfScreen(screen));
		width  		   = WidthOfScreen(screen);
		height 		   = HeightOfScreen(screen);

		XImage *img    = XGetImage(disp, root, 0, 0, width, height, XAllPlanes(), ZPixmap);

		sprintf(shot_location, "%s/screenfetch-%s_%s.jpg", getenv("HOME"), time_str, res_str);

		if ((file = fopen(shot_location, "wb")) == NULL)
		{
			XCloseDisplay(disp);
			fprintf(stderr, "Could not open \"%s\" for writing\n", shot_location);
			exit(1);
		}

		char *tmp = malloc(sizeof(char)*3*width*height);

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pixel = XGetPixel(img,x,y);
				tmp[y*width*3+x*3+0] = (pixel>>16);
				tmp[y*width*3+x*3+1] = ((pixel&0x00ff00)>>8);
				tmp[y*width*3+x*3+2] = (pixel&0x0000ff);
			}
		}

		instance.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&instance);
		jpeg_stdio_dest(&instance, file);

		instance.image_width	  = width;
		instance.image_height	  = height;
		instance.input_components = 3;
		instance.in_color_space   = JCS_RGB;

		jpeg_set_defaults(&instance);
		jpeg_set_quality(&instance, 100, TRUE);
		jpeg_start_compress(&instance, TRUE);

		while (instance.next_scanline < instance.image_height)
		{
			row_ptr = (JSAMPROW)\
			&tmp[instance.next_scanline*(img->depth>>3)*width];
			jpeg_write_scanlines(&instance, &row_ptr, 1);
		}

		free(tmp);
		jpeg_finish_compress(&instance);
		fclose(file);

		jpeg_destroy_compress(&instance);
		XDestroyImage(img);
		XCloseDisplay(disp);

		if (FILE_EXISTS(shot_location) && verbose)
			VERBOSE_OUT(_("Screenshot successfully saved."), "");

		else if (!FILE_EXISTS(shot_location) && error)
			ERR_REPORT(_("Problem saving screenshot."));

	}
	else
	{
		ERR_REPORT(_("No X server, aborting the creation of screenshot."));
		return;
	}

#else

#if !defined(__CYGWIN__)
	int call_status = 1;
	char shot_str[MAX_STRLEN+MAX_STRLEN], file_loc[MAX_STRLEN+MAX_STRLEN];
	sprintf(shot_location, "/screenfetch-%s_%s.png", time_str, res_str);
#endif

	printf("%s", "Taking shot in 3..");
	fflush(stdout);
	sleep(1);
	printf("%s", "2..");
	fflush(stdout);
	sleep(1);
	printf("%s", "1..");
	fflush(stdout);
	sleep(1);
	printf("%s\n", "0");

#if defined(__CYGWIN__)
	/* terrible hack, the printscreen key is simulated */
	keybd_event(VK_SNAPSHOT, 0, 0, 0);

	if (verbose)
		VERBOSE_OUT(_("Screenshot has been saved to the clipboard."), "");

	/* NOT FINSISHED - HBITMAP needs to be saved
	HDC screen_dc = GetDC(NULL);
	HDC mem_dc = CreateCompatibleDC(screen_dc);

	int horiz = GetDeviceCaps(screen_dc, HORZRES);
	int vert = GetDeviceCaps(screen_dc, VERTRES);

	HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, horiz, vert);
	HBITMAP old_bitmap = SelectObject(mem_dc, bitmap);

	BitBlt(mem_dc, 0, 0, horiz, vert, screen_dc, 0, 0, SRCCOPY);
	bitmap = SelectObject(mem_dc, old_bitmap);

	DeleteDC(screen_dc);
	DeleteDC(mem_dc);
	*/
#elif defined(__APPLE__) && defined(__MACH__)
		sprintf(shot_str, "screencapture -x ~%s 2> /dev/null", shot_location);
#else
		sprintf(shot_str, "scrot ~%s 2> /dev/null", shot_location);
#endif

#if !defined(__CYGWIN__)
	call_status = system(shot_str);
	safe_strncpy(file_loc, getenv("HOME"), MAX_STRLEN);
	strncat(file_loc, shot_location, MAX_STRLEN);

	if (FILE_EXISTS(file_loc) && verbose)
		VERBOSE_OUT(_("Screenshot successfully saved."), "");

	else if (call_status && error)
		ERR_REPORT(_("Problem saving screenshot."));

#endif
#endif /* xlib_h, jpeg_h */

	return;
}