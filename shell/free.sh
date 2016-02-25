#!/bin/sh
#FreeDNS updater script

UPDATEURL="http://freedns.afraid.org/dynamic/update.php?MEtZcGlKeEpQNkRIbW1USVpWUlc1VzZNOjEzMDI2MzEw"
DOMAIN="loversonnao.strangled.net"

registered=$(nslookup $DOMAIN|tail -n2|grep A|sed s/[^0-9.]//g)
echo "registered: ${registered}"

current=$(wget -q -O - http://checkip.dyndns.org|sed s/[^0-9.]//g)
echo "current: $current"

[ "$current" != "$registered"  ] && {
       wget -q -O /dev/null $UPDATEURL
       echo "DNS updated on:"; date
}
