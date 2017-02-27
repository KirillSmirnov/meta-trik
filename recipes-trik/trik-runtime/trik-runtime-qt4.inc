DECRIPTION = "Runtime for TRIK controller"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=175792518e4ac015ab6696d16c4f607e"
RDEPENDS_${PN} = "locale-default espeak alsa-utils"
#RRECOMMENDS_${PN} = "vlc"
RCONFLICTS_${PN} += "trik-runtime-qt5"
RPROVIDES_${PN} += "trik-runtime"

inherit qt4x11

TAG = "${PV}"

SRC_URI = "git://github.com/trikset/trikRuntime.git;protocol=https;tag=${TAG};nobranch=1 \
          file://trikGui.sh \
          file://qws_display.sh \
          file://trik_runtime_path.sh \
          file://python_path.sh \
          file://gamepad-service"


S = "${WORKDIR}/git"

QMAKE_PROFILES = "${S}/trikRuntime.pro"
EXTRA_QMAKEVARS_PRE = "CONFIG+=nosanitizers"

do_compile(){
        export CROSS_COMPILE="${TARGET_PREFIX}"
        oe_runmake
}

do_install() {
        install -d ${D}/home/root/trik
        cp -rf ${S}/bin/arm-release/* ${D}/home/root/trik

        install -d ${D}/${sysconfdir}/trik/
        install -m 0755 ${WORKDIR}/trikGui.sh ${D}/${sysconfdir}/trik/

        install -d ${D}/${sysconfdir}/init.d/
        install -m 0755 ${WORKDIR}/gamepad-service ${D}/${sysconfdir}/init.d/

        install -d ${D}/${sysconfdir}/profile.d/
        install -m 0755 ${WORKDIR}/qws_display.sh ${D}/${sysconfdir}/profile.d/
        install -m 0755 ${WORKDIR}/trik_runtime_path.sh ${D}/${sysconfdir}/profile.d/
        install -m 0755 ${WORKDIR}/python_path.sh ${D}/${sysconfdir}/profile.d/

        LIBS="trikCommunicator \
              trikControl \
              trikHal \
              trikKernel \
              trikNetwork \
              trikScriptRunner \
              trikTelemetry \
              trikWiFi"

        install -d ${D}${includedir}/trikRuntime
        cp -rf ${S}/qslog/*.h ${D}${includedir}/trikRuntime
        for lib in $LIBS; do
            cp -rf ${S}/${lib}/include/* ${D}${includedir}/trikRuntime/${lib}
        done

        install -d ${D}${libdir}
        cp -rf ${S}/bin/arm-release/lib*.so* ${D}${libdir}

        for lib in $(ls ${D}${libdir}); do
            ln -sf ${libdir}/${lib} ${D}/home/root/trik/${lib}
        done
}


FILES_${PN} += "/home/root/trik/"
#FILES_${PN}-conf += "${sysconfdir}"
#FILES_${PN}-dev += "${includedir}/trikRuntime/*"
#FILES_${PN} += "${libdir}"
FILES_${PN}-dev += "/home/root/trik/*.so"
#FILES_${PN}-dbg += "/home/root/trik/.debug"

pkg_postinst_${PN} () {
        killall trikGui || true
        sleep 3
        killall -9 trikGui 2> /dev/null || true
}