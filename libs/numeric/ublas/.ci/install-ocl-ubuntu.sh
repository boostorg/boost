#!/bin/bash
#this might work on other DEB based distros, YMMV
#prereqs: wget, rpm2cpio, cpio, clinfo (for testing), and no other conflicting OpenCL drivers
#Get this file
#https://software.intel.com/en-us/articles/opencl-drivers#philinux
#http://registrationcenter.intel.com/irc_nas/4181/opencl_runtime_14.2_x64_4.5.0.8.tgz
#or this one http://registrationcenter.intel.com/irc_nas/5193/opencl_runtime_15.1_x64_5.0.0.57.tgz
#wget http://registrationcenter.intel.com/irc_nas/4181/opencl_runtime_14.2_x64_4.5.0.8.tgz
wget http://registrationcenter.intel.com/irc_nas/5193/opencl_runtime_15.1_x64_5.0.0.57.tgz

#unpack the tarball
tar xvf opencl_runtime_15.1_x64_5.0.0.57.tgz

#unpack the rpms
#according to http://mhr3.blogspot.com/2013/06/opencl-on-ubuntu-1304.html, we don't need all of them unpacked
#basically, just the ICD itself
rpm2cpio opencl_runtime_15.1_x64_5.0.0.57/rpm/opencl-1.2-intel-cpu-5.0.0.57-1.x86_64.rpm | cpio -idmv

#stub out the directory structure for the deb package as a staging area
#some of these steps are from here http://mhr3.blogspot.com/2013/06/opencl-on-ubuntu-1304.html
mkdir opencl-driver-intel-cpu
cd opencl-driver-intel-cpu
mkdir DEBIAN
mkdir -p etc/OpenCL/vendors
mkdir -p usr/lib/x86_64-linux-gnu/OpenCL/vendors/intel
mkdir -p usr/share/doc/opencl-driver-intel-cpu
cd ..

#put the right description of the package in the right place
#from the blog post with my modifications
#version number is from the filename
cat <<'EOF' >>  opencl-driver-intel-cpu/DEBIAN/control
Package: opencl-driver-intel-cpu
Version: 5.0.0.57
Section: libs
Priority: optional
Architecture: amd64
Depends: ocl-icd-libopencl1 (>= 2.0), libnuma1
Maintainer: Your Name 
Description: Intel OpenCL CPU implementation
 This package provides Intel OpenCL implementation which can utilize Intel Core processors.
EOF

#copy over the documentation into the deb staging area
cp opt/intel/opencl-1.2-5.0.0.57/doc/* opencl-driver-intel-cpu/usr/share/doc/opencl-driver-intel-cpu/

#the icd registry could be copied from opt/intel/opencl-1.2-5.0.0.57/etc/intel64.icd 
#but it's wrong anyway and we'd have to change it, handle this later

#now put the libs where they go in the staging area
cp opt/intel/opencl-1.2-5.0.0.57/lib64/* opencl-driver-intel-cpu/usr/lib/x86_64-linux-gnu/OpenCL/vendors/intel/

#now deal with that icd registry
cat <<'EOF' >>  opencl-driver-intel-cpu/etc/OpenCL/vendors/intel64.icd
/usr/lib/x86_64-linux-gnu/OpenCL/vendors/intel/libintelocl.so
EOF

#finally build the deb package
dpkg-deb --build opencl-driver-intel-cpu

#and install it
sudo dpkg --install opencl-driver-intel-cpu.deb
sudo apt-get install -f
sudo ldconfig
clinfo #this verifies it installed correctly
#cleanup
rm -rf opencl-driver-intel-cpu opencl_runtime_15.1_x64_5.0.0.57*
