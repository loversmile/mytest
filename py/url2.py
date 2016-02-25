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
req = urllib2.Request("https://192.168.124.157:8089/cgi?action=updateWarningDB&warning_general={%22PRIMARY%22:%20[{%22key%22:%20%22id%22,%20%22value%22:%201},%20{%22key%22:%20%22item%22,%20%22value%22:%20%22enable%22}],%20%22CONTENT%22:%20{%22value%22:%200}}")
req.add_header( "Cookie" ,"TRACKID=cc473e4111cc481b6fe96d8017675dab; session-identify=sid217522279-1400574678; username=admin; user_id=0; position=home; locale=en-US")
i = 0
for i in range(0, 100000):
#while 1:
    res = urllib2.urlopen( req ) 
    html = res.read() 
    print html
    res.close()
