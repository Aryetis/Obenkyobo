#!/bin/bash
set -e #-x
# Script called by Obenkyobo.pro's QMAKE_POST_LINK
# packager.sh release [/]Obenkyobo/Src/Obenkyobo [/]Obenkyobo/build-ObenkyoboProject-KoboLibraH2o-Release/Src/Obenkyobo Obenkyobo
#[BuildPath].adds/   [Dependencies]
#				  | [kfmon] / config / [ProjectName].ini
#				  | [nm] / [ProjectName]
#				  | [ProjectName] /   [ProjectName]
#				  | [LauncherName].sh & exit_nickel.sh & start_nickel.sh
LongName="Obenkyobo !"
RemoteIp="192.168.1.18"
Author="Aryetis"
KfMonDbComment="Obenkyo clone-ish"
Version="0.3.1"
LauncherName=$4_launcher.sh
QtPluginKobo=kobo
# for easier deployment, make your qt binary folder, this QtFolder variable and (if necessary) koboplatformplugin's .pri CUSTOM_QTDIR match with each others
QtFolder="qt-linux-5.15-kde-kobo"
BuildFolder=$3/../..

echo ===============================================================
echo packager.sh $1 $2 $3 $4
echo ===============================================================

#Launcher and application
mkdir -p $BuildFolder/Output/.adds/$4
cp $3/$4 $BuildFolder/Output/.adds/$4/$4
cp $2/OtherFiles/RemoteScripts/launcher.sh $BuildFolder/Output/.adds/$4/$LauncherName
sed  -i "1s/.*/""export APPNAME=$4""/" $BuildFolder/Output/.adds/$4/$LauncherName
sed  -i "4s/.*/""export QTPATH=\${ADDSPATH}\/$QtFolder/g" $BuildFolder/Output/.adds/$4/$LauncherName
sed  -i "5s/.*/""export QTPLUGINOBENKYOBO=$QtPluginKobo/g" $BuildFolder/Output/.adds/$4/$LauncherName
cp $2/OtherFiles/RemoteScripts/start_nickel.sh $BuildFolder/Output/.adds/$4/start_nickel.sh
cp $2/OtherFiles/RemoteScripts/exit_nickel.sh $BuildFolder/Output/.adds/$4/exit_nickel.sh
cp $2/OtherFiles/RemoteScripts/pic.png $BuildFolder/Output/$4.png
echo Launcher and application - Done

#Vocab files
mkdir -p $BuildFolder/Output/.adds/$4/vocab
cp $2/OtherFiles/vocab/* $BuildFolder/Output/.adds/$4/vocab/ -r
echo Vocab files - Done

#Dependencies
for f in $2/OtherFiles/Dependencies/*;
do
  if [ -e "$f" ];
  then
    cp $f $BuildFolder/Output/.adds/ -rL
  else
    echo "!!!!! COULD NOT COPY (broken symbolic link ?) : "$f" !!!!!"
  fi
done
mkdir -p $BuildFolder/Output/.adds/$QtFolder/plugins/platforms/
cp $3/../Libs/qt5-kobo-platform-plugin/build/ereader/lib$QtPluginKobo.so $BuildFolder/Output/.adds/qt-linux-5.15-kde-kobo/plugins/platforms/lib$QtPluginKobo.so
mkdir -p $BuildFolder/Output/.adds/$4/lib/
cp -a $3/../Libs/KoboExtraFunk/libKoboExtraFunk* $BuildFolder/Output/.adds/$4/lib/
echo Dependencies - Done

#kfmon
mkdir -p $BuildFolder/Output/.adds/kfmon/config
cp $2/OtherFiles/RemoteScripts/KfMonTemplate.ini $BuildFolder/Output/.adds/kfmon/config/$4.ini
sed -i "s/shortName/$4/g" $BuildFolder/Output/.adds/kfmon/config/$4.ini
sed -i "s/longName/$LongName/g" $BuildFolder/Output/.adds/kfmon/config/$4.ini
sed -i "s/KfMonDbComment/$KfMonDbComment/g" $BuildFolder/Output/.adds/kfmon/config/$4.ini
sed -i "s/launcherName/$LauncherName/g" $BuildFolder/Output/.adds/kfmon/config/$4.ini
echo kfmon - Done

#NickelMenu
mkdir -p $BuildFolder/Output/.adds/nm
cp $2/OtherFiles/RemoteScripts/nmTemplate $BuildFolder/Output/.adds/nm/$4
sed -i "s/shortName/$4/g" $BuildFolder/Output/.adds/nm/$4
sed -i "s/longName/$LongName/g" $BuildFolder/Output/.adds/nm/$4
sed -i "s~launcherAddress~/mnt/onboard/.adds/$4/$LauncherName~g" $BuildFolder/Output/.adds/nm/$4
echo NickelMenu - Done

# zip it up if building release
if [[ $1 == release ]];
then
    cd $BuildFolder/Output
    zip -FSr ${ProjectSubDirName}_AIO_Release_$Version.zip ./
    echo zip - Done
fi

echo ===============================================================
echo Packaging done !
echo ===============================================================
