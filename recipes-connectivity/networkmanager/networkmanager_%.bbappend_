
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
  file://dns.conf \
"

do_install:append(){

   install -m 0644 ${WORKDIR}/dns.conf  ${D}${sysconfdir}/NetworkManager/conf.d/

}
