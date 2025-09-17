#!/bin/bash
(
    lsblk | grep sdb >> /dev/null
    is_present=$?
    if [ $is_present -ne 0 ]; then
        echo "No /dev/sdb found. Exiting."
        exit 1
    fi
    echo "Found /dev/sdb."

    cd poky-rpi/build/tmp/deploy/images/raspberrypi5 || exit 1
    echo "Flashing..."

    set -e

    bzcat gateway-image-raspberrypi5.rootfs.wic.bz2 | sudo dd of=/dev/sdb bs=4M status=progress conv=fsync
    echo "Done."
)