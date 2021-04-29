#!/bin/sh

# Note: this script should be ran from repo root folder

OS=`uname -s`

if [ "$OS" = "Linux" ]; then
    tools/buildifier/buildifier -r `find -iname BUILD`
else
    tools/buildifier/buildifier_win -r `find -iname BUILD`
fi
