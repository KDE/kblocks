#! /bin/sh
$EXTRACTRC *.rc *.kcfg >> rc.cpp
$XGETTEXT rc.cpp *.cpp -o $podir/kblocks.pot
