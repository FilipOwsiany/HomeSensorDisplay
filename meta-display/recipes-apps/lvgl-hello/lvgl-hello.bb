SUMMARY = "Minimal LVGL hello (fbdev + evdev)"
HOMEPAGE = "https://lvgl.io"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://CMakeLists.txt \
    file://lv_conf.h \
    file://main.c \
    file://lvgl_hello.service \
"

S = "${WORKDIR}"

inherit cmake pkgconfig
inherit systemd

# Unit systemd
SYSTEMD_SERVICE:${PN} = "lvgl_hello.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

LVGL_LOCAL_PATH ?= "/home/filip/git/HomeSensorDisplay/displayApplication/lvgl"

EXTRA_OECMAKE = "-DLV_CONF_INCLUDE_SIMPLE=ON -DCMAKE_BUILD_TYPE=Release"

addtask prepare_lvgl after do_unpack before do_configure

do_prepare_lvgl[cleandirs] = "${S}/lvgl"
do_prepare_lvgl() {
    if [ ! -d "${LVGL_LOCAL_PATH}" ]; then
        bbfatal "LVGL_LOCAL_PATH dot not exist: ${LVGL_LOCAL_PATH}"
    fi

    install -d ${S}/lvgl
    cp -R "${LVGL_LOCAL_PATH}/." "${S}/lvgl/"
}

do_install:append() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/lvgl-hello ${D}${bindir}/lvgl-hello

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/lvgl_hello.service \
        ${D}${systemd_system_unitdir}/lvgl_hello.service
}

FILES:${PN} += " \
    ${bindir}/lvgl-hello \
    ${systemd_system_unitdir}/lvgl_hello.service \
"
