#!/usr/bin/python
# Filename: aboutlambda

li = [{'age':25, 'name':'def'},{'age':21, 'name':'abc'},{'age':22, 'name':'ppp'}]
li = sorted(li, key=lambda x:x['age'])
print(li)
