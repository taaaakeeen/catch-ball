import socket
import time
import datetime

def imageFileToBinary(file):
    with open(file, 'rb') as f:
      b = f.read()
    return b

IP = '127.0.0.1'
PORT = 8765
destination = (IP, PORT)

#ipv4=>AF_INET
#tcp/ip通信=>SOCK_STREAM
tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpSocket.connect(destination)

while True:

    s = "2"
    b = s.encode("UTF-8")

    b = imageFileToBinary("datas/data.jpg")

    # 送信
    send_data = b
    tcpSocket.send(send_data)

    # 受信
    recv_data = tcpSocket.recv(4096).decode()
    print(recv_data)

    time.sleep(3)


# line = ''
# while line != 'bye':

#     # 標準入力からデータを取得
#     print('偶数の数値を入力して下さい')
#     line = input('>>>')
    
#     # サーバに送信
#     data = line.encode("UTF-8")
#     socket1.send(data)
    
#     # サーバから受信
#     data1 = socket1.recv(4096).decode()
    
#     # サーバから受信したデータを出力
#     print('サーバーからの回答: ' + str(data1))

# socket1.close()
# print('クライアント側終了です')