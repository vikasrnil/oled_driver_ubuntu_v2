#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stddef.h>

/* System Info */
void get_hostname(char *hostname, size_t size);
void get_ip_and_interface(char *ip, char *iface);

/* OLED Display Helper */
void display_system_info(void);

#endif

