SUMMARY = "Gateway image"
LICENSE = "CLOSED"
inherit core-image

IMAGE_FEATURES += "debug-tweaks"

IMAGE_INSTALL:append = " packagegroup-ssh packagegroup-support packagegroup-wifi"

IMAGE_INSTALL:append = " wifi-auto"
