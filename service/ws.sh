#!/bin/sh

# The script will be run by systemd service

# The working directory will be set to the top of the project:
# /home/zvyagin/Projects/QF/CurvesConstruction

# The web service files are deployed already to
# service/web-client

. bin/activate

(cd $VIRTUAL_ENV/src/web-client && npm run build) || exit 1

installdir=$VIRTUAL_ENV/service/web-client
(rm -rf ${installdir}) || exit 1
(mkdir ${installdir}) || exit 1
(cp -a $VIRTUAL_ENV/src/web-client/public/* ${installdir}) || exit 1

ws --port 8100 ${installdir}
