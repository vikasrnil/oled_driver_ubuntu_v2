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

    get_hostname(hostname, sizeof(hostname));
    get_ip_and_interface(ip, iface);

    ssd1306_oled_clear_screen();

    /* Hostname */
//    ssd1306_oled_set_XY(0, 0);
//    ssd1306_oled_puts(SSD1306_FONT_NORMAL, "Hostname:");
//    ssd1306_oled_set_XY(0, 1);
    ssd1306_oled_puts_center(2, hostname);

    /* IP */
   // ssd1306_oled_set_XY(0, 4);
   // ssd1306_oled_puts(SSD1306_FONT_NORMAL, "IP:");
   // ssd1306_oled_set_XY(0, 5);
    ssd1306_oled_draw_pixel_hline(28, 10, 118);
    ssd1306_oled_puts_center(4, ip);
    ssd1306_oled_draw_pixel_hline(42, 10, 118); 
    /* Interface */
   // ssd1306_oled_set_XY(0, 6);
    ssd1306_oled_draw_pixel_hline(44, 10, 118);
    ssd1306_oled_puts_center(6, iface);

}

