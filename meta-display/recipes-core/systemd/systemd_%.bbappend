# meta-display/recipes-core/systemd/systemd_%.bbappend

do_install:append() {
    # 1. Usuń autostart getty na tty1 (jeśli jest jako symlink)
    rm -f ${D}${sysconfdir}/systemd/system/getty.target.wants/getty@tty1.service

    # 2. Zamaskuj getty@tty1.service – nawet generator go nie odpali
    install -d ${D}${sysconfdir}/systemd/system
    ln -sf /dev/null ${D}${sysconfdir}/systemd/system/getty@tty1.service
}
