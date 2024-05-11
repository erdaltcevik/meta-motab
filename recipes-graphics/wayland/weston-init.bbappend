FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

##DEPENDS:remove = "docker-ce"

SRC_URI:append = " \
"
#
#    file://weston.ini \
#    file://weston.service \
#

do_install:append() {

}

# use "mkpasswd -m MD5 weston" to get the password hash
# escape the '$' chars inside the hash
PASSWD = "\$1\$/7GQMJwI\$hhUDyjzgZOdJkwQRnEU9T0"
USERADD_PARAM:${PN} =+ " -p '${PASSWD}' "


