import datetime
import cv2
import numpy as np
import matplotlib.pyplot as plt

def imageFileToBinary(file):
    with open(file, 'rb') as f:
      b = f.read()
    return b

def binaryImageCheck(b):
    arr = np.frombuffer(b, dtype=np.uint8)
    img = cv2.imdecode(arr, cv2.IMREAD_UNCHANGED)

    # cv2.imshow("title", img)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    fig, ax = plt.subplots()
    ax.imshow(img)
    plt.show()

b = imageFileToBinary("datas/data.jpg")
# print(b.hex())
binaryImageCheck(b)


def getNow():
    now = datetime.datetime.now()
    year = now.year
    month = now.month
    day = now.day
    hour = now.hour
    minute = now.minute
    second = now.second
    microsecond = now.microsecond
    now = now.strftime('%Y-%m-%d %H:%M:%S.%f')
    return now

def func01():
    b_img = ""
    img_size = ""

    b_txt = ""
    txt_size = ""

    b_datetime = ""
    datetime_size = ""





