SECTION = "Kernel"
KERNEL_VERSION ="4.14"
DESCRIPTION = "Linux Kernel ${KERNEL_VERSION} for TRIK"
LICENSE = "GPLv2"
KERNEL_IMAGETYPE = "zImage"

inherit kernel

KERNEL_MODULE_AUTOLOAD += "jex_epwm"
KERNEL_MODULE_AUTOLOAD += "jcx_pwm"

MULTI_CONFIG_BASE_SUFFIX = ""
SRCREV="${AUTOREV}"
SRC_URI = "git://github.com/trikset/ti-linux-kernel;protocol=https;branch=trikset-ti-linux-4.14.y \
	   file://defconfig \
           file://display_settings.sh \
           "

S = "${WORKDIR}/git"
LIC_FILES_CHKSUM="file://COPYING;beginline=1;endline=355;md5=bad9197b13faffd10dfc69bd78fd072e"

PACKAGES += "${PN}-data"
FILES_${PN}-data = "${datadir} ${sysconfdir}"
RDEPENDS_${PN} += "${PN}-data"

RDEPENDS_kernel-base += "kernel-devicetree"

do_install_append(){
    install -p -D -m 0755 -t ${D}${sysconfdir}/trik/ ${WORKDIR}/display_settings.sh
}