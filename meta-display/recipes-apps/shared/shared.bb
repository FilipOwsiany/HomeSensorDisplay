SUMMARY = "HomeSensor liblary (shared)"
HOMEPAGE = "https://lvgl.io"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "gitsm://github.com/FilipOwsiany/HomeSensorGUI.git;protocol=https;branch=main"

PV = "1.0+git${SRCPV}"
SRCREV = "e0fd3e77e570f5c29a85958123f8d5b459fa8783"

S = "${WORKDIR}/git/shared/ipcSocket"

inherit cmake systemd

PACKAGES = "${PN}"
FILES_SOLIBSDEV = ""
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

EXTRA_OECMAKE = "-DCMAKE_BUILD_TYPE=Release"

do_install() {
    install -d ${D}${libdir}
    install -m 0644 ${B}/libipcSocket.so ${D}${libdir}/libipcSocket.so
}

FILES:${PN} += " \
    ${libdir}/libipcSocket.so \
"