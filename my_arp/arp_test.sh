#!/bin/sh

sudo ifconfig tap0 up
arping -I tap0 42.42.42.42 -c 10
