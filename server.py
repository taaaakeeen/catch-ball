import socket
import pickle
import cv2
import numpy as np
import matplotlib.pyplot as plt

def binaryImageCheck(b):
    arr = np.frombuffer(b, dtype=np.uint8)
    img = cv2.imdecode(arr, cv2.IMREAD_UNCHANGED)

    # cv2.imshow("img", img)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    fig, ax = plt.subplots()
    ax.imshow(img)
    plt.show()

# 受信部
IP = '127.0.0.1'
PORT = 8765
recv_address = (IP, PORT)

#ipv4=>AF_INET
#tcp/ip通信=>SOCK_STREAM
tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpSocket.bind(recv_address)
tcpSocket.listen(1)

print('クライアントからの入力待ち')

# コネクションとアドレスを取得
connection, address = tcpSocket.accept()
print('送信元:',str(address))

while True:

    # 受信
    b = connection.recv(4096)

    obj = pickle.loads(b)
    print(obj[0])

    # 送信
    s = "OK"
    b = s.encode("UTF-8")
    send_data = b
    connection.send(send_data)