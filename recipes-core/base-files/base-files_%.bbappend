
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
  file://fstab \
  file://profile \
"

do_install_append(){

   install -m 0644 ${WORKDIR}/fstab    ${D}${sysconfdir}
   install -m 0644 ${WORKDIR}/profile  ${D}${sysconfdir}

}
