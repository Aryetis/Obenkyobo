# Obenkyobo

## What is it ?

Obenkyobo is an application for kyobo ereader designed to help you memorize Hiraganas and Katakanas.

Please be aware, this application is still early in its life cycle. For the time being there is no wifi nor sleep support. So to preserve your battery I recommend quiting the application after each session.

## How does it look ?
<p align="center">
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/welcome.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/qcmHiragana1.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/qcmHiragana2.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/qcmKatakana.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/editSymbols1.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/appSettings.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/fntSettings.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/screenSetting.png" width="252" height="320" >
</p>

## What are its features ? (and its Roadmap)

Here's what available currently
- Hiragana/Katakana to Romanji MCQ
- Romanji -> Hiragana/Katakana MCQ
- editable MCQ Hiragana/Katakana entry pool to learn at your own pace 
- weighted random / "learning state" for each Hiragana/Katakana to keep MCQ more entertaining (of course, progress is stored in between each session)
- Multiple fonts and fonts settings
- Screen options (Luminosity/Tint) support

What's planned / Roadmap (in order of priority):
(For incoming weeks/months)
- Sleep/WakeUp support
- Wifi support
- Hiragana/Katakana to Romanji Keyboard exercices
- Overall UI improvement 
- proper "Obenkyobo cover.png"
(Maybe next year)
- "learn how to count" exercices
- "guess the particle の, は, が, etc" exercices
(Somewhen between the release of Bayonetta 3 and infinity)
- include "kanji-data-media" dictionary 

## How to install ?

Firstly, this application is designed to be compatible with most (if not all) Kobo devices. Do not try to install it for other ereader, it will not work ! This application is provided "as is". Don't come to me if somehow your device catch fire.

Secondly, you'll need to install a "launcher" beforehand. Common options are : 
- <a href="https://github.com/pgaskin/NickelMenu">NickelMenu (NM)</a>
- <a href="https://github.com/pgaskin/NickelMenu">Kute File Monitor (KfMon)</a>

Once you're done installing at least one of those you can either : 
- Download the following All-In-One package (containing Obenkyobo + Qt Libraries + NM and KfMon entries) TODOTODOTODOTODOTODOTODO , connect the ereader to your computer and extract the package at its root
- Donwload the following "no qt" package (containing Obenkyobo + NM and KfMon entries) TODOTODOTODOTODOTODOTODO and install it the same way. Please note that Obenkyobo REQUIRES those Qt Libraries. Only select this one if you know what you're doing and have already Qt Libraries (same version) installed on your device from another program such as <a href="https://github.com/Rain92/UltimateMangaReader/">Ultimate Manga Reader</a>

And voilà ! You should now have a NickelMenu entries at the bottom right corner of your ereader "home screen" and a new "book" in your library to open with KfMon
<p>
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/NickelMenu.png" width="252" height="320" >
</p>

## Got issues ? 

Is your screen rotated the wrong way ? Are your touch inputs broken ?
TODOTODOTODOTODOTODOTODO

If you encounter any issues, please report them by either : 
- Creating a new Ticket on Github <a href="https://github.com/Aryetis/Obenkyobo/issues">here</a> describing your issue as precisely as possible, include the type of ereader your using  
- Respond in Obenkyobo's MobileRead thread <a href="">TODOTODOTODOTODOTODOTODO</a>in the same way.


## Thanks

Thank you for actually reading a readme file

And big big thanks to @Rain92/OfficerAction, @NiLuJe and @NiMa for putting up with me on Discord helping me to setup my environment and walking me throught some Qt Kobo specific minefields. 

## For developpers, dummies, and future me. How to compile it ?

### Setup the environment 

You can setup a very basic Kobo dev environment by following the <a href="https://github.com/koreader/koxtoolchain">koxtoolchain instructions</a> and work your way from here by cross-compiling qt for ARM and the QTPA kobo platform plugin

Or you can go the "easy" route and use @Rain92 <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a> to setup everything "libs, Qt binaries, deployment scripts".

For Obenkyobo to work you'll also need to compile and install <a href="https://github.com/Rain92/qt5-kobo-platform-plugin">qt5-kobo-platform-plugin</a> copy the produced libkobo.so on your erear to the [QtBinaries]/plugins/platforms folder.

You'll probably run along some troubles, so to save you some time here's the tldr version from the <a href="https://discord.com/channels/793941135419506728/796445063127236648/897503681275129876">discord discussion</a>

To compile the problematic brotli library, tldr switch to the 3.0.0 branch by :
1/ comment previous and following libs related content and prevent any git checkout/clean from install_libs.sh
2/ git checkout 3.0.0 in brotly's folder : 'kobo-qt-setup-scripts/libs/brotli/'
3/ run install_libs.sh script again
4/ comment brotli's section, restore git checkout stuff,  rerun install_libs.sh and move on

fix for libfreetype with harfbuzz not compiling due to pthreads issues, run the following commands (will very probably break at some point in the future) :
```
export PKG_CONFIG_PATH=""
export PKG_CONFIG_LIBDIR="${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/lib/pkgconfig:${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/share/pkgconfig"
export PKG_CONFIG="pkg-config"
```

fix for the deploy script : 
1/ set the correct IP at the end
2/ set correct QTBINPATH and kobopluginpath
3/ add :
```
cp -t $TMPPATH/lib ${SYSROOT}/usr/lib/libbrotlidec.so.1
cp -t $TMPPATH/lib ${SYSROOT}/usr/lib/libbrotlicommon.so.1
```

### Setup QtCreator

 
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

NEVER ever modify .sh scripts under windows.... Thanks for inserting different line ending character and fucking up the whole script

Should create everyfile needed and send them over to the kobo, stop nickel and start the application upon clicking the run button. Ok bye !

### Setup gdb

QtCreator specific setup steps : 

Use cross compiled arm gdb (install with this script : https://github.com/Rain92/kobo-qt-setup-scripts/blob/master/install_gbd.sh )

Or 

Install gdb-multiarch and set it up for arm architecture in QtCreator by : 
Tools->Options->Debugger->GDB->Additional Startup Commands : -q --nh -ex 'set architecture arm'

### Setup the Ereader

Ereader is rebooting upon Deployment ? Probably because kfmon/nm is scanning for the freshly installed/deployed launcher.sh and reboots the device to update its nickelMenu entry

=> only select INSTALLS += target , when working daily

Force wifi-on in dev mode cause ... yeah do it


