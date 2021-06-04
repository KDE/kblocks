#! /bin/sh
$EXTRACTRC *.rc *.kcfg >> rc.cpp
$XGETTEXT `find . -name '*.cpp'` -o $podir/kblocks.pot
