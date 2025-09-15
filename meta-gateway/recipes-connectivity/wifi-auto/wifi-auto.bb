SUMMARY = "Auto Wi-Fi on boot (wpa_supplicant + DHCP)"
LICENSE = "CLOSED"

SRC_URI = "file://wpa_supplicant-wlan0.conf file://wlan0.network"

RDEPENDS:${PN} = "wpa-supplicant"

do_install() {
    # /etc/wpa_supplicant
    install -d ${D}${sysconfdir}/wpa_supplicant
    install -m 0600 ${WORKDIR}/wpa_supplicant-wlan0.conf \
        ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant-wlan0.conf

    # /etc/systemd/network
    install -d ${D}${sysconfdir}/systemd/network
    install -m 0644 ${WORKDIR}/wlan0.network \
        ${D}${sysconfdir}/systemd/network/wlan0.network

    install -d ${D}${sysconfdir}/systemd/system/multi-user.target.wants

    ln -sfn ${systemd_unitdir}/system/wpa_supplicant@.service \
        ${D}${sysconfdir}/systemd/system/multi-user.target.wants/wpa_supplicant@wlan0.service

    ln -sfn ${systemd_unitdir}/system/systemd-networkd.service \
        ${D}${sysconfdir}/systemd/system/multi-user.target.wants/systemd-networkd.service
}

FILES:${PN} += " \
    ${sysconfdir}/wpa_supplicant/wpa_supplicant-wlan0.conf \
    ${sysconfdir}/systemd/network/wlan0.network \
    ${sysconfdir}/systemd/system/multi-user.target.wants/wpa_supplicant@wlan0.service \
    ${sysconfdir}/systemd/system/multi-user.target.wants/systemd-networkd.service \
"
