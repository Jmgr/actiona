#!/usr/bin/env bash
set -e

cd "$(dirname "$0")"
cd ..

PROJECT_NAME="actiona"
VERSION=$(git describe --tags --abbrev=0 | sed 's/^v//')

ARCHIVE_NAME="${PROJECT_NAME}-${VERSION}-source"
MAIN_TAR="${ARCHIVE_NAME}.tar"

echo "Creating main tar archive for version: ${VERSION}"

# Update submodules
git submodule update --init --recursive

# 1) Create a single main archive (including submodules)
git archive --prefix "${ARCHIVE_NAME}/" -o "${MAIN_TAR}" HEAD
git submodule foreach --recursive \
  "git archive --prefix=${ARCHIVE_NAME}/\$path/ --output=\$sha1.tar HEAD && \
   tar --concatenate --file=$(pwd)/${MAIN_TAR} \$sha1.tar && \
   rm \$sha1.tar"

# 2) Create the Windows archive by copying the main tar
WINDOWS_TAR="${ARCHIVE_NAME}-windows.tar"
cp "${MAIN_TAR}" "${WINDOWS_TAR}"
gzip -c "${WINDOWS_TAR}" > "${WINDOWS_TAR}.gz"
echo "Created ${WINDOWS_TAR}.gz"

# 3) Create the Linux archive by copying the main tar and removing unwanted directories
LINUX_TAR="${ARCHIVE_NAME}-linux.tar"
cp "${MAIN_TAR}" "${LINUX_TAR}"

# Adjust paths inside the archive to match exactly how they're stored
tar --delete --file="${LINUX_TAR}" \
  "${ARCHIVE_NAME}/actiona/3rdparty/QDarkStyleSheet/docs" \
  "${ARCHIVE_NAME}/actiona/src/3rdparty/breakpad"

gzip -c "${LINUX_TAR}" > "${LINUX_TAR}.gz"
echo "Created ${LINUX_TAR}.gz"

# 4) (Optional) Cleanup intermediate tars if you only want the compressed files
rm -f "${MAIN_TAR}" "${WINDOWS_TAR}" "${LINUX_TAR}"
