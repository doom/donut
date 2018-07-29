/*
** Created by doom on 25/07/18.
*/

#ifndef MY_ARP_TUNTAP_H
#define MY_ARP_TUNTAP_H

#include <unistd.h>

#define TUN_FAILED  (-1)

typedef int tun_handle_t;

tun_handle_t tun_open(char *dev);

tun_handle_t tun_close(int fd);

ssize_t tun_read(tun_handle_t handle, char *out, size_t count);

ssize_t tun_write(tun_handle_t handle, const char *buf, size_t count);

#endif //MY_ARP_TUNTAP_H
