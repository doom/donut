/*
** Created by doom on 25/07/18.
*/

#ifndef MY_ARP_ARP_HANDLER_H
#define MY_ARP_ARP_HANDLER_H

#include "device.h"
#include "ethernet.h"

void handle_arp_packet(device_t *dev, ethernet_hdr_t *eth_hdr);

#endif //MY_ARP_ARP_HANDLER_H
