import socket

s = socket.socket()
s.connect(("localhost", 379))

data = s.recv(1024)

while len(data) != 0:
    decoded = data.decode()
    print(decoded)

    if decoded.__contains__("username"):
        msg = "\"jackward\""
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    if decoded.__contains__("password"):
        msg = "\"mypassword\""
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())
    
    if decoded.__contains__("[>]"):
        msg = "1"
        print(f">>> Sending {msg}\n")
        s.send(msg.encode())

    try:
        data = s.recv(1024)
    except:
        data = 0

s.close()

