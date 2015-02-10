/*  parser.c
 *  Authors: William Woodruff and Aaron Caffrey
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
#include <inttypes.h>

#include "misc.h"
#include "extern.h"
#include "prototypes.h"


const char *argp_program_version = "screenfetch-c "PROG_VERSION;
const char *argp_program_bug_address = "https://github.com/woodruffw/screenfetch-c ";
static const char doc[] = N_("Display computer information and an ASCII logo");

static const struct argp_option options[] =
{
  { .doc = N_("Main operation mode:") },
  { .name = "verbose",         .key = 'v', .doc = N_("Enable verbosity during output.") },
  { .name = "screenshot",      .key = 's', .doc = N_("Take a screenshot.") },
  { .name = "suppress-errors", .key = 'E', .doc = N_("Suppress error output.") },
  { .name = "portrait",        .key = 'p', .doc = N_("Print output in portrait mode.") },
  { .name = "logo-only",       .key = 'L', .arg = "DISTRO", .doc = N_("Output only DISTRO's logo.") },
  { .name = "distro",          .key = 'D', .arg = "DISTRO", .doc = N_("Print output with DISTRO's logo.") },
  { .name = "no-logo",         .key = 'n', .doc = N_("Print output without a logo.") },
  { .doc = NULL }

};

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
      case 'L': break_requested = true;
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