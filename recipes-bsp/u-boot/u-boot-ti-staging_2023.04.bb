SUMMARY = "u-boot bootloader for motab"

require ${COREBASE}/meta/recipes-bsp/u-boot/u-boot-common.inc
require ${COREBASE}/meta/recipes-bsp/u-boot/u-boot.inc

BRANCH ?= "master"
UBOOT_GIT_URI ?= "git://git.ti.com/git/ti-u-boot/ti-u-boot.git"
UBOOT_GIT_PROTOCOL = "https"
SRC_URI = "${UBOOT_GIT_URI};protocol=${UBOOT_GIT_PROTOCOL};branch=${BRANCH}"

# u-boot needs devtree compiler to parse dts files
DEPENDS += "dtc-native bc-native lzop-native flex-native bison-native python3-setuptools-native"

# u-boot needs these for binman
DEPENDS += "python3-pyelftools-native python3-pyyaml-native python3-jsonschema-native"

COMPATIBLE_MACHINE = "(ti-soc)"

BINMAN_INDIRS="${STAGING_DIR_HOST}${nonarch_base_libdir}/firmware"

EXTRA_OEMAKE += "${PACKAGECONFIG_CONFARGS} BINMAN_INDIRS=${BINMAN_INDIRS}"

PROVIDES += "u-boot"
PKG:${PN} = "u-boot"
PKG:${PN}-dev = "u-boot-dev"
PKG:${PN}-dbg = "u-boot-dbg"

S = "${WORKDIR}/git"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}-2023.04:"


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
#
#  file://am33xx.dtsi \
#  file://am33xx-l4.dtsi \
#  file://am33xx-clocks.dtsi \
#


do_configure:prepend() {

  cp ${WORKDIR}/motab_defconfig        ${S}/configs
  cp ${WORKDIR}/motab-dvl_defconfig    ${S}/configs

#  cp ${WORKDIR}/am33xx.dtsi        ${S}/arch/arm/dts
#  cp ${WORKDIR}/am33xx-l4.dtsi     ${S}/arch/arm/dts
#  cp ${WORKDIR}/am33xx-clocks.dtsi ${S}/arch/arm/dts

  cp ${WORKDIR}/motab.dts              ${S}/arch/arm/dts
  cp ${WORKDIR}/motab-u-boot.dtsi      ${S}/arch/arm/dts
  cp ${WORKDIR}/motab-dvl.dts          ${S}/arch/arm/dts
  cp ${WORKDIR}/motab-dvl-u-boot.dtsi  ${S}/arch/arm/dts

  cp ${WORKDIR}/motab.h                ${S}/include/configs
  ln -sf  motab.h   ${S}/include/configs/motab-dvl.h

  mkdir -p ${S}/board/brana/motab
  cp ${WORKDIR}/board_Kconfig          ${S}/board/brana/motab/Kconfig
  cp ${WORKDIR}/board_Makefile         ${S}/board/brana/motab/Makefile
  cp ${WORKDIR}/board.c                ${S}/board/brana/motab/board.c
  cp ${WORKDIR}/u-boot.lds             ${S}/board/brana/motab

}



