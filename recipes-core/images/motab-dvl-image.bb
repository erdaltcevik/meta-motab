SUMMARY = "MOTAB Development filesystem image"

DESCRIPTION = "Complete filesystem image for MOTAB"

require recipes-core/images/arago-image.inc

IMAGE_FEATURES:remove = " package-management splash "

IMAGE_FEATURES += " ssh-server-openssh "
##IMAGE_FEATURES:append = " tools-debug "

IMAGE_INSTALL += " \
  packagegroup-motab \
  "



BAD_RECOMMENDATIONS:pn-${PN} += " \
  udev-hwdb \
  e2fsprogs-e2fsck \
  adwaita-icon-theme-symbolic \
  "

##inherit extrausers
##PASSWD = "\$5\$OOS4Be1emX2Yq8l6\$zahpw8ygr3q/0xcTUxcw4/hhmqIinjg7DugBV0BNUk6"
##EXTRA_USERS_PARAMS = " \
##  useradd -p '${PASSWD}' erdal \
##  "
##



export IMAGE_BASENAME = "motab-dvl-image"

