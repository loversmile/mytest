#!/usr/bin/python

import urllib2 
'''
req = urllib2.Request("https://192.168.124.101:8089/cgi?action=challenge&user=admin")
req.add_header( "Cookie" ,"TRACKID=c2f190088f4093e3323337e3ed7bf7e2; session-identify=sid34104180-1417774174; locale=en-US")
res = urllib2.urlopen( req ) 
html = res.read() 
print html
res.close()
'''
req = urllib2.Request("https://192.168.124.101:8089/cgi?action=login&user=admin&token=1e70ae873307d789a7ca671d550185ab")
req.add_header( "Cookie" ,"session-identify=sid34104180-1417774174")
res = urllib2.urlopen( req ) 
html = res.read()
aa = res.info()
'''
print html
print aa
print aa["Set-Cookie"]
print '---------------------'
for row in aa:
    #if row[0:7] == "session":
    print row
    print '+++++++'
    print aa[row]
print '[][][][][][][][][][]'
result = aa["Set-Cookie"]

index = result.find('session-identify')
print index
print result[61:]
'''

index = aa['Set-Cookie'].find('session-identify')
result = aa['Set-Cookie'][index:]
print result

res.close()
