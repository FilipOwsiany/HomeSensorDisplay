SUMMARY = "Gateway image"
LICENSE = "CLOSED"
inherit core-image

# Przydatne na dev (usuń debug-tweaks na produkcji)
IMAGE_FEATURES += "debug-tweaks"

# Wybierz moduły (packagegroupy) – patrz niżej:
IMAGE_INSTALL:append = " packagegroup-ssh packagegroup-support packagegroup-wifi"

# Jeśli chcesz od razu auto-Wi-Fi (opcjonalnie, gdy dodasz receptę wifi-auto):
IMAGE_INSTALL:append = " wifi-auto"
