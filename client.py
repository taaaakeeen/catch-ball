import socket
import time
import pickle
from datetime import datetime
import cv2
import numpy as np
import matplotlib.pyplot as plt

def binaryImageCheck(b):
    arr = np.frombuffer(b, dtype=np.uint8)
    img = cv2.imdecode(arr, cv2.IMREAD_UNCHANGED)
    fig, ax = plt.subplots()
    ax.imshow(img)
    plt.show()

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

#ipv4=>AF_INET
#tcp/ip通信=>SOCK_STREAM
tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpSocket.connect((IP,PORT))

while True:

  timestamp = get_now()
  image = imageFileToBinary("data.jpg")
  label = textFileToStr("data.txt")

  # データ
  obj = [timestamp,image,label]
  b = pickle.dumps(obj)

  # 送信
  tcpSocket.send(b)
  print("------------------------------")
  print(len(b),"byte","サーバに送信")

  obj = pickle.loads(b)

  timestamp = obj[0]
  print(timestamp)

  image = obj[1]
  # binaryImageCheck(image)

  label = obj[2]
  # print(label)

  # 返信
  res_msg = tcpSocket.recv(4096)
  print("受信完了メッセージをサーバから受信=>",res_msg.decode())

  time.sleep(3)
