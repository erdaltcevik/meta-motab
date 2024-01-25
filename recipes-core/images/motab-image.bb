SUMMARY = "MOTAB Release filesystem image"

DESCRIPTION = "Complete filesystem image for MOTAB"

require recipes-core/images/arago-image.inc

IMAGE_FEATURES:remove = " package-management splash "

IMAGE_INSTALL += " \
  packagegroup-motab \
"







export IMAGE_BASENAME = "motab-image"

