SUMMARY = "Gateway image"
LICENSE = "CLOSED"
inherit core-image

IMAGE_FEATURES += "debug-tweaks"

IMAGE_INSTALL:append = " fbset"

IMAGE_INSTALL:append = " fbida "

IMAGE_INSTALL:append = " \
  gcc gcc-symlinks \
  g++ g++-symlinks \
  binutils \
  make cmake git gdb pkgconfig \
  glibc-dev \
  linux-libc-headers-dev \
  libstdc++-dev \
"
IMAGE_INSTALL:append = " fb0fill"

IMAGE_INSTALL:append = " touchread"

IMAGE_INSTALL:append = " evtest"

IMAGE_INSTALL:append = " lvgl-hello"

IMAGE_INSTALL:append = " packagegroup-ssh packagegroup-support packagegroup-wifi"

IMAGE_INSTALL:append = " wifi-auto"
