
FILESEXTRAPATHS:prepend := "${THISDIR}/ntp:"

SRC_URI:append = " \
"

##  file://ntp.conf \
##

do_install:append(){
##  install -m 0644 ${WORKDIR}/ntp.conf  ${D}${sysconfdir}/
}
