DISTRO_VERSION = "2024.01"
DISTRO_FEED_URI ?= "http://lcpd.itg.ti.com"
BUILD_VARIANT = "${@['prod','rt','systest'][int(d.getVar('ARAGO_RT_ENABLE'))+int(d.getVar('ARAGO_SYSTEST_ENABLE'))*2]}"
TISDK_VERSION ?= "live"
FEEDURIPREFIX ?= "builds/${DISTRO}-kirkstone/${BUILD_VARIANT}/${MACHINE}/latest/packages/"

OPKG_ARGS:remove = "--prefer-arch-to-version"

TI_MIRROR = "http://software-dl.ti.com/processor-sdk-mirror/sources/"
MIRRORS += " \
bzr://.*/.*      ${TI_MIRROR} \n \
cvs://.*/.*      ${TI_MIRROR} \n \
git://.*/.*      ${TI_MIRROR} \n \
gitsm://.*/.*    ${TI_MIRROR} \n \
hg://.*/.*       ${TI_MIRROR} \n \
osc://.*/.*      ${TI_MIRROR} \n \
p4://.*/.*       ${TI_MIRROR} \n \
npm://.*/.*      ${TI_MIRROR} \n \
ftp://.*/.*      ${TI_MIRROR} \n \
https?$://.*/.*  ${TI_MIRROR} \n \
svn://.*/.*      ${TI_MIRROR} \n \
"

# SDK paths and names
SDK_NAME_PREFIX = "arago"
SDK_VERSION = "${DISTRO_VERSION}"
SDK_VENDOR = "-${SDK_NAME_PREFIX}"
SDK_NAME = "${SDK_NAME_PREFIX}-${SDK_VERSION}"
SDKPATHINSTALL = "/opt/${SDK_NAME}"

##IMAGE_FSTYPES += "tar.xz.md5sum"

# Extra boot files for WIC images
##do_image_wic[depends] += "tisdk-uenv:do_deploy"
##IMAGE_BOOT_FILES += "uEnv.txt"

# Mask any broken recipes (currently none)
#BBMASK = ""

TCMODEAPPEND ?= "-${TCMODE}"
TMPDIR .= "${TCMODEAPPEND}"

# Start with default set of distro features
##DISTRO_FEATURES = "${DISTRO_FEATURES_DEFAULT} ${DISTRO_FEATURES_LIBC}"

# Add additional distro features
##DISTRO_FEATURES:append = " pam wayland opengl"
#DISTRO_FEATURES:append = " opencl"
##DISTRO_FEATURES:append = " vulkan"
##DISTRO_FEATURES:append = " opencv"
##DISTRO_FEATURES:append = " openmp"

# Normally target distro features will not be applied to native builds:
# Native distro features on this list will use the target feature value
##DISTRO_FEATURES_FILTER_NATIVE:append = " opencl opencv openmp"
##DISTRO_FEATURES_FILTER_NATIVESDK:append = " opencl opencv openmp"

# Set global runtime providers for major components
ARAGO_SYSVINIT ??= "0"
INIT_MANAGER ?= "${@['systemd','sysvinit'][int(d.getVar('ARAGO_SYSVINIT'))]}"

# Distro-specific package configuration
PACKAGECONFIG:append:pn-systemd = " coredump networkd resolved serial-getty-generator"

# Configure Qt5 flags (GLES is still required for qtdeclarative and qtwebkit)
PACKAGECONFIG_GL:pn-qtbase = "gles2 linuxfb"
PACKAGECONFIG_DISTRO:pn-qtbase = "icu examples accessibility gif gbm kms libinput"
PACKAGECONFIG_FONTS:pn-qtbase = "fontconfig"
PACKAGECONFIG:remove:pn-qtwayland = "xcomposite-egl xcomposite-glx"

# Add gstreamer in package configuration to enable video
PACKAGECONFIG:append:pn-qtmultimedia = " gstreamer"

# Add examples in Qt5 package configurations
PACKAGECONFIG:append:pn-qtcharts = " examples"
#PACKAGECONFIG:append:pn-qtdeclarative = " examples"
PACKAGECONFIG:append:pn-qtlocation = " examples"
PACKAGECONFIG:append:pn-qtmultimedia = " examples"
PACKAGECONFIG:append:pn-qtscript = " examples"
PACKAGECONFIG:append:pn-qtserialport = " examples"
PACKAGECONFIG:append:pn-qtsvg = " examples"
PACKAGECONFIG:append:pn-qtwayland = " examples"
PACKAGECONFIG:append:pn-qtwebengine = " examples"

# Prefer OpenSSL over GnuTLS for curl due to TSU exemptions
PACKAGECONFIG:pn-curl = "${@bb.utils.contains("DISTRO_FEATURES", "ipv6", "ipv6", "", d)} proxy openssl verbose zlib"

PACKAGECONFIG:pn-glmark2 = "drm-gles2 wayland-gles2"

# Set this to empty string to prevent xwayland config from being added
PACKAGECONFIG:pn-weston-init = ""

# Enable testing for opencl and vulkan with piglit
PACKAGECONFIG:append:pn-piglit = " \
    ${@bb.utils.filter("DISTRO_FEATURES", "opencl vulkan", d)} \
    ${@bb.utils.contains("DISTRO_FEATURES", "opengl", "deqp-gles", "", d)} \
    ${@bb.utils.contains("DISTRO_FEATURES", "vulkan", "deqp-vk", "", d)} \
"
PACKAGECONFIG:pn-opengl-es-cts = "${@bb.utils.contains("DISTRO_FEATURES", "wayland", "wayland", "surfaceless", d)}"
PACKAGECONFIG:pn-vulkan-cts = "${@bb.utils.contains("DISTRO_FEATURES", "wayland", "wayland", "surfaceless", d)}"

# Use llvmpipe for better performance with software rendering
# Only available for armv6+ devices
#MESA_GALLIUM_LLVM_PACKAGECONFIG = "gallium-llvm"
#MESA_GALLIUM_LLVM_PACKAGECONFIG:omapl138 = ""
#PACKAGECONFIG:append:pn-mesa = " ${MESA_GALLIUM_LLVM_PACKAGECONFIG}"
#PACKAGECONFIG:append:pn-mesa-pvr = " ${MESA_GALLIUM_LLVM_PACKAGECONFIG}"

# Enable encryption support in open62541
PACKAGECONFIG:append:pn-open62541 = " encrypt"

# Enable netperf intervals/pacing and histogram
PACKAGECONFIG:append:pn-netperf = " intervals histogram"

# Use OpenGL ES2 for SDL2
PACKAGECONFIG_GL:pn-libsdl2 = "gles2"

# Add KMS/DRM backend support
PACKAGECONFIG:append:pn-libsdl2 = " kmsdrm"

# Enable cryptodev/devcrypto engine in OpenSSL 1.1
PACKAGECONFIG:append:pn-openssl = " cryptodev-linux"

# Enable GST_TRACER logging in gstreamer
PACKAGECONFIG:append:pn-gstreamer1.0 = " tracer-hooks coretracers"

# Enable kmssink in gstreamer
PACKAGECONFIG:append:pn-gstreamer1.0-plugins-bad = " kms"

# Enable gstreamer plugin for libcamera
PACKAGECONFIG:append:pn-libcamera = " gst"

# Disable pulseaudio routing in alsa by default
PACKAGECONFIG:remove:pn-alsa-plugins = "pulseaudio"
RDEPENDS:pulseaudio-server:remove = "alsa-plugins-pulseaudio-conf"

# Configure RAMFS for jailhouse linux-demo
JH_RAMFS_IMAGE = "tisdk-tiny-initramfs"

# Enable UI and scripting for perf
PERF_FEATURES_ENABLE ?= "perf-scripting perf-tui"

# Do not warn on meta-gnome w/o x11
SKIP_META_GNOME_SANITY_CHECK = "1"

ARAGO_KERNEL_SUFFIX ?= ""

# Set real-time kernel suffix
ARAGO_RT_ENABLE ?= "0"
ARAGO_KERNEL_SUFFIX:append = "${@oe.utils.conditional("ARAGO_RT_ENABLE", "1", "-rt", "", d)}"

# Set systest kernel suffix
ARAGO_SYSTEST_ENABLE ?= "0"
ARAGO_KERNEL_SUFFIX:append = "${@oe.utils.conditional("ARAGO_SYSTEST_ENABLE", "1", "-systest", "", d)}"
IMAGE_FSTYPES:remove = "${@['','ubi ubifs'][d.getVar('ARAGO_SYSTEST_ENABLE') == '1']}"

# Enable basic stack and buffer overflow protections
#TARGET_CPPFLAGS += "-fstack-protector -D_FORTIFY_SOURCE=1"
#TARGET_CPPFLAGS += "-fstack-protector"

# Load default preferences
require conf/distro/include/arago-prefs.inc

# Allow branding on top of Arago Distro and Core TI-SDK
# If ARAGO_BRAND is not set in local.conf, default to core
ARAGO_BRAND ??= "core"

## Already included in meta-edgeai/con/layer.conf
## Comment out here to get rid of duplicate inclusion warnings
##require conf/distro/include/branding-${ARAGO_BRAND}.inc

# Disable polkit's systemd configuration from meta-oe
POLKITAUTH = ""

# Disable meta-virtualization sanity check and warning
SKIP_META_VIRT_SANITY_CHECK = "1"

# Set ENTERPRISE_DISTRO flag to ensure that recipes that have been flagged
# do not attempt to enable features that have potential commercial
# licensing issues.
ENTERPRISE_DISTRO = "1"

# oe-core uses external libav instead of internal libav for gst-ffmpeg. We
# disable by default the codecs in libav that causes issues with shipping an
# Arago based SDK. No need to prevent gst-ffmpeg from being used since we
# disable troublesome software.
LICENSE_FLAGS_ACCEPTED = "commercial_gst-ffmpeg commercial_gstreamer1.0-libav commercial_faad2 non-commercial_netperf"

# Do not package own copy of perl into devkit, rely on host one
ASSUME_PROVIDED += "nativesdk-perl"

###--------------------------------------------------------------------------------------

DISTRO_FEATURES:remove = " \
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
"

DISTRO_FEATURES_BACKFILL_CONSIDERED = " \
 pulseaudio \
"
DISTRO_FEATURES:append = " pam wayland opengl "

DEFAULT_TIMEZONE = "Europe/Istanbul"

USE_VT = "0"

CREATE_SRCIPK:pn-linux-ti-staging = "0"
CREATE_SRCIPK:pn-u-boot-ti-staging = "0"

PACKAGECONFIG:remove:pn-gdb = "python"
PACKAGECONFIG:remove:pn-opkg-utils = "python"

PACKAGECONFIG:remove:pn-systemd = " backlight binfmt zstd hibernate "

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




