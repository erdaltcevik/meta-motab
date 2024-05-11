
# Look in the generic major.minor directory for files
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "  \
  file://motab.dts \
  file://motab-dvl.cfg \            
  "
#
#  file://am33xx.dtsi \
#  file://am33xx-l4.dtsi \
#  file://am33xx-clocks.dtsi \
#


do_configure:append () {

#  cp ${WORKDIR}/am33xx.dtsi        ${S}/arch/arm/boot/dts
#  cp ${WORKDIR}/am33xx-l4.dtsi     ${S}/arch/arm/boot/dts
#  cp ${WORKDIR}/am33xx-clocks.dtsi ${S}/arch/arm/boot/dts
  cp ${WORKDIR}/motab.dts          ${S}/arch/arm/boot/dts
#  cp ${WORKDIR}/motab.dts          ${S}/arch/arm/boot/dts/ti/omap

  echo 'dtb-$(CONFIG_SOC_AM33XX) += motab.dtb' >> ${S}/arch/arm/boot/dts/Makefile
#  echo 'dtb-$(CONFIG_SOC_AM33XX) += motab.dtb' >> ${S}/arch/arm/boot/dts/ti/omap/Makefile 

}

KERNEL_CONFIG_FRAGMENTS += "${WORKDIR}/motab-dvl.cfg"
