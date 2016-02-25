#/bin/sh

#a="1"
#b="0"
#c="1"
#d="0"
#
#loop=1
#if [ $a == "1" ];then
#    arr[$loop]="a"
#    loop=`expr $loop + 1`
#    echo $loop + "a"
#fi
#if [ $b == "1" ];then
#    arr[$loop]="b"
#    loop=`expr $loop + 1`
#    echo $loop + "b"
#fi
#
#if [ $c == "1" ];then
#    arr[$loop]="c"
#    loop=`expr $loop + 1`
#    echo $loop + "c"
#fi
#if [ $d == "1" ];then
#    arr[$loop]="d"
#    loop=`expr $loop + 1`
#    echo $loop + "d"
#fi
#
#    loop=`expr $loop - 1`
#
#echo $loop
#echo ${arr[0]}
#echo ${arr[1]}
#
#for i in `seq $loop` ;do   
#    echo "${arr[$i]}"  
#done 
monitor=1
meetme=0
queue=0
vm=1
fax=1
loop=1
if [ $monitor == "1" ]; then
    arr[$loop]="monitor"
    loop=`expr $loop + 1`
fi
if [ $meetme == "1" ]; then
    arr[$loop]="meetme"
    loop=`expr $loop + 1`
fi
if [ $queue == "1" ]; then
    arr[$loop]="queue"
    loop=`expr $loop + 1`
fi
if [ $vm == "1" ]; then
    arr[$loop]="voicemail"
    loop=`expr $loop + 1`
fi
if [ $fax == "1" ]; then
    arr[$loop]="fax"
    loop=`expr $loop + 1`
fi
loop=`expr $loop - 1`
echo $loop
for delfile in `seq $loop` ;do 
    echo ${arr[$delfile]} 
#    echo $delfile
done
