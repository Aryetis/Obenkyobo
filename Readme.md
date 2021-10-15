# Obenkyobo

## What is it ?

Obenkyobo is an application for kyobo ereaders designed to help you memorize Hiraganas and Katakanas. 

```diff
- Please be aware, this application is still early in its life cycle. For the time being there is no wifi nor sleep support. So to preserve your battery I recommend quiting the application after each session.
```

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

## Features ? (and Roadmap)

Here's what available currently
- Hiragana/Katakana to Romanji MCQ
- Romanji to Hiragana/Katakana MCQ
- editable MCQ Hiragana/Katakana entry pool to learn at your own pace 
- weighted random / "learning state" for each Hiragana/Katakana to keep MCQ more entertaining (of course, progress is stored in between each session)
- Multiple fonts and fonts settings
- Screen options (Luminosity/Tint) support


What's planned / Roadmap (in order of priority):
(For incoming weeks/months)
- Wifi support 
- Sleep/WakeUp support
- Hiragana/Katakana to Romanji Keyboard exercices
- Overall UI improvement 
- proper "Obenkyobo cover.png"
(Maybe next year)
- "learn how to count" exercices
- "guess the particle の, は, が, etc" exercices
(Somewhen between the release of Bayonetta 3 and infinity)
- include "kanji-data-media" dictionary 

Tested on : 
- libra h2o
- please report if it works for your model too

## How to install ?

Firstly, this application is designed to be compatible with most (if not all) Kobo devices. Do not try to install it for other ereader, it will not work ! This application is provided "as is". Don't come to me if somehow your device catch fire.

Secondly, you'll need to install a "launcher" beforehand. Common options are : 
- <a href="https://github.com/pgaskin/NickelMenu">NickelMenu (NM)</a>, recommended option
- <a href="https://github.com/pgaskin/NickelMenu">Kute File Monitor (KfMon)</a>

Once you're done installing at least one of those you can either : 
- Download the following All-In-One package (containing Obenkyobo + Qt Libraries + QTQPA libkobo.so + NM and KfMon entries) <a href="https://hijackedbrain.com/data/Obenkyobo_AIO_Release_V0.1.zip">Obenkyobo_AIO_Release_V0.1.zip</a> , connect the ereader to your computer and extract the package at its root
- Donwload the following "no qt" package (containing Obenkyobo + NM and KfMon entries) <a href="https://hijackedbrain.com/data/Obenkyobo_Light_Release_V0.1.zip">Obenkyobo_Light_Release_V0.1.zip</a> and install it the same way. Please note that Obenkyobo REQUIRES those Qt Libraries. Only select this one if you know what you're doing and have already Qt Libraries (same version as in the AIO package) and libkobo.so installed on your device from another program such as <a href="https://github.com/Rain92/UltimateMangaReader/">Ultimate Manga Reader</a>

<img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/NickelMenu.png" width="303" height="184" align="left"/>
<br/>
<br/>
<br/>
And voilà ! You should now have a NickelMenu entries at the bottom right corner of your ereader "home screen" and a new "book" in your library to open with KfMon
<br clear="left"/>

## issues ? 

Is your screen rotated the wrong way ? Are your touch inputs broken ? This kind of issue can happen on very old/new devices/untested devices. You can force rotations of screen and touch inputs by modifying the `QT_QPA_PLATFORM` variable in `.adds\\Obenkyobo\\Obenkyobo_launcher.sh` on your ereader (depending of your OS you might have to <a href="https://www.google.com/search?client=firefox-b-d&q=enable+hidden+folder+">enable "hidden folder"</a>) :
```
touchscreen_rotate=auto|0|90|180|270
touchscreen_invert_x=auto|0|1
touchscreen_invert_y=auto|0|1 

eg : export QT_QPA_PLATFORM=kobo:touchscreen_rotate=auto:touchscreen_invert_x=auto:touchscreen_invert_y=auto
```

If you encounter any issues, please report them by either : 
- Creating a new Ticket on Github <a href="https://github.com/Aryetis/Obenkyobo/issues">here</a> describing your issue as precisely as possible, include the type of ereader your using  
- Respond in Obenkyobo's MobileRead thread <a href="">TODOTODOTODOTODOTODOTODO</a>in the same way.


## Thanks

Thank you for actually reading a readme file

And big big thanks to <a href="https://github.com/Rain92/">@Rain92/OfficerAction</a> , <a href="https://github.com/NiLuJe">@NiLuJe</a> and <a href="https://github.com/tux-linux">@NiMa</a> for putting up with me on Discord helping me setup my environment and walking me throught some Qt Kobo specific minefields. 

## For developpers, dummies, and future me. How to compile it ?

### Setup the environment 

You can setup a very basic Kobo dev environment by following either 
- the <a href="https://github.com/koreader/koxtoolchain">koxtoolchain instructions</a> and work your way from here by cross-compiling qt for ARM and the QTPA kobo platform plugin. To validate your arm gcc, run your first non graphical "hello world" throught an ssh-server or use koreader "run command" option.
- Or you can go the "easy" route and use @Rain92's <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a> to setup everything "libs, Qt binaries, deployment scripts".

Once you've got your arm QtBinaries, copy its folder (with necessaries additional libs) to `OtherFiles/Dependencies`

For Obenkyobo to work you'll also have to compile <a href="https://github.com/Rain92/qt5-kobo-platform-plugin">qt5-kobo-platform-plugin</a> and copy the produced `libkobo.so` to `OtherFiles/Dependencies/[QtBinaries]/plugins/platforms` folder.

You'll probably run along some troubles, so to save you some time here's the tldr version from the <a href="https://discord.com/channels/793941135419506728/796445063127236648/897503681275129876">discord discussion</a>

To compile the problematic brotli library, tldr switch to the 3.0.0 branch by :
1. comment previous and following libs related content and prevent any git checkout/clean from install_libs.sh
2. git checkout 3.0.0 in brotly's folder : 'kobo-qt-setup-scripts/libs/brotli/'
3. run install_libs.sh script again
4. comment brotli's section, restore git checkout stuff,  rerun install_libs.sh and move on

fix for libfreetype with harfbuzz not compiling due to pthreads issues, run the following commands (will very probably break at some point in the future) :
```
export PKG_CONFIG_PATH=""
export PKG_CONFIG_LIBDIR="${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/lib/pkgconfig:${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/share/pkgconfig"
export PKG_CONFIG="pkg-config"
```

fix for the deploy script : 
1. set the correct IP at the end
2. set correct QTBINPATH and kobopluginpath
3. add :
```
cp -t $TMPPATH/lib ${SYSROOT}/usr/lib/libbrotlidec.so.1
cp -t $TMPPATH/lib ${SYSROOT}/usr/lib/libbrotlicommon.so.1
```

Fill the following Obenkyobo.pro variables correctly : 
```
INCLUDEPATH += $$PWD/libs/qt5-kobo-platform-plugin/src # should link to the libkobo.so git local repo
INSTALLS += target everything thumbnail # use only this for full deploy, to save time set it to += target afterwards  
```

### Setup QtCreator

For a better workflow and one click build+deploy+launch from within QtCreator : 
```
Projects->Kobo(Kit)->Build->Add Custom Process Step (in both Release and debug) with : 
Command : %{sourceDir}/OtherFiles/packager.sh.
Arguments : %{ActiveProject:BuildConfig:Type} %{sourceDir} %{ActiveProject:BuildConfig:Path} %{ActiveProject:Name}
Working Directory : %{sourceDir}

Projects->Kobo(Kit)->Run->Deployment-> Add Run custom remote command (in both Release and debug) with :  
/mnt/onboard/.adds/Obenkyobo/debugEnv.sh

Projects->Kobo(Kit)->Run->Environment->(System Environment)->Add create new variable with at least 
LD_LIBRARY_PATH = /mnt/onboard/.adds/qt-linux-5.15-kde-kobo/lib:lib:
QT_QPA_PLATFORM = kobo
```

NEVER modify any of the .sh scripts under windows... Windows end of line will mess things up when ran on linux

### Setup gdb

Two solutions : 
- Use cross compiled arm gdb from <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a>
- Install gdb-multiarch and set it up for arm architecture in QtCreator by :  `Tools->Options->Debugger->GDB->Additional Startup Commands : -q --nh -ex 'set architecture arm'`

### Setup the Ereader

Setup a fixed IP address for it and use it in <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a>  deploy_script.sh if you go this route instead of using QtCreator

Ereader is rebooting upon Deployment ? Probably because kfmon/nm is scanning for the freshly installed/deployed stuff and reboots the device to update its nickelMenu entry. You should probably setup only select INSTALLS += target , when working daily

Turn on developer mode on the kobo by searching for a book named `devmodeon`, then force the wifi to stay on during dev session : Plus->Parameters->Technical informations->Developer options->force Wifi ON

Install <a href="https://www.mobileread.com/forums/showthread.php?t=254214">Niluje's kobo stuff</a> package to get various programs running on the kobo such as : 
- `dropbear` ssh server (connect with root and empty password)
- `nano`, `gdb`, `strace`
- `fbgrab picture.png` to take screenshots
- etc 

