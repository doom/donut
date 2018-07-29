/*
** Created by doom on 25/07/18.
*/

#ifndef MY_ARP_DEVICE_H
#define MY_ARP_DEVICE_H

#include <net/if.h>
#include <stdint.h>
#include "tuntap.h"

typedef struct device_s
{
    tun_handle_t handle;            /** The handle to the TUN device */
    uint32_t ip_addr;               /** The device IP address, in network byte order */
    char name[IFNAMSIZ];            /** The device name, for example "tap0" */
    unsigned char mac_addr[6];      /** The device MAC address */
} device_t;

int device_init(device_t *dev, const char *ip_addr, const char *mac_addr);

ssize_t device_read(device_t *dev, char *out, size_t count);

ssize_t device_write(device_t *dev, const char *out, size_t count);

void device_wipe(device_t *dev);

#endif //MY_ARP_DEVICE_H
