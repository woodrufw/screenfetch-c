/*  common.c
 *  Authors: William Woodruff and Aaron Caffrey
 *  -------------
 *  
 *  The detection functions that are cross-platform available are defined here.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "../misc.h"
#include "../extern.h"
#include "../prototypes.h"


/*  detect_shell
    detects the shell currently running on the computer
*/
void detect_shell(void)
{
    char *shell_name;
    uint_fast16_t x = 0;
    bool found_shell = false;

    const char *mult_shell_arr[][2] =
    {
        {"bash --version | head -1", "bash"},
        {"zsh --version", "zsh"},
        {"csh --version | head -1", "csh"},
        {"fish --version", "fish"}
    };

    const size_t shells_num = ARR_LEN(mult_shell_arr);

    if (!(shell_name = getenv("SHELL")))
    {
        if (error)
            ERR_REPORT(_("Could not detect a shell - $SHELL not defined."));
    }

    else if (STREQ(shell_name, "/bin/sh"))
        safe_strncpy(shell_str, "POSIX sh", MAX_STRLEN);

    else
    {
        for (x = 0; x < shells_num; x++)
            if (strstr(shell_name, mult_shell_arr[x][1]))
            {
                found_shell = true;
                break;
            }

        if (found_shell)
            popen_raw_shell_version(mult_shell_arr[x][0], mult_shell_arr[x][1]);
        else
            safe_strncpy(shell_str, shell_name, MAX_STRLEN);
    }

    return;
}