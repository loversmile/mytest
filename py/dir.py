import os
import sys
PATH="/home/jklou/mylou/"

cur_list = os.listdir("/home/jklou/mylou")
for item in cur_list:
    item_tmp = PATH+item
    if os.path.isdir(item_tmp):
        print item
