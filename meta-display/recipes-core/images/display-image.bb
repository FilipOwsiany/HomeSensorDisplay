SUMMARY = "Custom Display Image"
DESCRIPTION = "Custom Display Image for Raspberry Pi 5"
LICENSE = "MIT"

python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*  Example recipe created by bitbake-layers!  *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

addtask display_banner before do_build

inherit core-image

IMAGE_FEATURES += "ssh-server-dropbear"

IMAGE_INSTALL += "\
    fb0fill \
    frontend \
    backend \
    touchread \
    wifi-auto \
    hcsr501 \
    kernel-module-hcsr501 \
    rsync \
"
