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

#ipv4=>AF_INET
#tcp/ip通信=>SOCK_STREAM
tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("ソケット作成完了")

tcpSocket.bind((IP,PORT))
print("バインド完了")

tcpSocket.listen(1)
print('クライアントからの入力待ち')

# コネクションとアドレスを取得
connection, address = tcpSocket.accept()
print('送信元:',str(address))

data = b""
c = 0
while True:

    c = c + 1

    # 受信
    b = connection.recv(4096)
    print(c,len(b))

    # 合体
    data += b

    if len(b) < 4096:

        print("------------------------------")
        print(len(data),"byte","クライアントから受信")

        obj = pickle.loads(data)

        timestamp = obj[0]
        print(timestamp)

        image = obj[1]
        print("画像を確認してください")
        binaryImageCheck(image)
        

        label = obj[2]
        print(label)

        # リセット
        data = b""
        c = 0

        # 返信
        res_msg = "OK"
        print("受信完了メッセージをクライアントに送信=>",res_msg)
        connection.send(res_msg.encode("UTF-8"))
        print("------------------------------")
