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
LauncherName=$4_launcher.sh
#QtPluginKobo=kobo_obenkyobo  #Make it match koboplatformplugin.json key used to compile libkobo.so
QtPluginKobo=kobo


#Launcher and application
mkdir -p $3/Output/.adds/$4
cp $3/$4 $3/Output/.adds/$4/$4
cp $2/OtherFiles/Resources/launcher.sh $3/Output/.adds/$4/$LauncherName
sed  -i "1s/.*/""export APPNAME=$4""/" $3/Output/.adds/$4/$LauncherName
sed  -i "5s/.*/""export QTPLUGINOBENKYOBO=$QtPluginKobo/g" $3/Output/.adds/$4/$LauncherName
if [[ $1 == debug ]];
then
    cp $2/OtherFiles/Resources/debugEnv.sh $3/Output/.adds/$4/debugEnv.sh
    sed  -i "1s/.*/""export APPNAME=$4""/" $3/Output/.adds/$4/debugEnv.sh
fi
cp $2/OtherFiles/Resources/start_nickel.sh $3/Output/.adds/$4/start_nickel.sh
cp $2/OtherFiles/Resources/exit_nickel.sh $3/Output/.adds/$4/exit_nickel.sh
cp $2/OtherFiles/Resources/pic.png $3/Output/$4.png

#Dependencies
cp $2/OtherFiles/Dependencies/* $3/Output/.adds/ -r

#kfmon
mkdir -p $3/Output/.adds/kfmon/config
cp $2/OtherFiles/Resources/KfMonTemplate.ini $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/shortName/$4/g" $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/longName/$LongName/g" $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/KfMonDbComment/$KfMonDbComment/g" $3/Output/.adds/kfmon/config/$4.ini
sed -i "s/launcherName/$LauncherName/g" $3/Output/.adds/kfmon/config/$4.ini

#NickelMenu
mkdir -p $3/Output/.adds/nm
cp $2/OtherFiles/Resources/nmTemplate $3/Output/.adds/nm/$4
sed -i "s/shortName/$4/g" $3/Output/.adds/nm/$4
sed -i "s/longName/$LongName/g" $3/Output/.adds/nm/$4
sed -i "s~launcherAddress~/mnt/onboard/.adds/$4/$LauncherName~g" $3/Output/.adds/nm/$4

#Renaming QTPA plugin
#mv $3/Output/.adds/qt-linux-5.15.2-kobo/plugins/platforms/libkobo.so $3/Output/.adds/qt-linux-5.15.2-kobo/plugins/platforms/lib$QtPluginKobo.so

#+ zip it up if building release
if [[ $1 == release ]];
then
    cd $3/Output
    zip -FSr $4_AIO_Release.zip ./
fi
