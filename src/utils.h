#ifndef UTILS_H
#define UTILS_H


#include <stddef.h>
#include <stdint.h>

void arfillzeros(char *arr, size_t len);
void print_array(unsigned char *arr, size_t len);
long buf_to_long(unsigned char *buf);
uint64_t buf_to_long_long(unsigned char *buf);
void string_toupper(char *str);
void print_hex(unsigned char *buf, size_t n);
int compare_arrays(unsigned char *a1, unsigned char *a2, size_t len);
size_t remove_file_from_path(char *path);

int lsb_from_long(long long l);
int msb_from_long(long long l);
void itoa_hex(int i, char *buf);
void lltoa_hex(long long i, char *buf);
void print_argv(int argc, char **argv);

int get_random_percentage();

void free_triple_char_pointer(char ***config);
char ***config_from_str(char *str);
char * my_strchrnul(char *str, int c);
void print_config(char ***config);
void eliminate_range_from_string(char *str, size_t offset, size_t len_section, size_t len_str);
uint32_t my_atoi_base(char *str, int base);
int bool_str(char *str);


#endif // UTILS_H
