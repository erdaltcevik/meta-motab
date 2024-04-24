SUMMARY = "MOTAB Release filesystem image"
DESCRIPTION = "Complete filesystem image for MOTAB"
LICENSE = "MIT"
export IMAGE_BASENAME = "motab-image"

##require recipes-core/images/arago-image.inc

IMAGE_FEATURES:remove = " package-management splash "

IMAGE_INSTALL += " \
  packagegroup-motab \
"









