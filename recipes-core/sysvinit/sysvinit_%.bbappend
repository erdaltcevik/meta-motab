FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
  file://rcS-default \
"

do_install_append(){

   install -m 0644 ${WORKDIR}/rcS-default  ${D}${sysconfdir}/default/rcS

   rm ${D}${sysconfdir}/rcS.d/S07bootlogd

   rm ${D}${sysconfdir}/rc5.d/S99stop-bootlogd

}
