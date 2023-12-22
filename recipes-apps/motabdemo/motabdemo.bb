DESCRIPTION = "MOTAB HMI Demo Application launcher"
##HOMEPAGE = ""
SECTION = "graphics"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

##DEPENDS = "qtbase qtquick3d"
DEPENDS = " libdrm"
##RDEPENDS_${PN} = "qtquick3d"

##BRANCH = "master"
##SRCREV = "48076d49e65fd86b4ad91384fd8a3b84ed6d6905"

SRC_URI = "file://src \
           file://motabdemo.sh \
"

S = "${WORKDIR}/src"

inherit  update-rc.d

do_install_append () {
    install -d ${D}${bindir}
    install -m 0755 fb-test ${D}${bindir}/motabdemo

    install -d ${D}${sysconfdir}/init.d
    install -m 755 ${WORKDIR}/motabdemo.sh ${D}${sysconfdir}/init.d/motabdemo.sh
}

INITSCRIPT_NAME="motabdemo.sh"
INITSCRIPT_PARAMS = "start 01 S ."

FILES_${PN} += "${sysconfdir}/init.d/motabdemo.sh"

