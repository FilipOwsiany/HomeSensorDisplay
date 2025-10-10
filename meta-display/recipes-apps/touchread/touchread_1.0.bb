SUMMARY = "Simple reader touch app"
DESCRIPTION = "Small evdev-based touchscreen reader"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://touchread.c \
           file://Makefile \
"

S = "${WORKDIR}"

do_compile() {
    oe_runmake
}

do_install() {
    oe_runmake install DESTDIR=${D}
}

FILES:${PN} = "${bindir}/touchread"
