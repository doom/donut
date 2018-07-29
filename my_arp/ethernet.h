/*
** Created by doom on 25/07/18.
*/

#ifndef MY_ARP_ETHERNET_H
#define MY_ARP_ETHERNET_H

#include <linux/if_ether.h>
#include <netinet/in.h>
#include <stdint.h>

typedef struct ethernet_hdr_s
{
    unsigned char dest_mac[ETH_ALEN];       /** Destination MAC address */
    unsigned char src_mac[ETH_ALEN];        /** Destination MAC address */
    uint16_t type;                          /** Payload type if >= 1536, payload length otherwise */
    unsigned char payload[];                /** Payload data */
} __attribute__((packed)) ethernet_hdr_t;

static inline ethernet_hdr_t *ethernet_get_header(char *data)
{
    return (ethernet_hdr_t *)data;
}

static inline ethernet_hdr_t *eth_hdr_ntoh(ethernet_hdr_t *eth_hdr)
{
    eth_hdr->type = ntohs(eth_hdr->type);
    return eth_hdr;
}

static inline ethernet_hdr_t *eth_hdr_hton(ethernet_hdr_t *eth_hdr)
{
    eth_hdr->type = htons(eth_hdr->type);
    return eth_hdr;
}

#endif //MY_ARP_ETHERNET_H
