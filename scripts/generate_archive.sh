#!/usr/bin/env bash
set -e

cd "$(dirname "$0")"
cd ..

PROJECT_NAME="actiona"
VERSION=$(git describe --tags --abbrev=0 | sed 's/^v//')
ARCHIVE_NAME="${PROJECT_NAME}-${VERSION}-source"

echo "Creating tar archive for version: ${VERSION}"
git submodule update --init --recursive

git archive --prefix "${ARCHIVE_NAME}/" -o "${ARCHIVE_NAME}.tar" HEAD
git submodule foreach --recursive "git archive --prefix=${ARCHIVE_NAME}/\$path/ --output=\$sha1.tar HEAD && tar --concatenate --file=$(pwd)/${ARCHIVE_NAME}.tar \$sha1.tar && rm \$sha1.tar"

gzip "${ARCHIVE_NAME}.tar"

echo "Created ${ARCHIVE_NAME}.gz"
