#! /usr/bin/env python3
# coding: utf-8
# tcp_server

import socket
import threading
import time
import csv
import requests
import json

bind_ip = "0.0.0.0"
bind_port = 10250
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)

print("[*] Listening on %s:%d" % (bind_ip, bind_port))

def post_data(data_fields):
    try:
        data = {
            "mode": int(data_fields[0]),
            "date_time": data_fields[1],
            "illuminance": int(data_fields[2]),
            "is_people_detected": bool(int(data_fields[3]))
        }
        url = f"{"""ngrokによって生成されるURL"""}/raw-data"
        response = requests.post(url, json=data)
        return response.status_code == 200
    except ValueError as e:
        print(f"[*] Error converting data types: {e}")
        return response.status_code == 400
    except requests.exceptions.RequestException as e:
        print(f"[*] Error making request: {e}")
        return response.status_code == 400

def handle_client(client_socket):
    bufsize = 1024
    default_time = time.time()
    timeout = 10
    
    try:
        while True:
            request = client_socket.recv(bufsize)
            
            # If no data received, client disconnected
            if not request:
                if time.time() - default_time > timeout:
                    print("[*] Connection timeout")
                else:
                    print("[*] Client disconnected")
                break
            
            # Decode and parse the received data
            try:
                data_str = request.decode("utf-8").strip()
                print(f"[*] Received: {data_str}")
                
                # Check if this is an HTTP request (from browser)
                if data_str.startswith(("GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH")):
                    print("[*] HTTP request detected - rejecting")
                    client_socket.send(b"ERR\r\n")
                    print("[*] Sent: ERR")
                    break
                
                # Split CSV data: "id,timestamp,illuminance,md_status"
                data_fields = data_str.split(',')
                
                # Write to CSV file
                with open("arduino_data.csv", "a", newline='') as f:
                    csv.writer(f).writerow(data_fields)
                
                # Send acknowledgment
                client_socket.send(b"OK\r\n")
                print("[*] Sent: OK")
                try:
                    post_data(data_fields)
                    print("[*] Data posted successfully")
                except Exception as e:
                    print(f"[*] Error posting data: {e}")
                
            except UnicodeDecodeError as e:
                print(f"[*] Decode error: {e}")
                client_socket.send(b"ERR\r\n")
            except Exception as e:
                print(f"[*] Error processing data: {e}")
                client_socket.send(b"ERR\r\n")
                
    except BrokenPipeError:
        print("[*] Client disconnected (broken pipe)")
    except Exception as e:
        print(f"[*] Connection error: {e}")
    finally:
        try:
            client_socket.close()
        except:
            pass

while True:
    client, addr = server.accept()
    print("[*] Accepted connection from %s:%d" % (addr[0], addr[1]))
    client_handler = threading.Thread(target=handle_client, args=(client,))
    client_handler.start()