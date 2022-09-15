
from PIL import Image
import cv2
import numpy as np
import matplotlib.pyplot as plt
import requests

from io import BytesIO

def binaryImageCheck(b):
    arr = np.frombuffer(b, dtype=np.uint8)
    img = cv2.imdecode(arr, cv2.IMREAD_UNCHANGED)
    fig, ax = plt.subplots()
    ax.imshow(img)
    plt.show()

def imageFileToBinary(file):
    with open(file, 'rb') as f:
      b = f.read()
    return b

def func01():
    img = Image.open('datas/data.jpg')
    b = img.tobytes()
    return b



b = imageFileToBinary("datas/data.jpg")
print(len(b))
binaryImageCheck(b)




# url = "https://raw.githubusercontent.com/GitHub30/sketch2code-cli/master/samples/sample1.jpg"
# response = requests.get(url)
# img = Image.open(BytesIO(response.content))
# print(img)
# plt.imshow(img)

