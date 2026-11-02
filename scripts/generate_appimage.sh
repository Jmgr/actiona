#!/usr/bin/env bash
set -e

if [ "$#" -ne 2 ] || [ -z "$1" ] || [ -z "$2" ]; then
        echo "Usage: $0 <Qt installation directory> <Actiona version>" >&2
        exit 1
fi

QTROOT=${1%/}
ACTIONA_VERSION=$2
if [ ! -x "${QTROOT}/bin/qmake" ] || [ ! -d "${QTROOT}/plugins" ]; then
        echo "Invalid Qt installation: $1 (expected bin/qmake and plugins/)" >&2
        exit 1
fi
# Keep the generated qmake wrapper usable with relative input paths too.
QTROOT=$(cd -- "$QTROOT" && pwd)

APPDIR=AppDir
EXECUTABLE=${APPDIR}/usr/bin/actiona
DESKTOP_FILE=${APPDIR}/usr/share/applications/actiona.desktop
ICON_FILE=${APPDIR}/usr/share/icons/hicolor/48x48/apps/actiona.png
OPENSSL_DIR=${PWD}/openssl/lib64
if [ ! -f ${OPENSSL_DIR}/libssl.so.3 ] || [ ! -f ${OPENSSL_DIR}/libcrypto.so.3 ]; then
        OPENSSL_DIR=/usr/lib/x86_64-linux-gnu
fi
OPENSSL_LIBSSL=${OPENSSL_DIR}/libssl.so.3
OPENSSL_LIBCRYPTO=${OPENSSL_DIR}/libcrypto.so.3

# Make sure we don't re-use artifacts of a previous run
rm -rf ${APPDIR}

cmake --build build
cmake --install build --prefix ${APPDIR}/usr

QT_DEPLOY_ROOT=${PWD}/.appimage-qt
QT_DEPLOY_PLUGINS=${QT_DEPLOY_ROOT}/plugins
QT_DEPLOY_BIN=${QT_DEPLOY_ROOT}/bin
QMAKE=${QT_DEPLOY_BIN}/qmake
export LD_LIBRARY_PATH="$QTROOT/lib:${PWD}/${APPDIR}/usr/lib:${PWD}/${APPDIR}/usr/lib/actiona:${PWD}/${APPDIR}/usr/lib/actiona/actions:${OPENSSL_DIR}${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
PATH=${QT_DEPLOY_BIN}:$QTROOT/bin:$PATH

# Present linuxdeploy-plugin-qt with a filtered plugin tree. Some Qt installer
# SQL drivers depend on proprietary/client libraries that are not available on
# the build host, and Actiona does not need them in the AppImage.
rm -rf ${QT_DEPLOY_ROOT}
mkdir -p ${QT_DEPLOY_PLUGINS} ${QT_DEPLOY_BIN}
cp -a ${QTROOT}/plugins/. ${QT_DEPLOY_PLUGINS}/
rm -f ${QT_DEPLOY_PLUGINS}/sqldrivers/libqsqlibase.so
rm -f ${QT_DEPLOY_PLUGINS}/sqldrivers/libqsqlmimer.so
rm -f ${QT_DEPLOY_PLUGINS}/sqldrivers/libqsqloci.so
cat > ${QMAKE} <<EOF
#!/usr/bin/env bash
if [ "\$1" = "-query" ] && [ "\$2" = "QT_INSTALL_PLUGINS" ]; then
        echo "${QT_DEPLOY_PLUGINS}"
        exit 0
fi

if [ "\$1" = "-query" ] && [ "\$#" -eq 1 ]; then
        "${QTROOT}/bin/qmake" "\$@" | sed "s|^QT_INSTALL_PLUGINS:.*|QT_INSTALL_PLUGINS:${QT_DEPLOY_PLUGINS}|"
        exit 0
fi

exec "${QTROOT}/bin/qmake" "\$@"
EOF
chmod +x ${QMAKE}
export QMAKE

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
export LDAI_OUTPUT="actiona-${ACTIONA_VERSION}-x86_64.AppImage"
export LDAI_UPDATE_INFORMATION='gh-releases-zsync|Jmgr|actiona|latest|actiona-*-x86_64.AppImage.zsync'

./linuxdeploy-x86_64.AppImage \
        --appdir        ${APPDIR} \
        --executable    ${EXECUTABLE} \
        --desktop-file  ${DESKTOP_FILE} \
        --icon-file     ${ICON_FILE} \
        --library       ${OPENSSL_LIBSSL} \
        --library       ${OPENSSL_LIBCRYPTO} \
        --output        appimage

test -s "${LDAI_OUTPUT}"
test -s "${LDAI_OUTPUT}.zsync"
