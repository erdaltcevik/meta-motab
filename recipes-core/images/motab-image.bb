
inherit core-image

export IMAGE_BASENAME = "motab-image"

COMPATIBLE_MACHINE = "ti-soc"

IMAGE_INSTALL += " \
 packagegroup-motab \
 libdrm-tests \
"

BAD_RECOMMEMDATIONS = ""
