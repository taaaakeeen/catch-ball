import socket
import cv2
import numpy as np

def binaryImageCheck(b):
    arr = np.frombuffer(b, dtype=np.uint8)
    img = cv2.imdecode(arr, cv2.IMREAD_UNCHANGED)
    cv2.imshow("img", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

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

    binaryImageCheck(b)

    recv_data = b.decode()
    print(recv_data)

    # 送信
    s = "OK"
    b = s.encode("UTF-8")
    send_data = b
    connection.send(send_data)





# # 無限ループ　byeの入力でループを抜ける
# recvline = ''
# sendline = ''
# num = 0
# while True:

#     # クライアントからデータを受信
#     recvline = connection.recv(4096).decode()

#     if recvline == 'bye':
#         break

#     try:
#         num = int(recvline)

#         if num % 2 == 0:
#             sendline = 'OKです'.encode('utf-8')
#         else:
#             sendline = 'NGです'.encode('utf-8')
#         connection.send(sendline)

#     except ValueError as e:
#         # 送信用の文字を送信
#         sendline = '数値を入力して下さい'.encode('utf-8')
#         connection.send(sendline)
#     finally:
#         print('クライアントで入力された文字＝' + str(recvline))
        
# # クローズ
# connection.close()
# socket1.close()
# print('サーバー側終了です')