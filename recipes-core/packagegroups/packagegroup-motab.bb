SUMMARY = "MOTAB requirements"
DESCRIPTION = "Packages "
PR = "r1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

RDEPENDS:${PN} = " \
  weston-init \
  wayland-utils \
  openssh \
  gdbserver \
"
#
#  gtk4 \
#   libegl \
#  weston-examples \
#
#  libdrm-tests \
#  
#  tslib \
#  evtest \
#  
#  fb-test \
#  gtk+3 \
#  gtk+3-demo \
#
#  kmscube \
#  gobject-introspection \
#  ti-sgx-ddk-um \
#  dbus \
#  networkmanager \
#  
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

