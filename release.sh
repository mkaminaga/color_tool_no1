#!/bin/bash
# Author Mamoru Kaminaga
# Date 2017/11/19
# Shell script for release
# Copyright 2017 Mamoru Kaminaga
# This program is provided with MIT license. See "LICENSE.md".

# Required: The latest version.
VERSION='v1.0.0_alpha4'

# Output directory is created
OUTDIR="release"
mkdir -p ${OUTDIR}

touch *.cc *.rc
nmake | iconv -f cp932 -t utf-8
if [ $? != 0 ]; then exit 1; fi

TARGET="${OUTDIR}/release_${VERSION}"

rm -r ${TARGET}
mkdir -p ${TARGET}

mkdir -p ${TARGET}/data/
cp -r data/*.png ${TARGET}/data/

mkdir -p ${TARGET}/doc/
cp -r doc/*.png ${TARGET}/doc/

cp -r colors/ ${TARGET}/

cp -r *.md ${TARGET}/
cp *.exe ${TARGET}/

# Zip file is created.
zip -r ${TARGET}.zip ${TARGET}

exit 0
