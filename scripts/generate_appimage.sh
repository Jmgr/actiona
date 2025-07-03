#!/usr/bin/env bash
set -e

APPDIR=AppDir
EXECUTABLE=${APPDIR}/usr/bin/actiona
DESKTOP_FILE=${APPDIR}/usr/share/applications/actiona.desktop
ICON_FILE=${APPDIR}/usr/share/icons/hicolor/48x48/apps/actiona.png
OPENSSL_DIR=${PWD}/openssl/lib64
OPENSSL_LIBSSL=${OPENSSL_DIR}/libssl.so.3
OPENSSL_LIBCRYPTO=${OPENSSL_DIR}/libcrypto.so.3

# Make sure we don't re-use artifacts of a previous run
rm -r ${APPDIR}

cmake --install build --prefix ${APPDIR}/usr

QTROOT=$1
QMAKE=$QTROOT/bin/qmake
LD_LIBRARY_PATH=$QTROOT/lib:${APPDIR}/usr/lib:${OPENSSL_DIR}:$LD_LIBRARY_PATH
PATH=$QTROOT/bin:$PATH
ACTIONA_VERSION=$2

# Hack to prevent this plugin to be included
rm $QTROOT/plugins/sqldrivers/libqsqlmimer.so

# Generate the AppDir first
./linuxdeploy-x86_64.AppImage \
        --appdir        ${APPDIR} \
        --executable    ${EXECUTABLE} \
        --desktop-file  ${DESKTOP_FILE} \
        --icon-file     ${ICON_FILE} \
        --library       ${OPENSSL_LIBSSL} \
        --library       ${OPENSSL_LIBCRYPTO} \
        --plugin        qt

# Remove this plugin so we don't have to bundle QtQuick (we are not using it anywhere else)
rm ${APPDIR}/usr/plugins/multimedia/libffmpegmediaplugin.so
rm ${APPDIR}/usr/lib/libQt6Quick.so.6

# We are not using this plugin
rm ${APPDIR}/usr/plugins/tls/libqcertonlybackend.so

# Generate the AppImage
./linuxdeploy-x86_64.AppImage \
        --appdir        ${APPDIR} \
        --executable    ${EXECUTABLE} \
        --desktop-file  ${DESKTOP_FILE} \
        --icon-file     ${ICON_FILE} \
        --library       ${OPENSSL_LIBSSL} \
        --library       ${OPENSSL_LIBCRYPTO} \
        --output        appimage
 
 mv *.AppImage actiona-${ACTIONA_VERSION}-x86_64.AppImage