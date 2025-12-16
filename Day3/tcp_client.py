#! /usr/bin/env python3
# coding: utf-8
# tcp_client

import socket

target_url = "0.0.0.0"
target_port = 10250
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((target_url, target_port))

client.send("Hello Server!\n")
response = client.recv(4096)

print("[*] Response: %s" % response)