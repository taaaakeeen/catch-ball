import socket
HOST_NAME = "127.0.0.1"
PORT = 8080
#ipv4を使うので、AF_INET
#tcp/ip通信を使いたいので、SOCK_STREAM
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#localhostとlocal portを指定
sock.bind((HOST_NAME,PORT))
#server動作開始
sock.listen(1)
#接続を許可して、待つ
client,remote_addr = sock.accept()
print("accepted remote. remote_addr {}.".format(remote_addr))
while True:
    #接続されたら、データが送られてくるまで待つ
    rcv_data = client.recv(1024)
    #接続が切られたら、終了
    if not rcv_data:
        print("receive data don't exist")
        break
    else:
        print("receive data : {} ".format(rcv_data.decode("utf-8")))
        #clientにOKを送信
        client.sendall('OK\n'.encode())
print("close client communication")
#clientとserverのsocketを閉じる
client.close()
sock.close()