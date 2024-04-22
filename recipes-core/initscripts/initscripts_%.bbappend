
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
"

##  file://hostname.sh \
##

do_install:append(){

##   install -m 0644 ${WORKDIR}/hostname.sh    ${D}${sysconfdir}/init.d/

##   rm ${D}${sysconfdir}/rcS.d/S02banner.sh
##   rm ${D}${sysconfdir}/rcS.d/S38dmesg.sh

##   rm ${D}${sysconfdir}/rc5.d/S15mountnfs.sh

}
