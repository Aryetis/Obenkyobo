#!/bin/bash
set -e #-x
# QtCreator parameters %{CurrentBuild:Type} %{sourceDir} %{CurrentProject:BuildPath}
#[BuildPath].adds/   [Dependencies]
#				  | [kfmon] / config / [ProjectName].ini
#				  | [nm] / [ProjectName]
#				  | [ProjectName] /   [ProjectName]
#				  | [LauncherName].sh & exit_nickel.sh & start_nickel.sh
LongName="Obenkyobo !"
ProjectSubDirName="Obenkyobo"
RemoteIp="192.168.1.18"
Author="Aryetis"
KfMonDbComment="Obenkyo clone-ish"
Version="0.3.1"
LauncherName=${ProjectSubDirName}_launcher.sh
QtPluginKobo=kobo
QtFolder="qt-linux-5.15-kde-kobo"

echo ===============================================================
echo packager.sh $1 $2 $3
echo ===============================================================

#Launcher and application
mkdir -p $3/Output/.adds/$ProjectSubDirName
cp $3/Src/$ProjectSubDirName/$ProjectSubDirName $3/Output/.adds/$ProjectSubDirName/$ProjectSubDirName
cp $2/Src/$ProjectSubDirName/OtherFiles/RemoteScripts/launcher.sh $3/Output/.adds/$ProjectSubDirName/$LauncherName
sed  -i "1s/.*/""export APPNAME=${ProjectSubDirName}""/" $3/Output/.adds/$ProjectSubDirName/$LauncherName
sed  -i "5s/.*/""export QTPLUGINOBENKYOBO=$QtPluginKobo/g" $3/Output/.adds/$ProjectSubDirName/$LauncherName
if [[ $1 == debug ]];
then
    cp $2/Src/$ProjectSubDirName/OtherFiles/RemoteScripts/debugEnv.sh $3/Output/.adds/$ProjectSubDirName/debugEnv.sh
    sed  -i "1s/.*/""export APPNAME=${ProjectSubDirName}""/" $3/Output/.adds/$ProjectSubDirName/debugEnv.sh
    sed  -i "5s/.*/""export QTPLUGINOBENKYOBO=$QtPluginKobo/g" $3/Output/.adds/$ProjectSubDirName/debugEnv.sh
fi
cp $2/Src/$ProjectSubDirName/OtherFiles/RemoteScripts/start_nickel.sh $3/Output/.adds/$ProjectSubDirName/start_nickel.sh
cp $2/Src/$ProjectSubDirName/OtherFiles/RemoteScripts/exit_nickel.sh $3/Output/.adds/$ProjectSubDirName/exit_nickel.sh
cp $2/Src/$ProjectSubDirName/OtherFiles/RemoteScripts/pic.png $3/Output/$ProjectSubDirName.png
echo Launcher and application - Done

#Vocab files
mkdir -p $3/Output/.adds/${ProjectSubDirName}/vocab
cp $2/Src/$ProjectSubDirName/OtherFiles/vocab/* $3/Output/.adds/${ProjectSubDirName}/vocab/ -r
echo Vocab files - Done

#Dependencies
for f in $2/Src/$ProjectSubDirName/OtherFiles/Dependencies/*;
do
  if [ -e "$f" ];
  then
    cp $f $3/Output/.adds/ -rL
  else
    echo "!!!!! COULD NOT COPY (broken symbolic link ?) : "$f" !!!!!"
  fi
done
mkdir -p $3/Output/.adds/$QtFolder/plugins/platforms/
cp $3/Src/Libs/qt5-kobo-platform-plugin/lib$QtPluginKobo.so $3/Output/.adds/qt-linux-5.15-kde-kobo/plugins/platforms/lib$QtPluginKobo.so
echo Dependencies - Done

#kfmon
mkdir -p $3/Output/.adds/kfmon/config
cp $2/Src/$ProjectSubDirName/OtherFiles/RemoteScripts/KfMonTemplate.ini $3/Output/.adds/kfmon/config/${ProjectSubDirName}.ini
sed -i "s/shortName/${ProjectSubDirName}/g" $3/Output/.adds/kfmon/config/${ProjectSubDirName}.ini
sed -i "s/longName/$LongName/g" $3/Output/.adds/kfmon/config/${ProjectSubDirName}.ini
sed -i "s/KfMonDbComment/$KfMonDbComment/g" $3/Output/.adds/kfmon/config/${ProjectSubDirName}.ini
sed -i "s/launcherName/$LauncherName/g" $3/Output/.adds/kfmon/config/${ProjectSubDirName}.ini
echo kfmon - Done

#NickelMenu
mkdir -p $3/Output/.adds/nm
cp $2/Src/$ProjectSubDirName/OtherFiles/RemoteScripts/nmTemplate $3/Output/.adds/nm/$ProjectSubDirName
sed -i "s/shortName/${ProjectSubDirName}/g" $3/Output/.adds/nm/$ProjectSubDirName
sed -i "s/longName/$LongName/g" $3/Output/.adds/nm/$ProjectSubDirName
sed -i "s~launcherAddress~/mnt/onboard/.adds/${ProjectSubDirName}/$LauncherName~g" $3/Output/.adds/nm/$ProjectSubDirName
echo NickelMenu - Done

# zip it up if building release
if [[ $1 == release ]];
then
    cd $3/Output
    zip -FSr ${ProjectSubDirName}_AIO_Release_$Version.zip ./
    echo zip - Done
fi

echo ===============================================================
echo Packaging done !
echo ===============================================================
