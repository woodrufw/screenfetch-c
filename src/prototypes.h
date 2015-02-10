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
void remove_excess_cpu_txt(void);
char *safe_strncpy(char *destination, const char *source, size_t num);
void split_uptime(long uptime, uint_least32_t *secs, uint_least32_t *mins, uint_least32_t *hrs, uint_least32_t *days);
void take_screenshot(void);


/* parser.c module */
void Init_parseR(int argc, char **argv);


/* disp.c module */
void process_data(const char *logo[], uint_fast16_t num, char *colour);
void main_text_output(bool verbose);
void main_ascii_output(char *passed_distro);


/* plat/'os'/detect.c module */
void detect_distro(void);
void detect_host(void);
void detect_kernel(void);
void detect_uptime(void);
void detect_pkgs(void);
void detect_cpu(void);
void detect_gpu(void);
void detect_disk(void);
void detect_mem(void);
void detect_res(void);
void detect_de(void);
void detect_wm(void);
void detect_wm_theme(void);
void detect_gtk(void);


/* plat/common.c module */
void detect_shell(void);


#endif /* SCREENFETCH_C_PROTOTYPES_H */