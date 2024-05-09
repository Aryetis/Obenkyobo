export APPNAME=templateAppName
export ADDSPATH=/mnt/onboard/.adds
export APPPATH=${ADDSPATH}/${APPNAME}
export QTPATH=${ADDSPATH}/templateQtDir
export QTPLUGINOBENKYOBO=kobo

LOGFILE=${APPPATH}/log.txt

# kill nickel
source ${APPPATH}/exit_nickel.sh

# koboplatformplugin input debug
#export QT_QPA_EVDEV_DEBUG=true

# export QT stuff
export LD_LIBRARY_PATH=${QTPATH}/lib:${APPPATH}/lib:lib:
export QT_QPA_PLATFORM=${QTPLUGINOBENKYOBO}:debug
