QtCreator specific setup steps : 

Use cross compiled arm gdb (install with this script : https://github.com/Rain92/kobo-qt-setup-scripts/blob/master/install_gbd.sh )

Or 

Install gdb-multiarch and set it up for arm architecture in QtCreator by : 
Tools->Options->Debugger->GDB->Additional Startup Commands : -q --nh -ex 'set architecture arm'

----------
 
Projects->Kobo(Kit)->Build->Add Custom Process Step with : 
Command : %{sourceDir}/OtherFiles/packager.sh.
Arguments : %{ActiveProject:BuildConfig:Type} %{sourceDir} %{ActiveProject:BuildConfig:Path} %{ActiveProject:Name}
Working Directory : %{sourceDir}
(for both Release and debug)

----------

Projects->Kobo(Kit)->Run->Deployment-> Add Run custom remote command : 
/mnt/onboard/.adds/Obenkyobo/debugEnv.sh
(for both Release and debug)

-----------

Projects->Kobo(Kit)->Run->Environment->(System Environment)->Add create new variable with at least 
QT_QPA_PLATFORM=kobo
(for both Release and debug)

Should create everyfile needed and send them over to the kobo, stop nickel and start the application upon clicking the run button. Ok bye !

------------
Ereader is rebooting upon Deployment ? Probably because kfmon/nm is scanning for the freshly installed/deployed launcher.sh and reboots the device to update its nickelMenu entry

=> only select INSTALLS += target , when working daily

------------
NEVER ever modify .sh scripts under windows.... Thanks for inserting different line ending character and fucking up the whole script

------------
Force wifi-on in dev mode cause ... yeah do it

------------
Alpha Release link (no wifi setting, no sleep, no forced screen refresh, no Keyboard exercice) : https://hijackedbrain.com/data/Obenkyobo_AIO_AlphaRelease.zip

------------
link to the qt setup discord discussion https://discord.com/channels/793941135419506728/796445063127236648/897503681275129876
how to fix brotli compilation tldr : switch to the 3.0.0 branch:
1/ comment previous content from install_libs.sh
2/ git checkout 3.0.0 in brotly 
3/ run install_libs.sh script

fix for libfreetype with harfbuzz : 
export PKG_CONFIG_PATH=""
export PKG_CONFIG_LIBDIR="${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/lib/pkgconfig:${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/share/pkgconfig"
export PKG_CONFIG="pkg-config"
(will very probably break at some point in the future)

fix for the deploy script : 
1/ set the correct IP at the end
2/ set correct QTBINPATH and kobopluginpath
3/ add :
cp -t $TMPPATH/lib ${SYSROOT}/usr/lib/libbrotlidec.so.1
cp -t $TMPPATH/lib ${SYSROOT}/usr/lib/libbrotlicommon.so.1
