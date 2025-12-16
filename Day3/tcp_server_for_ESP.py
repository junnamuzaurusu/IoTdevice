#! /usr/bin/env python3
# coding: utf-8
# tcp_server

import socket
import threading
import time

bind_ip = "0.0.0.0"
bind_port = 10250
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)
print("[*] Listening on %s:%d" % (bind_ip, bind_port))

def handle_client(client_socket):
    bufsize = 1024
    default_time = time.time()
    timeout = 10
    while True:
        request = client_socket.recv(bufsize)
        print("[*] Received: %s" % request)
        if not request and time.time() - default_time > timeout:
            print("[*] Connection failed")
            client_socket.close()
            break

            

while True:
    client, addr = server.accept()
    print("[*] Accepted connection from %s:%d" % (addr[0], addr[1]))
    client_handler = threading.Thread(target=handle_client, args=(client,))
    client_handler.start()