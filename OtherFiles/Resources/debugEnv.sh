export APPNAME=Obenkyobo
export ADDSPATH=/mnt/onboard/.adds
export APPPATH=${ADDSPATH}/${APPNAME}

LOGFILE=${APPPATH}/log.txt

# kill nickel
source ${APPPATH}/exit_nickel.sh

# export QT stuff
export QT_QPA_PLATFORM=kobo:touchscreen_rotate=auto:touchscreen_invert_x=auto:touchscreen_invert_y=auto:logicaldpitarget=108
