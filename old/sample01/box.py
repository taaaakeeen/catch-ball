from datetime import datetime
import cv2
import numpy as np
import matplotlib.pyplot as plt

class giftBox():
    def __init__(self,timestamp,image,label):
        self.timestamp = timestamp
        self.image = imageFileToBinary(image)
        self.label = textFileToStr(label)

    def get_now():
        now = datetime.now()
        now = now.strftime('%Y-%m-%d %H:%M:%S')
        return now

    def binaryImageCheck(b):
        arr = np.frombuffer(b, dtype=np.uint8)
        img = cv2.imdecode(arr, cv2.IMREAD_UNCHANGED)

        # cv2.imshow("img", img)
        # cv2.waitKey(0)
        # cv2.destroyAllWindows()

        fig, ax = plt.subplots()
        ax.imshow(img)
        plt.show()

    

def imageFileToBinary(file):
    with open(file, 'rb') as f:
      b = f.read()
    return b

def textFileToStr(file):
    with open(file, "r") as f:
      s = f.read()
    return s

