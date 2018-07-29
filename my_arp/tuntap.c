/*
** Created by doom on 25/07/18.
*/

#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "tuntap.h"

tun_handle_t tun_open(char *dev)
{
    struct ifreq ifr;
    int fd = open("/dev/net/tun", O_RDWR);

    if (fd < 0)
        return -1;
    memset(&ifr, 0, sizeof(ifr));

    /**
     * IFF_TAP:     we want a TAP device, since they handle network layer 2
     * IFF_NO_PI:   we don't want TUN/TAP to add extra Protocol Information to the data we read
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    if (ioctl(fd, TUNSETIFF, &ifr) < 0) {
        close(fd);
        return -1;
    }
    strcpy(dev, ifr.ifr_name);
    return fd;
}

int tun_close(tun_handle_t fd)
{
    return close(fd);
}

ssize_t tun_read(tun_handle_t fd, char *buf, size_t size)
{
    return read(fd, buf, size);
}

ssize_t tun_write(tun_handle_t fd, const char *buf, size_t size)
{
    return write(fd, buf, size);
}
