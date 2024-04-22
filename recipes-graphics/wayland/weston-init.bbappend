FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

DEPENDS:remove = "docker-ce"

SRC_URI:append = " \
    file://weston.ini \
    file://weston.service \
"

do_install:append() {

}


