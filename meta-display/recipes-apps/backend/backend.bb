SUMMARY = "HomeSensor Backend Application (backend)"
HOMEPAGE = "https://lvgl.io"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "gitsm://github.com/FilipOwsiany/HomeSensorGUI.git;protocol=https;branch=main \
           file://backend.service \
"

PV = "1.0+git${SRCPV}"
SRCREV = "69886fd1e07201a39c654a46c6c760d93b53c069"

S = "${WORKDIR}/git/backend"

inherit cmake systemd

PACKAGES = "${PN}"
FILES_SOLIBSDEV = ""
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

EXTRA_OECMAKE = "-DCMAKE_BUILD_TYPE=Release"

DEPENDS += "openssl"

do_install() {

    install -d ${D}${bindir}
    install -m 0755 ${B}/bin/backend ${D}${bindir}/backend

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/backend.service \
        ${D}${systemd_system_unitdir}/backend.service
}

FILES:${PN} += " \
    ${bindir}/backend \
    ${libdir}/libassets.so \
    ${systemd_system_unitdir}/backend.service \
"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "backend.service"
SYSTEMD_AUTO_ENABLE = "enable"
