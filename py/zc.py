#! /usr/bin/python
#
# Grandstream Phone - SIP Provision
#

import socket
import struct
import sys
import re
import time

from optparse import OptionParser
from string import Template

class gs_phone(object):
    """Basic representation of a gs phone."""

    def __init__(self, mac=None, ip=None, ven=None, mod=None, fw=None):
        self.mac_addr = mac
        self.ip_addr = ip
        self.sip_port = 5060
        self.vendor = ven
        self.model = mod
        self.fw_version = fw

    def __request(self):
        subscribe = "SUBSCRIBE sip:MAC%3A%s@224.0.1.75 SIP/2.0\r\n" % self.mac_addr
        subscribe += "Via: SIP/2.0/UDP %s:%s;branch=z9hG4bK1520506307\r\n" % (self.ip_addr, self.sip_port)
        subscribe += "From: <sip:MAC%3A%s@224.0.1.75>;tag=SIPpTag000000\r\n" % self.mac_addr
        subscribe += "To: <sip:MAC%3A%s@224.0.1.75>\r\n" % self.mac_addr 
        subscribe += "Call-ID: 1263455462-5060-1@BJC.BGI.C.BAC\r\n"
        subscribe += "CSeq: 10 SUBSCRIBE\r\n"
        subscribe += "Contact: <sip:%s:%s>\r\n" % (self.ip_addr, self.sip_port)
        subscribe += "Max-Forwards: 70\r\n"
        subscribe += "User-Agent: %s %s %s\r\n" % (self.vendor, self.model, self.fw_version)
        subscribe += "Expires: 0\r\n"
        subscribe += "Supported: replaces, path, timer, eventlist\r\n"
        subscribe += "Event: ua-profile;profile-type=\"device\";vendor=\"%s\";model=\"%s\";version=\"%s\"\r\n" % (self.vendor, self.model, self.fw_version)
        subscribe += "Accept: application/url\r\n"
        subscribe += "Allow: INVITE, ACK, OPTIONS, CANCEL, BYE, SUBSCRIBE, NOTIFY, INFO, REFER, UPDATE, MESSAGE\r\n"
        subscribe += "Content-Length: 0\r\n"
        return subscribe

def get_sip_info(text):
    """Get some relevant SIP information which we need in order to generate the responses."""
    
    lines = text.split('\r\n')
    method = lines[0][0:6]
    via_header = lines[1]
    from_header = lines[2]
    to_header = lines[3]
    call_id_header = lines[4]
    cseq_header = lines[5]
      
    return (method, via_header, from_header, to_header, call_id_header, cseq_header)

def get_ip_address():
    # This is a simple hack to find our IP address this is the only platform-independent way to obtain the address
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(('www.baidu.com', 0))
    return s.getsockname()[0]

prov_uri = None
parser = OptionParser()
parser.add_option('-s', '--test-server', action="store", dest="server", help="The server IP address to be test")
parser.add_option('-l', '--local-ip', action="store", dest="local_ip", help="Local IP address")
parser.add_option("-v", "--verbose",action="store_true", dest="verbose", default=False,help="make lots of noise")

(options, args) = parser.parse_args()

if not options.local_ip:
    ip_addr = get_ip_address()
else:
    ip_addr = options.local_ip
    
print "Local IP Address is : %s" % ip_addr
print "\nGrandstream multicast sip Provison\n"
print "=" * 80

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
count = 0
while count < 1:
    
    new_phone = gs_phone(mac="000b8227fcef", ip=ip_addr, ven="Grandstream", mod="GXP2160", fw="0.7.24.2")
    sock.sendto(new_phone.__request(), ("224.0.1.75", 5060))
    
    while 1:
        res = sock.recv(10240)
    
        if options.verbose: print res

        (method, via_header, from_header, to_header, call_id_header, cseq_header) = get_sip_info(res)
        (peer_addr, port) = sock.getpeername()
        if method == "NOTIFY":
            ok_response = "SIP/2.0 200 OK\r\n"
            ok_response += via_header + "\r\n"
            ok_response += from_header + "\r\n"
            ok_response += to_header + "\r\n"
            ok_response += call_id_header + "\r\n"
            ok_response += cseq_header + "\r\n"
            ok_response = "User-Agent: %s %s %s\r\n" % (new_phone.vendor, new_phone.model, new_phone.fw_version)
            ok_response = "Allow: INVITE, ACK, OPTIONS, CANCEL, BYE, SUBSCRIBE, NOTIFY, INFO, REFER, UPDATE, MESSAGE\r\n"
            ok_response = "Content-Length: 0\r\n"
            sock.sendto(ok_response, (server, 5060))
        if peer_addr == server:
            break
    
    time.sleep(5)
    count = count + 1