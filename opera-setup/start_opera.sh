#!/bin/sh

# echo $1

# sudo ./opera_nic 192.168.1.1 configs/node-1-link.csv 60
cd /opt/bpf-examples/opera-version1
nohup sudo ./opera_nic $1 configs/$2.csv 60 > /dev/null 2>&1 &
