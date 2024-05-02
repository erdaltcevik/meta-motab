DISTRO_VERSION = "2024.04"

INIT_MANAGER = "systemd"
##INIT_MANAGER = "sysvinit"
##INIT_MANAGER = "mdev-busybox"

##PREFERRED_PROVIDER_udev = ""
##PACKAGECONFIG:remove:pn-ti-sgx-ddk-um = "udev"

OPKG_ARGS:remove = "--prefer-arch-to-version"

DISTRO_FEATURES_DEFAULT:remove = " \
 acl \
 alsa \
 argp \
 bluetooth \
 debuginfod \
 ext2 \
 largefile \
 pcmcia \
 usbgadget \
 usbhost \
 wifi \
 xattr \
 nfs \
 zeroconf \
 pci \
 3g \
 nfc \
 x11 \
 vfat \
 seccomp \
"

DISTRO_FEATURES_BACKFILL_CONSIDERED = " \
 pulseaudio \
"
DISTRO_FEATURES:append = " pam wayland opengl "
##DISTRO_FEATURES:append = " wayland opengl "

DEFAULT_TIMEZONE = "Europe/Istanbul"

USE_VT = "0"

PACKAGECONFIG:remove:pn-gdb = "python"
PACKAGECONFIG:remove:pn-opkg-utils = "python"

PACKAGECONFIG:remove:pn-systemd = " \
  gshadow \
  hostnamed \
  idn \
  ima \
  kmod \
  localed \
  machined \
  myhostname \
  networkd \
  nss \
  nss-mymachines \
  nss-resolve \
  quotacheck \
  randomseed \
  resolved \
  set-time-epoch \
  sysusers \
  sysvinit \
  timedated \
  timesyncd \
  userdb \
  utmp \
  vconsole \
  wheel-group \
  backlight \
  binfmt \
  zstd \
  hibernate \
  "
#
#  logind \
#

SYSTEMD_DEFAULT_TARGET = "graphical.target"


##ENABLE_BINARY_LOCALE_GENERATION = "0"
##GLIBC_GENERATE_LOCALES = "en_US.UTF-8 tr_TR.UTF-8"
##IMAGE_LINGUAS = "en-us tr-tr"
##IMAGE_LINGUAS:remove = " en-us "

PACKAGECONFIG:remove:pn-libdrm = " \
 intel \
 radeon \
 amdgpu \
 nouveau \
 vmwgfx \
 freedreno \
 vc4 \
 etnaviv \
"
##hostname:pn-base-files = "the-hostname"

SIMPLECLIENTS = "egl"

PACKAGECONFIG:remove:pn-weston = " \
 shell-ivi \
 shell-kiosk \
 image-jpeg \
"
##
## wayland \
## shell-desktop \
##
##

INHERIT_DISTRO:remove = "create-spdx"


