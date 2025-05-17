#!/bin/sh

# https://www.gnu.org/software/autoconf-archive/

AUTOCONF_ARCHIVE_REPO="https://raw.githubusercontent.com/autoconf-archive/autoconf-archive"
AUTOCONF_ARCHIVE_PATH="$AUTOCONF_ARCHIVE_REPO/refs/heads/master/m4"

wget $AUTOCONF_ARCHIVE_PATH/ax_prog_doxygen.m4 -O doxygen.m4
wget $AUTOCONF_ARCHIVE_PATH/ax_check_enable_debug.m4 -O endebug.m4
