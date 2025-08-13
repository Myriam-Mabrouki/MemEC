#! /bin/bash

# Author: Myriam Mabrouki

VERSION=v6.x
RT_VERSION=6.9
LINUX_VERSION=linux-6.9.9
PATCH_VERSION=patch-6.9-rt5

cd
sudo apt install build-essential bc kmod cpio flex cpio libncurses5-dev bison libssl-dev wget
wget https://www.kernel.org/pub/linux/kernel/${VERSION}/${LINUX_VERSION}.tar.xz
wget https://www.kernel.org/pub/linux/kernel/projects/rt/${RT_VERSION}/${PATCH_VERSION}.patch.xz
xz -cd ${LINUX_VERSION}.tar.xz | tar xvf -
cd ${LINUX_VERSION}
xzcat ../${PATCH_VERSION}.patch.xz | patch -p1
