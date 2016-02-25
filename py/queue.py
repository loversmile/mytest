import os



##########################main
file=open('/cfg/etc/qmail/control/queuelifetime','w+')
#file=open('ttt','w+')
context="180"
file.write(context)
file.close()

if [ ! -f "${AST_BASE}/qmail/control/queuelifetime" ];then
    echo -n 3600 > ${AST_BASE}/qmail/control/queuelifetime
fi
