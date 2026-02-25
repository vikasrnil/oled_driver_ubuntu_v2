#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "ssd1306.h"
#include "functions.h"

/* ---------------- HOSTNAME ---------------- */
void get_hostname(char *hostname, size_t size)
{
    gethostname(hostname, size);
}

/* ---------------- IP + INTERFACE ---------------- */
void get_ip_and_interface(char *ip, char *iface)
{
    struct ifaddrs *ifaddr, *ifa;

    strcpy(ip, "No IP");
    strcpy(iface, "No IF");

    if (getifaddrs(&ifaddr) == -1)
        return;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *sa =
                (struct sockaddr_in *)ifa->ifa_addr;

            char *addr = inet_ntoa(sa->sin_addr);

            /* Skip loopback */
            if (strcmp(addr, "127.0.0.1") != 0)
            {
                strcpy(ip, addr);
                strcpy(iface, ifa->ifa_name);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
}


/* ---------------- DISPLAY FUNCTION ---------------- */

void display_system_info(void)
{
    char hostname[64];
    char ip[64];
    char iface[32];
    char buffer[128];

    get_hostname(hostname, sizeof(hostname));
    get_ip_and_interface(ip, iface);

    ssd1306_oled_clear_screen();

    /* Hostname */
    snprintf(buffer, sizeof(buffer), "Hostname: %s", hostname);
    ssd1306_oled_puts_center(2, buffer);
    memset(buffer,0,sizeof(buffer));

    /* IP */
    ssd1306_oled_draw_pixel_hline(28, 10, 118);
    snprintf(buffer, sizeof(buffer), "IP: %s", ip);
    ssd1306_oled_puts_center(4, buffer);
    ssd1306_oled_draw_pixel_hline(42, 10, 118);
    memset(buffer,0,sizeof(buffer));

    /* Interface */
    snprintf(buffer, sizeof(buffer), "Interface: %s", iface);
    ssd1306_oled_puts_center(6, buffer);
    memset(buffer,0,sizeof(buffer));
}

