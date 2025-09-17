SUMMARY = "Simple framebuffer filler with logs"
DESCRIPTION = "fb0fill fills /dev/fb0 with a chosen color (name, #RRGGBB, or R,G,B)"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://fb0fill.c \
           file://Makefile \
           file://fb0fill.service \
           file://default \
"

S = "${WORKDIR}"

inherit systemd

do_compile() {
    oe_runmake
}

do_install() {
    oe_runmake install DESTDIR=${D}

    # /etc/default
    install -d ${D}${sysconfdir}/default
    install -m 0644 ${WORKDIR}/default ${D}${sysconfdir}/default/fb0fill

    # unit systemd (właściwe miejsce!)
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/fb0fill.service ${D}${systemd_system_unitdir}/fb0fill.service
}

FILES:${PN} += "${bindir}/fb0fill \
                ${sysconfdir}/default/fb0fill \
                ${systemd_system_unitdir}/fb0fill.service \
"

# Autostart (zmień na "disable" jeśli nie chcesz od razu)
SYSTEMD_SERVICE:${PN} = "fb0fill.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"
