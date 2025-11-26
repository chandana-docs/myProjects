#!/bin/bash

TARGETSYSROOT=/home/targetfs/
CROSS_COMPILE=aarch64-linux-gnu-
BUILD_DIR=$(pwd)

export INSTALL_DIR=$BUILD_DIR/INSTALL

QT_SRC=$(pwd)

./configure \
-opengl es2 \
-device am62x-aarch64-gnu-g++ \
-device-option CROSS_COMPILE=$CROSS_COMPILE \
-qpa eglfs \
-opensource \
-confirm-license \
-eglfs \
-kms \
-gbm \
-no-feature-vnc \
-no-linuxfb \
-no-xcb \
-no-pch \
-no-use-gold-linker \
-no-feature-relocatable \
-no-openssl \
-no-strip \
-no-gif \
-no-harfbuzz \
-no-glib \
-no-cups \
-no-xcb-xlib \
-no-sql-sqlite \
-make examples \
-nomake tests \
-sysroot $TARGETSYSROOT \
-gcc-sysroot \
-skip qtquick3d \
-skip qt3d \
-skip qtlocation \
-skip qtwebengine \
-skip qtactiveqt \
-skip qtcanvas3d \
-skip qtconnectivity \
-skip qtdoc \
-skip qtimageformats \
-skip qtmacextras \
-skip qtmultimedia \
-skip qtscript \
-skip qtsensors \
-skip qtserialbus \
-skip qttranslations \
-skip qtwebchannel \
-skip qtwebview \
-skip qtwinextras \
-skip qtx11extras \
-skip qtdatavis3d \
-skip qtgamepad \
-skip qtpurchasing \
-skip qtscxml \
-skip qtvirtualkeyboard \
-skip qtdocgallery \
-skip qtfeedback \
-skip qtnetworkauth \
-skip qtpim \
-skip qtqa \
-skip qtrepotools \
-skip qtspeech \
-skip qtsystems \
-skip qtandroidextras \
-skip qtwayland \
-skip qtcharts \
-skip qtserialport \
-skip qtxmlpatterns \
-skip qttools \
-skip qtdeclarative \
-skip qtlottie \
-skip qtquickcontrols \
-skip qtquickcontrols2 \
-skip qtquicktimeline \
-skip qtremoteobjects \
-skip qtsvg \
-skip qtwebchannel \
-skip qtwebglplugin \
-skip qtwebview \
-skip qtwebsockets \
-no-tslib \
-rpath \
-prefix /opt/qt5u15 \
-extprefix $INSTALL_DIR/target/opt/qt5u15 \
-hostprefix $INSTALL_DIR/HOST \
