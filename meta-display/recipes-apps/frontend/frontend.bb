SUMMARY = "HomeSensor LVGL Display Application (frontend)"
HOMEPAGE = "https://lvgl.io"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "gitsm://github.com/FilipOwsiany/HomeSensorGUI.git;protocol=https;branch=main"
SRC_URI += "file://frontend.service"

PV = "1.0+git${SRCPV}"
SRCREV = "e0fd3e77e570f5c29a85958123f8d5b459fa8783"

S = "${WORKDIR}/git/frontend"

inherit cmake systemd

PACKAGES = "${PN}"
FILES_SOLIBSDEV = ""
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

EXTRA_OECMAKE = "-DLV_CONF_INCLUDE_SIMPLE=ON -DCMAKE_BUILD_TYPE=Release"

DEPENDS += "shared"
RDEPENDS:${PN} += "shared"

do_install() {

    install -d ${D}${bindir}
    install -m 0755 ${B}/bin/frontend ${D}${bindir}/frontend

    install -d ${D}${libdir}
    install -m 0644 ${B}/lib/libassets.so    ${D}${libdir}/libassets.so

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/frontend.service \
        ${D}${systemd_system_unitdir}/frontend.service
}

FILES:${PN} += " \
    ${bindir}/frontend \
    ${libdir}/libassets.so \
    ${systemd_system_unitdir}/frontend.service \
"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "frontend.service"
SYSTEMD_AUTO_ENABLE = "enable"
