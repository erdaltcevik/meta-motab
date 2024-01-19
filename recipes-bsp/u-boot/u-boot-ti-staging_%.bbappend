FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
  file://0001-append-issi-to-nand_ids.c.patch \
  file://motab_defconfig \
  file://motab-dvl_defconfig \
"




