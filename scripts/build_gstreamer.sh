#!/bin/bash -v         

# Create a log file of the build as well as displaying the build on the tty as it runs
exec > >(tee build_gstreamer.log)
exec 2>&1

################# COMPILE GSTREAMER 1.x ############



sudo apt-get install -y build-essential autotools-dev automake autoconf \
                        libtool autopoint libxml2-dev zlib1g-dev libglib2.0-dev \
                        pkg-config bison flex python git gtk-doc-tools libasound2-dev \
                        libgudev-1.0-dev libxt-dev libvorbis-dev libcdparanoia-dev \
                        libpango1.0-dev libtheora-dev libvisual-0.4-dev iso-codes \
                        libgtk-3-dev libraw1394-dev libiec61883-dev libavc1394-dev \
                        libv4l-dev libcairo2-dev libcaca-dev libspeex-dev libpng-dev \
                        libshout3-dev libjpeg-dev libaa1-dev libflac-dev libdv4-dev \
                        libtag1-dev libwavpack-dev libpulse-dev libsoup2.4-dev libbz2-dev \
                        libcdaudio-dev libdc1394-22-dev ladspa-sdk libass-dev \
                        libcurl4-gnutls-dev libdca-dev libdirac-dev libdvdnav-dev \
                        libexempi-dev libexif-dev libfaad-dev libgme-dev libgsm1-dev \
                        libiptcdata0-dev libkate-dev libmimic-dev libmms-dev \
                        libmodplug-dev libmpcdec-dev libofa0-dev libopus-dev \
                        librsvg2-dev librtmp-dev libschroedinger-dev libslv2-dev \
                        libsndfile1-dev libsoundtouch-dev libspandsp-dev libx11-dev \
                        libxvidcore-dev libzbar-dev libzvbi-dev liba52-0.7.4-dev \
                        libcdio-dev libdvdread-dev libmad0-dev libmp3lame-dev \
                        libmpeg2-4-dev libopencore-amrnb-dev libopencore-amrwb-dev \
                        libsidplay1-dev libtwolame-dev libx264-dev libusb-1.0

cd $HOME
mkdir packages
cd packages/ 
mkdir gstreamer-1.x
cd gstreamer-1.x	

export LD_LIBRARY_PATH=/usr/local/lib/ path

git clone -n git://anongit.freedesktop.org/git/gstreamer/gstreamer
cd gstreamer
git checkout -t origin/1.0
./autogen.sh 
make
sudo make install
cd ..


git clone -n git://anongit.freedesktop.org/git/gstreamer/gst-plugins-base
cd gst-plugins-base
git checkout -t origin/1.0
./autogen.sh
make
sudo make install
cd ..


git clone -n git://anongit.freedesktop.org/git/gstreamer/gst-plugins-good
cd gst-plugins-good
git checkout -t origin/1.0
./autogen.sh 
make
sudo make install
cd ..


git clone -n git://anongit.freedesktop.org/git/gstreamer/gst-plugins-ugly
cd gst-plugins-ugly
git checkout -t origin/1.0
./autogen.sh 
make
sudo make install
cd ..


git clone -n git://anongit.freedesktop.org/git/gstreamer/gst-libav
cd gst-libav
git checkout -t origin/1.0
./autogen.sh
make
sudo make install
cd ..


git clone -n git://anongit.freedesktop.org/git/gstreamer/gst-plugins-bad
cd gst-plugins-bad
git checkout -t origin/1.0
LDFLAGS='-L/opt/vc/lib' CPPFLAGS='-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux' ./autogen.sh 
make CFLAGS+="-Wno-error"
sudo make install
cd ..

################# COMPILE GST-OMX ##########


git clone -n git://anongit.freedesktop.org/git/gstreamer/gst-omx
cd gst-omx
git checkout -t origin/1.0
LDFLAGS='-L/opt/vc/lib' CPPFLAGS='-I/opt/vc/include -I/opt/vc/include/IL -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux' ./autogen.sh --with-omx-target=rpi
make CFLAGS+="-Wno-error"
sudo make install

