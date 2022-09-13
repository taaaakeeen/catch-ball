import socket
import pickle
from box import giftBox

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
    recv_data = connection.recv(4096)

    obj = pickle.loads(recv_data)
    print(obj.timestamp)

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