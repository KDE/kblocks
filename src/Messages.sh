#! /bin/sh
$EXTRACTRC `find . -name \*.rc -o -name \*.kcfg` >> rc.cpp
$XGETTEXT `find . -name '*.cpp'` -o $podir/kblocks.pot
