SUMMARY = "MOTAB Development filesystem image"
DESCRIPTION = "Development filesystem image for MOTAB"
LICENSE = "MIT"
export IMAGE_BASENAME = "motab-dvl-image"

inherit core-image


IMAGE_INSTALL += " \
  packagegroup-motab \
  "


BAD_RECOMMENDATIONS:pn-${PN} += " \
  udev-hwdb \
  e2fsprogs-e2fsck \
  adwaita-icon-theme-symbolic \
  ifupdown \
  init-ifupdown \
  "
#
#
#


##inherit extrausers
##PASSWD = "\$5\$OOS4Be1emX2Yq8l6\$zahpw8ygr3q/0xcTUxcw4/hhmqIinjg7DugBV0BNUk6"
##EXTRA_USERS_PARAMS = " \
##  useradd -p '${PASSWD}' erdal \
##  "
##


