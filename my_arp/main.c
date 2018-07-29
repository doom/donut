/*
** Created by doom on 25/07/18.
*/

#include <stdio.h>
#include "ethernet.h"
#include "device.h"
#include "arp_handler.h"

int main(void)
{
    device_t dev;

    if (device_init(&dev, "42.42.42.42", "42:42:42:42:42:42") == 0) {
        printf("Successfully initialized device '%s'\n", dev.name);
        while (1) {
            char buf[4096];
            ssize_t ret = device_read(&dev, buf, sizeof(buf));
            ethernet_hdr_t *hdr;

            if (ret < 0)
                break;

            /** Shamefully assuming device_read() read all the available data */
            hdr = ethernet_get_header(buf);
            eth_hdr_ntoh(hdr);
            switch (hdr->type) {
                case ETH_P_ARP:
                    puts("Got an ARP payload");
                    handle_arp_packet(&dev, hdr);
                    break;
                default:
                    printf("Unhandled ethernet payload type %hx\n", hdr->type);
                    break;
            }
        }
        device_wipe(&dev);
    } else {
        puts("Unable to initialize the device. Are you running with enough privileges ?");
        return 1;
    }
    return 0;
}
