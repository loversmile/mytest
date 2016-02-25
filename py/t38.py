#! /usr/bin/python
#
# snom multicast telephone discovery
#
#
# Author: Filip Polsakiewicz <filip.polsakiewicz@snom.de>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

import socket
import struct
import sys
import re
import time
import string



from optparse import OptionParser

class sip_message(object):
    """Basic representation of a snom phone."""

    def __init__(self, isReq=False, ip=None, port=5060, ip2=None, port2=5060, pkt=None, response=""):
        """Default constructor."""
        self.msg_isReq = isReq
        self.msg_rcvIP = ip
        self.msg_rcvPort = port
        self.msg_LocalIP = ip2
        self.msg_LocalPort = port2
        self.msg_method = 'NONE'
        self.msg_pkt = pkt 
        self.msg_url = '\0'
        self.msg_response = response 
        self.msg_CallID = ''
        self.msg_cseqno = ''
        self.msg_Via = ''
        self.msg_From = ''
        self.msg_To = ''
        self.msg_tag = ''

def get_headerline_by_name(buffer, name):
    """find the header line by name"""
    lines = buffer.split('\r\n')
    lineindex = 1
    while True:
        if len(lines[lineindex]) > 2:
            result = int(lines[lineindex].find(name, 0, 10))
            if result != -1:
                return lineindex
            lineindex += 1
        else:
            break    
    return 0

def IncreaseMacAddress(mac):
    """increase the mac value of mac string and process the carry issue"""
    macValue = string.atoi(mac, 16)
    macValue += 1
    mac = "%012x"%macValue
    return mac
    
def parse(text):
    """Parses the incoming ."""
    #try:
    lines = text.split('\r\n')
    msg = sip_message(pkt=text)
    result = int(lines[0][0:7].find('SIP/2.0'))
    if result == -1:
        msg.msg_isReq = True
        print 'this is a request message'
    else:
        msg.msg_response = lines[0][8:11]
        print 'this is a response message'
    print lines
    for line in lines:
        if line[0:3].find('Via') >= 0:
            msg.msg_Via = line
        if line[0:4].find('From') >= 0:
            msg.msg_From = line
            #tmp = line[0:4]
            tmp1 = line.split('@')
            tmp2 = tmp1[1].split('>')
            tmp3 = tmp2[0].split(':')
            i = 0
            for tt in tmp3:
                if i == 0:
                    msg.msg_rcvIP = tmp3[0]
                if i == 1:
                    msg.msg_rcvPort = tmp3[1]
                i += 1
            # msg.msg_rcvIP = tmp3[0]
            # if tmp3[1] != None:
            #     msg.msg_rcvPort = tmp3[1]
            index_tag = line.find('tag')
            msg.msg_tag = line[index_tag:]
        if line[0:2].find('To') >= 0:
            msg.msg_To = line
            tmp = line
            tmp1 = tmp.split('@')
            tmp2 = tmp1[1].split('>')
            tmp3 = tmp2[0].split(':')
            msg.msg_LocalIP = tmp3[0]
            i = 0
            for tt in tmp3:
                if i == 0:
                    msg.msg_LocalIP = tmp3[0]
                if i == 1:
                    msg.msg_LoaclPort = tmp3[1]
                i += 1
        if line[0:7].find('Contact') >= 0:
            pass
        if line[0:4].find('CSeq') >= 0:
            msg.msg_LastCseq = line
            tmp = line.split(' ')
            msg.msg_cseqno = tmp[1]
            msg.msg_method = tmp[2]
            
        if line[0:7].find('Call-ID') >= 0:
            msg.msg_CallID = line
    return msg
    #except:
     #   print "this may be empty packet\r\n"
     #   return None

def get_ip_address():
    # This is a simple hack to find our IP address
    # AFAIK this is the only platform-independent way to obtain the address

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(('snom.com', 0))
    return s.getsockname()[0]

def sendresponse(sock, buffer, msg_info):
    lines = buffer.split('\r\n')
    # Some SIP info we need
    lineindex = get_headerline_by_name(buffer, "Call-ID")
    if lineindex == 0:
        return False
    call_id = lines[lineindex][9:]
    lineindex = get_headerline_by_name(buffer, "CSeq")
    if lineindex == 0:
        return False
    cseq = lines[lineindex][6]
    lineindex = get_headerline_by_name(buffer, "Via")
    if lineindex == 0:
        return False
    via_header = lines[lineindex]
    lineindex = get_headerline_by_name(buffer, "From")
    if lineindex == 0:
        return False
    from_header = lines[lineindex]
    lineindex = get_headerline_by_name(buffer, "To")
    if lineindex == 0:
        return False
    to_header = lines[lineindex]
    ok_response = "SIP/2.0 200 OK\r\n"
    ok_response += via_header + "\r\n"
    ok_response += "Contact: <sip:" + msg_info.msg_LocalIP + ":" + "%d"%msg_info.msg_LocalPort + ";transport=tcp;handler=dum>\r\n"
    ok_response += to_header + "\r\n"
    ok_response += from_header + "\r\n"
    ok_response += "Call-ID: %s\r\n" % call_id
    ok_response += "CSeq: %s NOTIFY\r\nExpires: 0\r\nContent-Length: 0\r\n\r\n" % cseq

    print "from[%s:%d] to[%s:%d]\r\n"%(msg_info.msg_LocalIP, msg_info.msg_LocalPort, msg_info.msg_rcvIP, msg_info.msg_rcvPort)
    #print "%s\r\n"%ok_response
    sock.sendto(ok_response, (msg_info.msg_rcvIP, msg_info.msg_rcvPort))
    return True

multicast_address = None
mac_address="000b82000001"
default_multicast_address = '192.168.124.105'


parser = OptionParser()
parser.add_option('-d', '--multicast', action="store", dest="multicast_address", help="PDS discovery address")
parser.add_option('-r', '--multicast port', action="store", dest="multicast_port", help="PDS discovery port")
parser.add_option('-l', '--localip', action="store", dest="local_ip", help="local ip address")
parser.add_option('-p', '--localport', action="store", dest="local_port", help="local port number")
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="make lots of noise")
parser.add_option('-t', '--interval', action="store", dest="test_interval", help="test interval")
parser.add_option("-c", "--count", action="store", dest="request_num", help="request amount of sip subscribe with increasing MAC")
(options, args) = parser.parse_args()


print """
  '__'
  (oo)
  (__)````````)\\
     ||-----||  *
"""
print "multicast SUBSCRIBE is sent to %s\n" % default_multicast_address


#if not options.local_ip:
#    ip_adr = get_ip_address()
#else:
#    ip_adr = options.local_ip
ip_adr = '192.168.124.27'

if not options.multicast_address:
    multicast_address = default_multicast_address
else:
    multicast_address = options.multicast_address


if not options.multicast_port:
    multicast_port = 5060
else:
    multicast_port = int(options.multicast_port)


if not options.local_port:
    local_port = int(5060)
else:
    local_port = options.local_port

if not options.request_num:
    request_num = int(100)
else:
    request_num = int(options.request_num)


if not options.test_interval:
    test_interval = float(1)
else:
    test_interval = float(options.test_interval)


print "Local IP Address is :: %s" % ip_adr
print "=" * 80
#time.sleep(20)

cseq = 1
url_count = 0
test_count = 0

# Create a socket to send data
sendsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sendsock.bind(('%s' % ip_adr, local_port))
sendsock.settimeout(1)

print "test is started, request amount is %d\r\n"%request_num

# send multicast SUBSCRIBE request to local network...
while True:
    if request_num <= 0:
        break
    request_num -= 1    
    #if options.verbose: 
    print mac_address
    
    SEND_100TRYING = '''\
    SIP/2.0 100 Trying
    [last_Via:]
    [last_From:]
    [last_To:]
    [last_Call-ID:]
    [last_CSeq:]
    Contact: <sip:[local_ip]:[local_port]>
    Content-Length: 0
'''
    SEND_180RINGING = '''\
    SIP/2.0 180 Ringing
    [last_Via:]
    [last_From:]
    [last_To:];tag=[call_number]
    [last_Call-ID:]
    [last_CSeq:]
    Contact: <sip:[local_ip]:[local_port]>
    Content-Length: 0
'''
    SEND_200OK_1 = '''\
    SIP/2.0 200 OK
    [last_Via:]
    [last_From:]
    [last_To:];tag=[call_number]
    [last_Call-ID:]
    [last_CSeq:]
    Contact: <sip:[local_ip]:[local_port]>
    Remote-Party-ID: "mymei" <sip:1002@[local_ip]>;party=called;privacy=off;screen=no
    Content-Type: application/sdp
    Content-Length:[len]
    
    v=0
    o=system 8000 8000 IN IP[local_ip_type] [local_ip]
    s=SIP Call
    t=0 0
    m=audio [media_port+1000] RTP/AVP 0
    c=IN IP[media_ip_type] [media_ip]
    a=rtpmap:0 PCMU/8000
    a=ptime:20
    a=sendrecv
    m=video [media_port+1002] RTP/AVP 99
    a=sendrecv
    a=rtpmap:99 h264/90000
    '''
    SEND_200OK_2 = '''\
    SIP/2.0 200 OK
    [last_Via:]
    [last_From:]
    [last_To:];tag=[call_number]
    [last_Call-ID:]
    [last_CSeq:]
    Contact: <sip:[local_ip]:[local_port];transport=[transport]>  
    Content-Length: 0
    '''
    SEND_INVITE ='''\
    INVITE sip:1000@[remote_ip]:[remote_port] SIP/2.0
    Via: SIP/2.0/[transport] [local_ip]:[local_port];branch=[branch]
    From: <sip:2003@[local_ip]:[local_port]>;tag=[call_number]
    To: "Digital_1"<sip:1000@[remote_ip]:[remote_port]>[peer_tag_param]
    Call-ID: [call_id]
    CSeq: [cseq] INVITE
    Contact: sip:2003@[local_ip]:[local_port]
    Max-Forwards: 70
    Content-Type: application/sdp
    Content-Length: [len]
       
    v=0
    o=- 60159 61142 IN IP[local_ip_type] [local_ip]
    s=-
    c=IN IP[media_ip_type] [media_ip]
    t=0 0
    m=image 4697 udptl t38
    a=T38FaxVersion:0
    a=T38MaxBitRate:14400
    a=T38FaxFillBitRemoval:0
    a=T38FaxTranscodingMMR:0
    a=T38FaxTranscodingJBIG:0
    a=T38FaxRateManagement:transferredTCF
    a=T38FaxUdpEC:t38UDPRedundancy
    a=T38FaxMaxBuffer:128
    '''
    SEND_ACK = '''\
    ACK sip:1000@[remote_ip] SIP/2.0
    Via: SIP/2.0/[transport] [local_ip]:[local_port];branch=[branch];rport
    From: <sip:2003@[local_ip]:[local_port]>;tag=[call_number]
    To: "Digital_1"<sip:1000@[remote_ip]:[remote_port]>[peer_tag_param]
    Call-ID: [call_id]
    CSeq: [cseq] ACK
    Contact: sip:2003@[local_ip]:[local_port]
    Max-Forwards: 70
    Content-Length: 0
    '''

    call_number = ''#unknown
    branch = ''#unknown

    local_ip = ''
    local_port = ''
    remote_ip = ''
    remote_port = ''
    call_id = ''
    recv_retry =2
    subs = ''
    tag = ''
    while True:
        try:
            subs, rcvaddress= sendsock.recvfrom(10240)
        except:
            print "there is no packet before INVITE %s\r\n" % recv_retry
        service_msg = parse(subs)
        if service_msg:
            print 'method ===='+service_msg.msg_method
        if service_msg and service_msg.msg_method == 'INVITE':
            local_ip = service_msg.msg_LocalIP
            local_port = service_msg.msg_LocalPort
            remote_ip = service_msg.msg_rcvIP
            remote_port = service_msg.msg_rcvPort
            call_id = service_msg.msg_CallID[9:]
            tag = service_msg.msg_tag
            REAL_SEND_100TRYING = SEND_100TRYING.replace('[last_Via:]', service_msg.msg_Via).\
                replace('[last_Call-ID:]', service_msg.msg_CallID).\
                replace('[last_From:]', service_msg.msg_From).\
                replace('[last_To:]', service_msg.msg_To).\
                replace('[last_CSeq:]', service_msg.msg_LastCseq).\
                replace('[local_ip]', local_ip).\
                replace(':[local_port]','' if local_port == '' else (':'+str(local_port)))
            REAL_SEND_180RINGING = SEND_180RINGING.replace('[last_Via:]', service_msg.msg_Via).\
                replace('[last_Call-ID:]', service_msg.msg_CallID).\
                replace('[last_From:]', service_msg.msg_From).\
                replace('[last_To:]', service_msg.msg_To).\
                replace('[last_CSeq:]', service_msg.msg_LastCseq).\
                replace('[local_ip]', local_ip).\
                replace(':[local_port]','' if local_port == '' else (':'+str(local_port)))
            sendsock.sendto(REAL_SEND_100TRYING, (multicast_address, multicast_port))
            sendsock.sendto(REAL_SEND_180RINGING, (multicast_address, multicast_port))
            REAL_SEND_200OK_1 = SEND_200OK_1.replace('[last_Via:]', service_msg.msg_Via).\
                replace('[last_Call-ID:]', service_msg.msg_CallID).\
                replace('[last_From:]', service_msg.msg_From).\
                replace('[last_To:]', service_msg.msg_To).\
                replace('[last_CSeq:]', service_msg.msg_LastCseq).\
                replace('[local_ip]', local_ip).\
                replace(':[local_port]','' if local_port == '' else (':'+str(local_port)))
            for i in (1,2):
                sendsock.sendto(REAL_SEND_200OK_1, (multicast_address, multicast_port))
            print "100--"+REAL_SEND_100TRYING
            print "180--"+REAL_SEND_180RINGING
            print "200--"+REAL_SEND_200OK_1

            break
        time.sleep(3)

    while True:
        try:
            subs, rcvaddress= sendsock.recvfrom(10240)
        except:
            print "there is no packet before ACK %s\r\n" % recv_retry
        service_msg = parse(subs)
        if service_msg and service_msg.msg_method == 'ACK':
            REAL_SEND_INVITE = SEND_INVITE.replace('[remote_ip]', remote_ip).\
                replace(':[remote_port]','' if remote_port == '' else (':'+str(remote_port))).\
                replace('[local_ip]', local_ip).\
                replace(':[local_port]', '' if local_port == '' else (':'+str(local_port))).\
                replace('[call_id]', call_id).\
                replace('[peer_tag_param]', (';'+tag)).\
                replace('[cseq]', service_msg.msg_cseqno).\
                replace('[transport]', 'UDP').\
                replace('[call_number]', call_number).\
                replace('[branch]', branch)#----------------------
            for i in (1,2):
                sendsock.sendto(REAL_SEND_INVITE, (multicast_address, multicast_port))
                sendsock.sendto(REAL_SEND_INVITE, (multicast_address, multicast_port))
            break
        time.sleep(3)

    while True:
        try:
            subs, rcvaddress= sendsock.recvfrom(10240)
        except:
            print "there is no packet before 100 %s\r\n" % recv_retry
        service_msg = parse(subs)
        if service_msg and service_msg.response == '100':
            break
        time.sleep(3)

    while True:
        try:
            subs, rcvaddress= sendsock.recvfrom(10240)
        except:
            print "there is no packet before 200 %s\r\n" % recv_retry
        service_msg = parse(subs)
        if service_msg and service_msg.response == '200':
            REAL_SEND_ACK = SEND_ACK.replace('[remote_ip]', remote_ip).\
                replace(':[remote_port]','' if remote_port == '' else (':'+str(remote_port))).\
                replace('[local_ip]', local_ip).\
                replace(':[local_port]', '' if local_port == '' else (':'+str(local_port))).\
                replace('[call_id]', call_id).\
                replace('[peer_tag_param]', (';'+tag)).\
                replace('[cseq]', service_msg.msg_cseqno).\
                replace('[transport]', 'UDP').\
                replace('[call_number]', call_number).\
                replace('[branch]', branch)#----------------------
            for i in (1,2):
                sendsock.sendto(REAL_SEND_ACK, (multicast_address, multicast_port))
            break
        time.sleep(3)

    while True:
        try:
           subs, rcvaddress= sendsock.recvfrom(10240)
        except:
            print "there is no packet before BYE %s\r\n" % recv_retry 
        service_msg = parse(subs)
        if service_msg and service_msg.msg_method == 'BYE':
            REAL_SEND_200OK_2 = SEND_200OK_2.replace('[last_Via:]', service_msg.msg_Via).\
                replace('[last_Call-ID:]', service_msg.msg_CallID).\
                replace('[last_From:]', service_msg.msg_From).\
                replace('[last_To:]', service_msg.msg_To).\
                replace('[last_CSeq:]', service_msg.msg_LastCseq).\
                replace('[local_ip]', local_ip).\
                replace(':[local_port]','' if local_port == '' else (':'+str(local_port))).\
                replace('[transport]', 'UDP').\
                replace('[call_number]', call_number)
            sendsock.sendto(REAL_SEND_200OK_2, (multicast_address, multicast_port))
            break
        time.sleep(3)


    time.sleep(3)

sendsock.close()

print "=" * 40
print "Final Statistics:"
print "try %d times, get %d valid url\r\n" % (test_count , url_count)
print "=" * 40
time.sleep(1)
