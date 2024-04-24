SECTION = "kernel"
SUMMARY = "Linux kernel for TI devices"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

inherit kernel

require recipes-kernel/linux/setup-defconfig.inc

DEPENDS += "gmp-native libmpc-native"

# Look in the generic major.minor directory for files
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}-6.1:"

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT} \
		      ${EXTRA_DTC_ARGS}"

S = "${WORKDIR}/git"

BRANCH ?= "ti-linux-6.1.y"

SRCREV ?= "2e423244f8c09173a344e7069f0fe2bdf26cccee"
PV = "6.1.80+git${SRCPV}"

# Append to the MACHINE_KERNEL_PR so that a new SRCREV will cause a rebuild
MACHINE_KERNEL_PR:append = "b"
PR = "${MACHINE_KERNEL_PR}"

KERNEL_GIT_URI ?= "git://git.ti.com/git/ti-linux-kernel/ti-linux-kernel.git"
KERNEL_GIT_PROTOCOL = "https"
SRC_URI += "${KERNEL_GIT_URI};protocol=${KERNEL_GIT_PROTOCOL};branch=${BRANCH} \
            file://defconfig
            file://motab.dts \
            file://motab-dvl.cfg \            
            "
#
#  file://am33xx.dtsi \
#  file://am33xx-l4.dtsi \
#  file://am33xx-clocks.dtsi \
#


# Special configuration for remoteproc/rpmsg IPC modules
module_conf_rpmsg_client_sample = "blacklist rpmsg_client_sample"
module_conf_ti_k3_r5_remoteproc = "softdep ti_k3_r5_remoteproc pre: virtio_rpmsg_bus"
module_conf_ti_k3_dsp_remoteproc = "softdep ti_k3_dsp_remoteproc pre: virtio_rpmsg_bus"
KERNEL_MODULE_PROBECONF += "rpmsg_client_sample ti_k3_r5_remoteproc ti_k3_dsp_remoteproc"


do_configure:append () {

#  cp ${WORKDIR}/am33xx.dtsi        ${S}/arch/arm/boot/dts
#  cp ${WORKDIR}/am33xx-l4.dtsi     ${S}/arch/arm/boot/dts
#  cp ${WORKDIR}/am33xx-clocks.dtsi ${S}/arch/arm/boot/dts
  cp ${WORKDIR}/motab.dts          ${S}/arch/arm/boot/dts

  echo 'dtb-$(CONFIG_SOC_AM33XX) += motab.dtb' >> ${S}/arch/arm/boot/dts/Makefile

}

KERNEL_CONFIG_FRAGMENTS += "${WORKDIR}/motab-dvl.cfg"
