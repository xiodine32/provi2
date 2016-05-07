#!/bin/sh
mv backup.txt backup2.txt
cp backup.old backup.txt
xdg-open http://localhost:1030
./server &
sleep 5
pkill server
rm backup.txt
mv backup2.txt backup.txt
./server
