#!/bin/sh

if [ $# -lt 1 ]; then
    echo "Please input shell file"
else
    nohup sh $1 >> /dev/null 2>&1 &
fi
