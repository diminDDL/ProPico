#!/bin/bash
cd ./build/
make
cd ..
name=$(find ./build/ -name "*.uf2")
echo "found: $name"
found_device=false
for device in /dev/ttyACM*
do
    if [ -e "$device" ]
    then
        stty -F $device 1200
        found_device=true
    else
        echo "No device matching $device found."
    fi
done

if [ "$found_device" = false ] ; then
    echo "No devices matching /dev/ttyACM* found."
fi

RPI="/media/$USER/RPI-RP2/"
echo "waiting for RPI..."
timeout=0
while [ ! -d "$RPI" ]; do
    sleep 1
    timeout=$((timeout+1))
    if [ $timeout -gt 10 ]; then
        echo "RPI not found"
        exit 1
    fi
done
echo "found $RPI"
cp $name /media/$USER/RPI-RP2/
