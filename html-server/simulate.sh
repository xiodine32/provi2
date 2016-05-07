#!/bin/sh
# make sure that backup.txt is renamed into backup.old
if [ ! -f backup.old ]; then
    cp backup.txt backup.old
fi

# make sure that commands.txt is renamed into commands.backup
if [ ! -f commands.backup ]; then
    cp commands.txt commands.backup
fi

# make
make

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
kill $!

# delete 'old' backup
rm backup.txt

# move new backup in place
mv backup2.txt backup.txt

# run server
./server &

# wait 5 more seconds
sleep 5

# kill server
kill $!
