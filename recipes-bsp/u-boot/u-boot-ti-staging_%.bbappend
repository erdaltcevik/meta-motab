FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
  file://00-append-issi-to-nand_ids.c.patch \
  file://00-source-the-board-Kconfig.patch \
  file://00-add-motab-target-config.patch \
  file://00-append-issi-to-rawnand-h.patch \
  file://00-remove-debug-uart-init-call-from-crt0-S.patch \
  file://motab_defconfig \
  file://motab-dvl_defconfig \
  file://motab.dts \
  file://motab-u-boot.dtsi \
  file://motab-dvl.dts \
  file://motab-dvl-u-boot.dtsi \
  file://motab.h \
  file://board_Kconfig \
  file://board_Makefile \
  file://board.c \
  file://u-boot.lds \
"

do_configure:prepend() {

  cp ${WORKDIR}/motab_defconfig        ${S}/configs
  cp ${WORKDIR}/motab-dvl_defconfig    ${S}/configs

  cp ${WORKDIR}/motab.dts              ${S}/arch/arm/dts
  cp ${WORKDIR}/motab-u-boot.dtsi      ${S}/arch/arm/dts
  cp ${WORKDIR}/motab-dvl.dts          ${S}/arch/arm/dts
  cp ${WORKDIR}/motab-dvl-u-boot.dtsi  ${S}/arch/arm/dts

  cp ${WORKDIR}/motab.h                ${S}/include/configs
  ln -s  motab.h   ${S}/include/configs/motab-dvl.h

  mkdir -p ${S}/board/brana/motab
  cp ${WORKDIR}/board_Kconfig          ${S}/board/brana/motab/Kconfig
  cp ${WORKDIR}/board_Makefile         ${S}/board/brana/motab/Makefile
  cp ${WORKDIR}/board.c                ${S}/board/brana/motab/board.c
  cp ${WORKDIR}/u-boot.lds             ${S}/board/brana/motab

}



