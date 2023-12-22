
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
  file://dns.conf \
"

do_install_append(){

   install -m 0644 ${WORKDIR}/dns.conf  ${D}${sysconfdir}/NetworkManager/conf.d/

}
