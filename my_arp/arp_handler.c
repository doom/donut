/*
** Created by doom on 25/07/18.
*/

#include <arpa/inet.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "arp.h"
#include "arp_handler.h"

/**
 * See the ARP RFC specification: https://tools.ietf.org/html/rfc826
 */

#define ARP_CACHE_SIZE  20

typedef struct arp_cache_data_s
{
    uint32_t src_ip;
    uint16_t link_type;
    unsigned char src_mac[6];
    bool occupied;
} arp_table_element_t;

static arp_table_element_t arp_table[ARP_CACHE_SIZE];

static void insert_in_table(arp_hdr_t *hdr, arp_ipv4_payload_t *payload)
{
    const arp_table_element_t *end = arp_table + ARP_CACHE_SIZE;

    for (arp_table_element_t *cur = arp_table; cur < end; ++cur) {
        if (cur->occupied == false) {
            cur->src_ip = payload->src_ip;
            memcpy(cur->src_mac, payload->src_mac, sizeof(cur->src_mac));
            cur->link_type = hdr->link_type;
            cur->occupied = true;
            break;
        }
    }
}

static bool update_table(arp_hdr_t *hdr, arp_ipv4_payload_t *payload)
{
    const arp_table_element_t *end = arp_table + ARP_CACHE_SIZE;

    for (arp_table_element_t *cur = arp_table; cur < end; ++cur) {
        if (cur->occupied == true && cur->link_type == hdr->link_type && cur->src_ip == payload->src_ip) {
            memcpy(cur->src_mac, payload->src_mac, sizeof(cur->src_mac));
            return true;
        }
    }
    return false;
}

static void send_reply(device_t *dev, ethernet_hdr_t *eth_hdr)
{
    arp_hdr_t *arp_hdr = (arp_hdr_t *)eth_hdr->payload;
    arp_ipv4_payload_t *arp_payload = (arp_ipv4_payload_t *)arp_hdr->payload;

    memcpy(eth_hdr->dest_mac, eth_hdr->src_mac, sizeof(eth_hdr->dest_mac));
    memcpy(eth_hdr->src_mac, dev->mac_addr, sizeof(eth_hdr->src_mac));

    arp_hdr->opcode = ARP_REPLY;

    memcpy(arp_payload->dest_mac, arp_payload->src_mac, sizeof(arp_payload->dest_mac));
    memcpy(arp_payload->src_mac, dev->mac_addr, sizeof(arp_payload->src_mac));

    arp_payload->dest_ip = arp_payload->src_ip;
    arp_payload->src_ip = dev->ip_addr;

    eth_hdr_hton(eth_hdr);
    arp_hdr_hton(arp_hdr);
    arp_ipv4_hton(arp_payload);

    device_write(dev, (const char *)eth_hdr, sizeof(ethernet_hdr_t) + sizeof(arp_hdr_t) + sizeof(arp_ipv4_payload_t));
}

void handle_arp_packet(device_t *dev, ethernet_hdr_t *eth_hdr)
{
    arp_hdr_t *arp_hdr = (arp_hdr_t *)eth_hdr->payload;
    bool did_merge;
    arp_ipv4_payload_t *arp_payload;

    arp_hdr_ntoh(arp_hdr);

    if (arp_hdr->link_type != ARP_LINK_ETHERNET) {
        printf("Unhandled link type %hx\n", arp_hdr->link_type);
        return;
    }

    if (arp_hdr->protocol_type != ARP_PROTOCOL_IPV4) {
        printf("Unhandled protocol type %hx\n", arp_hdr->protocol_type);
        return;
    }

    arp_payload = (arp_ipv4_payload_t *)arp_hdr->payload;
    arp_ipv4_ntoh(arp_payload);

    /** First, attempt to update the table entry, if one matches */
    did_merge = update_table(arp_hdr, arp_payload);

    /** If the packet was for us */
    if (dev->ip_addr == arp_payload->dest_ip) {
        if (did_merge == false) {
            insert_in_table(arp_hdr, arp_payload);
            printf("Inserting payload in cache for address %x\n", arp_payload->src_ip);
        }

        /** If the ARP message was a request, we need to reply */
        if (arp_hdr->opcode == ARP_REQUEST) {
            struct in_addr in;

            in.s_addr = arp_payload->src_ip;
            printf("Replying to host with address %x (%s)\n", arp_payload->src_ip, inet_ntoa(in));
            send_reply(dev, eth_hdr);
        }
    }
}
