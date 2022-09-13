import pickle
from box import giftBox

timestamp = giftBox.get_now()
obj = giftBox(timestamp,"data.jpg","data.txt")
obj = "hoge"

b = pickle.dumps(obj)

obj = pickle.loads(b)

print(obj.timestamp)
print(giftBox.binaryImageCheck(obj.image))