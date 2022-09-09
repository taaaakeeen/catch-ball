import datetime


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





