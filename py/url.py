#!/usr/bin/python
'''
import os
import urllib2
ret = urllib2.urlopen("http://www.baidu.com")
print ret

rett = urllib2.urlopen("https://192.168.124.157:8089/cgi?action=getWarningDB&warning_general=all")
print rett
'''
''' t
import urllib2
try:
    response = urllib2.urlopen('http://www.baidu.com')
    print response.read()
except urllib2.HTTPError, e:
    print e.code
'''
'''
import urllib2
import cookielib
 
#cookie = cookielib.CookieJar("TRACKID=90bd09d60080e4c16ea3080c2f021b49; session-identify=sid140997603-1399451234; username=admin; user_id=0; position=home; locale=zh-CN")
cookie = "TRACKID=90bd09d60080e4c16ea3080c2f021b49; session-identify=sid140997603-1399451234; username=admin; user_id=0; position=home; locale=zh-CN"
#cookie = cookielib.CookieJar()
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookie))
response = opener.open('https://192.168.124.157:8089/cgi?action=getWarningDB&warning_general=all')
print response.read()
for item in cookie:
#    if item.name == 'some_cookie_item_name':
        print item.value
'''

import urllib2 
req = urllib2.Request("https://192.168.124.173:8089/cgi?action=listWarningLog&options=id%2Ctime%2Caction%2Ccontent%2Crow_num&item_num=10&page=1&sidx=time&sord=desc")
req.add_header( "Cookie" ,"TRACKID=0ac851a272d8e111b17cf10c92c976b6; session-identify=sid2052708374-1408435689; username=admin; user_id=0; position=home; locale=en-US; jumpMenu=warning.html; needApplyChange=yes")
i = 0
for i in range(0, 100000):
#while 1:
    res = urllib2.urlopen( req ) 
    html = res.read() 
    print html
    res.close()
