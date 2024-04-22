SUMMARY = "MOTAB requirements"
DESCRIPTION = "Packages "
PR = "r1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

RDEPENDS:${PN} = " \
  libegl \
  weston-init \
  weston-examples \
  wayland-utils \
  libdrm-tests \
  gtk4 \
  tslib \
  fb-test \
  evtest \
  gdbserver \
"

#
#  gtk+3 \
#  gtk+3-demo \
#
#  kmscube \
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
#  freeglut \
#  glmark2 \
#


RRECOMMENDS:${PN} = "\
"

