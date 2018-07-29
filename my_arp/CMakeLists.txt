cmake_minimum_required(VERSION 3.8)
project(my_arp_proj)

set(CMAKE_C_FLAGS "-Wall -Wextra")

set(my_arp_SRC
        tuntap.h
        tuntap.c
        device.h
        device.c
        ethernet.h
        arp.h
        arp_handler.h
        arp_handler.c
        main.c
        )

add_executable(my_arp ${my_arp_SRC})
