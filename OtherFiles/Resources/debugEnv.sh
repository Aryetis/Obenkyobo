export APPNAME=Obenkyobo
export ADDSPATH=/mnt/onboard/.adds
export APPPATH=${ADDSPATH}/${APPNAME}
export QTPATH=${ADDSPATH}/qt-linux-5.15.2-kobo
export QTPLUGINOBENKYOBO=kobo

LOGFILE=${APPPATH}/log.txt

# kill nickel
source ${APPPATH}/exit_nickel.sh

# export QT stuff
export LD_LIBRARY_PATH=${QTPATH}/lib:lib:
export QT_QPA_PLATFORM=${QTPLUGINOBENKYOBO}:touchscreen_rotate=auto:touchscreen_invert_x=auto:touchscreen_invert_y=auto:logicaldpitarget=108
