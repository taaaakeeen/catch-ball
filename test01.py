import pickle
from datetime import datetime




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

class Data():
    def __init__(self,timestamp,image,label):
        self.timestamp = timestamp
        self.image = image
        self.label = label


timestamp = get_now()
image = imageFileToBinary("data.jpg")
label = textFileToStr("data.txt")

obj = Data(timestamp,image,label)

data = pickle.dumps(obj)

obj = pickle.loads(data)




# with open("test.pickle", mode="wb") as f:
#     pickle.dump(obj, f)

# with open("test.pickle", mode="rb") as f:
#     obj = pickle.load(f)

print(obj.timestamp)
binaryImageCheck(obj.image)
print(obj.label)
