/*  parser.c
 *  Author: Aaron Caffrey
 *  -------------
 *  
 *  The primary argument(s) parser in screenfetch-c.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "misc.h"
#include "extern.h"
#include "prototypes.h"


const char *argp_program_version = "screenfetch-c "PROG_VERSION; /* -D macro */
const char *argp_program_bug_address = "https://github.com/woodruffw/screenfetch-c ";
static const char doc[] = N_("Display computer information and an ASCII logo");

/* "GRID" 0 means zero lines below doc[],
you can create as many grids as you prefer */
static const struct argp_option options[] =
{
#define GRID 0
  {NULL, 0, NULL, 0,
   N_("Main operation mode:"), GRID },
  {"verbose", 'v', 0, 0,
   N_("Enable verbosity during output."), GRID+1 },
  {"screenshot", 's', 0, 0,
   N_("Take a screenshot."), GRID+1 },
  {"no-logo", 'n', 0, 0,
   N_("Print output without a logo."), GRID+1 },
  {"suppress-errors", 'E', 0, 0,
   N_("Suppress error output."), GRID+1 },
  {"portrait", 'p', 0, 0,
   N_("Print output in portrait mode."), GRID+1 },
  {"logo-only", 'L', "DISTRO", 0,
   N_("Output only DISTRO's logo."), GRID+1 },
  {"distro", 'D', "DISTRO", 0,
   N_("Print output with DISTRO's logo."), GRID+1 },
#undef GRID
{ NULL, }
};

/*static const struct argp_option argp_default_options[] =
{
  {"help", '?', 0, 0, N_("Give this help list"), -1},
  {"usage", 'u', 0, 0, N_("Give a short usage message"), 0},
  {"version", 'V', 0, 0, N_("Print program version"), 0},
  {NULL, 0, 0, 0, NULL, 0}
};*/

/* The parsing state structure is pretty much overkiller for this program */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    switch(key)
    {
      case 'v': verbose = true;
                break;
      case 'n': main_logo = false;
                break;
      case 's': screenshot = true;
                break;
      case 'D': SET_GIVEN_DISTRO(arg);
                break;
      case 'E': error = false;
                break;
      case 'p': portrait = true;
                break;
      case 'L': logo_only_requested = true;
                main_ascii_output(arg);
                break;
      default:
              return ARGP_ERR_UNKNOWN;
    }
    return EXIT_SUCCESS;
}

void Init_parseR(int argc, char **argv)
{
    static const struct argp arg_parser = { .parser = parse_opt, .options = options, .doc = doc };

    argp_parse(&arg_parser, argc, argv, ARGP_IN_ORDER, NULL, NULL);
}