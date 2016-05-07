#!/bin/sh
# make sure that backup.txt is renamed into backup.old before
# running script over and over again.

# call fix
./punctaje_fix

# revert changes to the old version
mv backup.txt backup2.txt
cp backup.old backup.txt

# open server with old changes
xdg-open http://localhost:1030
./server &

# wait for 5 seconds to preview changes
sleep 5

# kill server
pkill server

# delete 'old' backup
rm backup.txt

# move new backup in place
mv backup2.txt backup.txt

# run server
./server
