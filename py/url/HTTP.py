#!/usr/bin/python

import httplib, urllib
#params = urllib.urlencode({'@number': 12524, '@type': 'issue', '@action': 'show'})
params = urllib.urlencode({'<userListRequest><mac>000b826731fc</mac><model>GXV3240V1.7A</model><version>1.0.3.9</version><totalAcctNum>6</totalAcctNum><supportLdap>true</supportLdap></userListRequest>': None}) 
#params = '<userListRequest><mac>000b826731fc</mac><model>GXV3240V1.7A</model><version>1.0.3.9</version><totalAcctNum>6</totalAcctNum><supportLdap>true</supportLdap></userListRequest>'
print params
headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
conn = httplib.HTTPConnection("bugs.python.org")
conn.request("POST", "", params, headers)
#response = conn.getresponse()
#print response.status, response.reason
#data = response.read()
#print data
conn.close()
