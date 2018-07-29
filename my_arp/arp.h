/*
** Created by doom on 25/07/18.
*/

#ifndef MY_ARP_ARP_H
#define MY_ARP_ARP_H

#include <stdint.h>
#include <netinet/in.h>

enum arp_opcode
{
    ARP_REQUEST = 1,
    ARP_REPLY = 2,
};

enum arp_link_type
{
    ARP_LINK_ETHERNET = 0x0001,
};

enum arp_protocol_type
{
    ARP_PROTOCOL_IPV4 = 0x0800,
};

typedef struct arp_hdr_s
{
    uint16_t link_type;                     /** Link (hardware) layer type      (see arp_link_type above) */
    uint16_t protocol_type;                 /** Protocol type                   (see arp_protocol_type above) */
    unsigned char link_field_size;          /** Link field size                 (for example, 6 for MAC addresses */
    unsigned char protocol_field_size;      /** Protocol field size             (for example, 4 for IPv4 addresses */
    uint16_t opcode;                        /** The type of ARP message         (see arp_opcode above) */
    unsigned char payload[];                /** Actual payload of the message */
} __attribute__((packed)) arp_hdr_t;

typedef struct arp_ipv4_payload_s
{
    unsigned char src_mac[6];               /** MAC address of the source */
    uint32_t src_ip;                        /** IP address of the source */
    unsigned char dest_mac[6];              /** MAC address of the destination */
    uint32_t dest_ip;                       /** IP address of the destination */
} __attribute__((packed)) arp_ipv4_payload_t;

static inline arp_hdr_t *arp_hdr_hton(arp_hdr_t *arp_hdr)
{
    arp_hdr->link_type = htons(arp_hdr->link_type);
    arp_hdr->protocol_type = htons(arp_hdr->protocol_type);
    arp_hdr->opcode = htons(arp_hdr->opcode);
    return arp_hdr;
}

static inline arp_hdr_t *arp_hdr_ntoh(arp_hdr_t *arp_hdr)
{
    arp_hdr->link_type = ntohs(arp_hdr->link_type);
    arp_hdr->protocol_type = ntohs(arp_hdr->protocol_type);
    arp_hdr->opcode = ntohs(arp_hdr->opcode);
    return arp_hdr;
}

static inline arp_ipv4_payload_t *arp_ipv4_hton(arp_ipv4_payload_t *payload)
{
    /* IPs don't really need to be swapped, since inet_aton also outputs them in network byte order */
#if 0
    payload->src_ip = htonl(payload->src_ip);
    payload->dest_ip = htonl(payload->dest_ip);
#endif
    return payload;
}

static inline arp_ipv4_payload_t *arp_ipv4_ntoh(arp_ipv4_payload_t *payload)
{
    /* IPs don't really need to be swapped, since inet_aton also outputs them in network byte order */
#if 0
    payload->src_ip = ntohl(payload->src_ip);
    payload->dest_ip = ntohl(payload->dest_ip);
#endif
    return payload;
}

#endif //MY_ARP_ARP_H
