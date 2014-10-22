#########################################################################
# File Name: build.sh
# Author: finlay
# mail: liufy.thu@gmail.com
# Created Time: Mon 01 Sep 2014 12:31:30 AM CST
#########################################################################
#!/bin/bash
make 
dmesg -c
echo ################################
echo local prints!!!!
echo ################################
insmod hello.ko
rmmod hello
dmesg -c
echo ################################
echo prints ended!!!!!
make clean
