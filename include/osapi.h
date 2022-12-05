#pragma once

#include <string.h>
#include "os_type.h"

void ets_bzero(void *s, size_t n);
void ets_delay_us(uint16_t us);
void ets_install_putc1(void (*p)(char c));

#define os_bzero ets_bzero
#define os_delay_us ets_delay_us
#define os_install_putc1 ets_install_putc1

int ets_memcmp(const void *str1, const void *str2, unsigned int nbyte);
void *ets_memcpy(void *dest, const void *src, unsigned int nbyte);
void *ets_memmove(void *dest, const void *src, unsigned int nbyte);
void *ets_memset(void *dest, int val, unsigned int nbyte);

int ets_strcmp(const char *s1, const char *s2);
char *ets_strcpy(char *s1, const char *s2);
int ets_strlen(const char *s);
int ets_strncmp(const char *s1, const char *s2, unsigned int n);
char *ets_strncpy(char *s1, const char *s2, unsigned int n);
char *ets_strstr(const char *s1, const char *s2);

#define os_memcmp ets_memcmp
#define os_memcpy ets_memcpy
#define os_memmove ets_memmove
#define os_memset ets_memset
#define os_strcat strcat
#define os_strchr strchr
#define os_strcmp ets_strcmp
#define os_strcpy ets_strcpy
#define os_strlen ets_strlen
#define os_strncmp ets_strncmp
#define os_strncpy ets_strncpy
#define os_strstr ets_strstr

int ets_sprintf(char *str, const char *format, ...)  __attribute__ ((format (printf, 2, 3)));
int os_printf_plus(const char *format, ...)  __attribute__ ((format (printf, 1, 2)));

#define os_sprintf  ets_sprintf
#define os_printf	os_printf_plus

unsigned long os_random(void);
int os_get_random(unsigned char *buf, size_t len);
