#! /bin/bash

# Author: Myriam Mabrouki

cd
wget https://www.kernel.org/pub/linux/kernel/v6.x/linux-6.9.9.tar.xz
wget https://www.kernel.org/pub/linux/kernel/projects/rt/6.9/patch-6.9-rt5.patch.xz
xz -cd linux-6.9.9.tar.xz | tar xvf -
cd linux-6.9.9
xzcat ../patch-6.9.9-rt5.patch.xz | patch -p1
