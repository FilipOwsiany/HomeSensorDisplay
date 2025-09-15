SUMMARY = "Wi-Fi essentials group"
LICENSE = "CLOSED"
inherit packagegroup

RDEPENDS:${PN} += " packagegroup-base-wifi linux-firmware-bcm43455"
