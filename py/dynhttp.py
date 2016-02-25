#!/usr/bin/env python
# coding=utf-8

import urllib2

req = urllib2.Request("GET /nic/update?hostname=yourhostname&myip=ipaddress&wildcard=NOCHG&mx=NOCHG&backmx=NOCHG HTTP/1.0")
req.add_header("Host", "members.dyndns.org")
req.add_header("Authorization", urllib.urlencode({"username":"root","password":"ROOTXXOO"}))
req.add_header("User-Agent", "")

res = urllib2.urlopen(req)
html = res.read()
print res.info()


#!/usr/bin/env python
#coding=utf8
 
import httplib
  
httpClient = None
   
try:
    httpClient = httplib.HTTPConnection('localhost', 80, timeout=30)
    httpClient.request('GET', '/nic/update')
              
    #response是HTTPResponse对象
    response = httpClient.getresponse()
    print response.status
    print response.reason
    print response.read()
except Exception, e:
    print e
finally:
    if httpClient:
        httpClient.close()
