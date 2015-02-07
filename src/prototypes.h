/*  prototypes.h
 *  Authors: William Woodruff and Aaron Caffrey
 *
 *  Function prototypes for all screenfetch-c modules.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#ifndef SCREENFETCH_C_PROTOTYPES_H
#define SCREENFETCH_C_PROTOTYPES_H


/* util.c module */
void popen_raw_shell_version(const char *str1, const char *str2);
void remove_excess_cpu_txt(char *str1);
char *safe_strncpy(char *destination, const char *source, size_t num);
void split_uptime(long uptime, uint_least32_t *secs, uint_least32_t *mins, uint_least32_t *hrs, uint_least32_t *days);
void take_screenshot(bool verbose, char *monitor_res);


/* parser.c module */
void Init_parseR(int argc, char **argv);


/* disp.c module */
void process_data(const char *logo[], uint_fast16_t num, char *colour);
void main_text_output(bool verbose);
void main_ascii_output(char *passed_distro);


/* detect.c module */
void detect_distro(char *str1);
void detect_host(void);
void detect_kernel(char *str);
void detect_uptime(char *str);
void detect_pkgs(char *str, const char *distro_str);
void detect_cpu(char *str);
void detect_gpu(char *str);
void detect_disk(char *str);
void detect_mem(char *str);
void detect_shell(char *str);
void detect_res(char *str);
void detect_de(char *str);
void detect_wm(char *str);
void detect_wm_theme(char *str, const char *wm_str);
void detect_gtk(char *str1, char *str2, char *str3);


#endif /* SCREENFETCH_C_PROTOTYPES_H */