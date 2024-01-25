SUMMARY = "MOTAB requirements"
DESCRIPTION = "Packages "
PR = "r1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

RDEPENDS:${PN} = " \
  libegl \
  kmscube \
  fb-test \
  evtest \
"


#  gobject-introspection \
#  ti-sgx-ddk-um \
#  dbus \
#  networkmanager \
#  openssh \
#  openssh-sftp-server \
#  tzdata \
#  ntp \
#  psplash \
#  motabdemo \
#  glmark2 \
#  gtk+3 \
#

RRECOMMENDS:${PN} = "\
"
