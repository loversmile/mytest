#!/usr/bin/env python
# coding=utf-8

import socket

def get_ip_address():
# This is a simple hack to find our IP address
# AFAIK this is the only platform-independent way to obtain the address

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(('snom.com', 0))
    return s.getsockname()[0]

a = get_ip_address()
print a
