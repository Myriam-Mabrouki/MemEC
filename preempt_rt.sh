#! /bin/bash

# Author: Myriam Mabrouki

# This file enables patches the PREEMPT-RT patch to your device.
# Feel free to change versions.

VERSION=v6.x
RT_VERSION=6.12
LINUX_VERSION=linux-6.12.25
PATCH_VERSION=patch-6.12.39-rt11

cd
sudo apt install build-essential bc kmod cpio flex cpio libncurses5-dev bison libssl-dev wget
wget https://www.kernel.org/pub/linux/kernel/${VERSION}/${LINUX_VERSION}.tar.xz
#wget https://www.kernel.org/pub/linux/kernel/projects/rt/${RT_VERSION}/${PATCH_VERSION}.patch.xz
xz -cd ${LINUX_VERSION}.tar.xz | tar xvf -
cd ${LINUX_VERSION}
#xzcat ../${PATCH_VERSION}.patch.xz | patch -p1
