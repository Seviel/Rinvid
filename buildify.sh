#!/bin/sh

# Note: this script should be ran from repo root folder

tools/buildifier/buildifier -r `find -iname BUILD`
