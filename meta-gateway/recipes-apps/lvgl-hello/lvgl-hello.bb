SUMMARY = "Minimal LVGL hello (fbdev + evdev)"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


# Dostarczamy wyłącznie własne pliki źródłowe
SRC_URI = " \
    git://github.com/lvgl/lvgl;protocol=https;branch=release/v9.1;name=lvgl;subdir=git/lvgl \
    file://CMakeLists.txt \
    file://lv_conf.h \
    file://main.c \
"

SRCREV_lvgl = "322e18e05b983231ea766f567ab2bf8e662de987"
SRCREV_FORMAT = "lvgl"
S = "${WORKDIR}"

inherit cmake pkgconfig

EXTRA_OECMAKE = "-DLV_CONF_INCLUDE_SIMPLE=ON -DCMAKE_BUILD_TYPE=Release"

do_install:append() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/lvgl-hello ${D}${bindir}/lvgl-hello
}

