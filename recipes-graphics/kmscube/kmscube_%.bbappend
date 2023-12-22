
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
   file://kmscube.sh \
"

# 
#

do_install_append(){
	install -d ${D}${sysconfdir}/init.d
  install -m 0644 ${WORKDIR}/kmscube.sh  ${D}${sysconfdir}/init.d/
}

inherit update-rc.d

INITSCRIPT_NAME = "kmscube.sh"
INITSCRIPT_PARAMS = "defaults 99"
