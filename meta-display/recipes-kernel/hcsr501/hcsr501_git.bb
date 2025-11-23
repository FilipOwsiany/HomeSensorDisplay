SUMMARY = "HC-SR501 PIR kernel module"
DESCRIPTION = "Out-of-tree Linux kernel module for the HC-SR501 PIR motion sensor"

LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "gitsm://github.com/FilipOwsiany/HomeSensorKernel.git;protocol=https;branch=main"

PV = "1.0+git${SRCPV}"
SRCREV = "51178695c5054b65e554934f7f85ac80ee8d473f"

S = "${WORKDIR}/git"

inherit module

EXTRA_OEMAKE += "KERNELDIR=${STAGING_KERNEL_DIR}"

do_configure() {
    :
}

do_compile() {
    oe_runmake -C ${S}
}

do_install() {
    install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra
    install -m 0644 ${S}/hcsr501.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra/

    install -d ${D}${sysconfdir}/modprobe.d
    echo "options hcsr501 gpio=586" > ${D}${sysconfdir}/modprobe.d/hcsr501.conf

    install -d ${D}${sysconfdir}/modules-load.d
    echo "hcsr501" > ${D}${sysconfdir}/modules-load.d/hcsr501.conf
}

FILES:${PN} += " \
    ${sysconfdir}/modprobe.d/hcsr501.conf \
    ${sysconfdir}/modules-load.d/hcsr501.conf \
"
