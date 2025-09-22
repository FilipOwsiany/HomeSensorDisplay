#!/bin/bash
(
    lsblk | grep sda >> /dev/null
    is_present=$?
    if [ $is_present -ne 0 ]; then
        echo "No /dev/sda found. Exiting."
        exit 1
    fi
    echo "Found /dev/sda."

    cd poky-rpi/build/tmp/deploy/images/raspberrypi5-gateway || exit 1
    echo "Flashing..."

    set -e

    bzcat gateway-image-raspberrypi5-gateway.rootfs.wic.bz2 | sudo dd of=/dev/sda bs=4M status=progress conv=fsync
    echo "Done."
)