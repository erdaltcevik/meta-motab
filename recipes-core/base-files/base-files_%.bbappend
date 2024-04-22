
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
  file://profile_inclusion \
  file://fstab.in \
"

do_install:append(){

  cat ${WORKDIR}/profile_inclusion >> ${D}${sysconfdir}/profile
  cp  ${WORKDIR}/fstab.in  ${D}${sysconfdir}/fstab

}
