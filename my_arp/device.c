/*
** Created by doom on 25/07/18.
*/

#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "device.h"

int device_init(device_t *dev, const char *ip_addr, const char *mac_addr)
{
    memset(dev, 0, sizeof(device_t));
    dev->handle = tun_open(dev->name);
    if (dev->handle == TUN_FAILED)
        return -1;
    if (inet_pton(AF_INET, ip_addr, &dev->ip_addr) != 1
        || sscanf(mac_addr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                  &dev->mac_addr[0], &dev->mac_addr[1],
                  &dev->mac_addr[2], &dev->mac_addr[3],
                  &dev->mac_addr[4], &dev->mac_addr[5]) != 6) {
        tun_close(dev->handle);
        return -1;
    }
    return 0;
}

ssize_t device_read(device_t *dev, char *out, size_t count)
{
    return tun_read(dev->handle, out, count);
}

ssize_t device_write(device_t *dev, const char *out, size_t count)
{
    return tun_write(dev->handle, out, count);
}

void device_wipe(device_t *dev)
{
    tun_close(dev->handle);
}
