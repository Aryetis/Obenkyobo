#!/bin/bash
# QtCreator parameters %{CurrentBuild:Type} %{sourceDir} %{CurrentProject:BuildPath} %{CurrentProject:Name}
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

echo ===============================================================
echo packager.sh $1\;$2\;$3\;$4
echo ===============================================================

#Launcher and application
mkdir -p $3/Output/.adds/$4
cp $3/$4 $3/Output/.adds/$4/$4
cp $2/OtherFiles/RemoteScripts/launcher.sh $3/Output/.adds/$4/$LauncherName
sed  -i "1s/.*/""export APPNAME=$4""/" $3/Output/.adds/$4/$LauncherName
sed  -i "5s/.*/""export QTPLUGINOBENKYOBO=$QtPluginKobo/g" $3/Output/.adds/$4/$LauncherName
if [[ $1 == debug ]];
then
    cp $2/OtherFiles/RemoteScripts/debugEnv.sh $3/Output/.adds/$4/debugEnv.sh
    sed  -i "1s/.*/""export APPNAME=$4""/" $3/Output/.adds/$4/debugEnv.sh
    sed  -i "5s/.*/""export QTPLUGINOBENKYOBO=$QtPluginKobo/g" $3/Output/.adds/$4/debugEnv.sh
fi
cp $2/OtherFiles/RemoteScripts/start_nickel.sh $3/Output/.adds/$4/start_nickel.sh
cp $2/OtherFiles/RemoteScripts/exit_nickel.sh $3/Output/.adds/$4/exit_nickel.sh
cp $2/OtherFiles/RemoteScripts/pic.png $3/Output/$4.png

#Vocab files
mkdir -p $3/Output/.adds/$4/vocab
cp $2/OtherFiles/vocab/* $3/Output/.adds/$4/vocab/ -r

#Dependencies
cp $2/OtherFiles/Dependencies/* $3/Output/.adds/ -rL
mkdir -p $3/Output/.adds/qt-linux-5.15-kde-kobo/plugins/platforms/
if [[ $1 == debug ]];
then
    cp $2/../qt5-kobo-platform-plugin/lib$QtPluginKobo.so.debug $3/Output/.adds/qt-linux-5.15-kde-kobo/plugins/platforms/lib$QtPluginKobo.so
else
    cp $2/../qt5-kobo-platform-plugin/lib$QtPluginKobo.so $3/Output/.adds/qt-linux-5.15-kde-kobo/plugins/platforms/lib$QtPluginKobo.so
fi

#kfmon
mkdir -p $3/Output/.adds/kfmon/config
cp $2/OtherFiles/RemoteScripts/KfMonTemplate.ini $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/shortName/$4/g" $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/longName/$LongName/g" $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/KfMonDbComment/$KfMonDbComment/g" $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/launcherName/$LauncherName/g" $3/Output/.adds/kfmon/config/$4.ini

#NickelMenu
mkdir -p $3/Output/.adds/nm
cp $2/OtherFiles/RemoteScripts/nmTemplate $3/Output/.adds/nm/$4
sed -i "s/shortName/$4/g" $3/Output/.adds/nm/$4
sed -i "s/longName/$LongName/g" $3/Output/.adds/nm/$4
sed -i "s~launcherAddress~/mnt/onboard/.adds/$4/$LauncherName~g" $3/Output/.adds/nm/$4

#+ zip it up if building release
if [[ $1 == release ]];
then
    cd $3/Output
    zip -FSr $4_AIO_Release_$Version.zip ./
fi
