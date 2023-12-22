
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
  file://motab-dvl.cfg \
"

##
##

do_install_append(){

##   install -m 0644 ${WORKDIR}/fstab ${D}${sysconfdir}

}
