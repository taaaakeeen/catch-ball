import socket
import pickle

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostname(), 1235))

full_msg = b''
while True:
    msg = s.recv(1024)
    if len(msg) <= 0:
        break
    full_msg += msg
d = pickle.loads(full_msg)
print(d)