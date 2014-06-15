#!/bin/sh

sudo more /proc/31479/smaps | grep "Size" | awk 'BEGIN{sum=0;}; { sum+= $2; }; END{print sum;}'
sudo more /proc/31479/smaps | grep "Rss" | awk 'BEGIN{sum=0;}; { sum+= $2; }; END{print sum;}'
sudo more /proc/31479/smaps | grep "Shared_Clean" | awk 'BEGIN{sum=0;}; { sum+= $2; }; END{print sum;}'
sudo more /proc/31479/smaps | grep "Shared_Dirty" | awk 'BEGIN{sum=0;}; { sum+= $2; }; END{print sum;}'
