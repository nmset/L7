#!/bin/bash
if [ ! -x 2_makemo.sh ]
then
    echo "Wrong working directory. Please cd to the directory containing this script."
    exit 0
fi

APPNAME=L7
DOMAIN=fr

msgfmt $DOMAIN/$APPNAME.po -o $DOMAIN/$APPNAME.mo

exit 0

