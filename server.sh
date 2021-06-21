#!/bin/bash
if [ -d bin ]
then 
   if [ -d bin/lib ]
   then
    cd bin 
    ./mpm-server $1 $2
    cd ..
    fi
fi