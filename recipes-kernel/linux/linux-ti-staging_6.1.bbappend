
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
  file://am33xx.dtsi \
  file://am33xx-l4.dtsi \
  file://am33xx-clocks.dtsi \
  file://motab.dts \
  file://motab-dvl.cfg \
"

do_configure:append () {

#  cp ${WORKDIR}/am33xx.dtsi        ${S}/arch/arm/boot/dts
#  cp ${WORKDIR}/am33xx-l4.dtsi     ${S}/arch/arm/boot/dts
#  cp ${WORKDIR}/am33xx-clocks.dtsi ${S}/arch/arm/boot/dts
  cp ${WORKDIR}/motab.dts          ${S}/arch/arm/boot/dts

  echo 'dtb-$(CONFIG_SOC_AM33XX) += motab.dtb' >> ${S}/arch/arm/boot/dts/Makefile

}

KERNEL_CONFIG_FRAGMENTS += "${WORKDIR}/motab-dvl.cfg"
