#!/bin/sh
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:$QTDIR/lib
export LD_LIBRARY_PATH
./actionaz.bin
