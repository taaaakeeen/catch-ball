import socket
import time
import pickle
from box import giftBox
from datetime import datetime

def get_now():
        now = datetime.now()
        now = now.strftime('%Y-%m-%d %H:%M:%S')
        return now

def imageFileToBinary(file):
    with open(file, 'rb') as f:
      b = f.read()
    return b

def textFileToStr(file):
    with open(file, "r") as f:
      s = f.read()
    return s

IP = '127.0.0.1'
PORT = 8765
destination = (IP, PORT)

#ipv4=>AF_INET
#tcp/ip通信=>SOCK_STREAM
tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpSocket.connect(destination)

while True:

    timestamp = get_now()
    image = imageFileToBinary("data.jpg")
    label = textFileToStr("data.txt")

    # 送信
    obj = [timestamp,image,label]
    b = pickle.dumps(obj)

    tcpSocket.send(b)

    # 受信
    recv_data = tcpSocket.recv(4096)
    print(recv_data.decode())

    time.sleep(3)