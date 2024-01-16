
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
"

##  file://fstab \
##  file://profile \
##


do_install:append(){

##   install -m 0644 ${WORKDIR}/fstab    ${D}${sysconfdir}
##   install -m 0644 ${WORKDIR}/profile  ${D}${sysconfdir}

}
