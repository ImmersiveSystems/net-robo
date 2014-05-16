#!/bin/bash

echo "Running WIFI check script"

while true ; do
   if ifconfig wlan0 | grep -q "inet addr:" ; then
      echo "Network okay!"
      sleep 1
   else
      echo "Network connection down! Attempting reconnection."
      sudo ifup --force wlan0
      sleep 1
   fi
done
