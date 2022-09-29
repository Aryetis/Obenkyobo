### Setup the environment 

You can setup a very basic Kobo dev environment by following either 
- the <a href="https://github.com/koreader/koxtoolchain">koxtoolchain instructions</a> and work your way from here by cross-compiling qt for ARM and the QTPA kobo platform plugin. To validate your arm gcc, run your first non graphical "hello world" throught an ssh-server or use koreader "run command" option.
- Or you can go the "easy" route and use @Rain92's <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a> to setup everything "libs, Qt binaries, deployment scripts".

Once you've got your arm QtBinaries, copy its folder (with necessaries additional libs) to `OtherFiles/Dependencies`

For Obenkyobo to work you'll also have to compile <a href="https://github.com/Aryetis/qt5-kobo-platform-plugin">qt5-kobo-platform-plugin</a> (for now use my fork of it as it includes a correct fbink build and a fix for sleepcover button) and have its folder next to Obenkyobo's folder. That way the packager.sh script will be able to grab the libkobo.so or libkobo.so.debug (you'll have to rename it) if you wish to build in debug mode.

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
QT_QPA_PLATFORM = kobo_obenkyobo (keep an eye on https://github.com/Rain92/qt5-kobo-platform-plugin to switch back to official qpa plugin once issues are resolved over here)
```

NEVER modify any of the .sh scripts under windows... Windows end of line will mess things up when ran on linux

### Setup gdb

Two solutions : 
- Use cross compiled arm gdb from <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a>. You might have to `apt install python2 python2-dev` and tweak the ./install_gdb.sh to add "--with-python" to the ./configure flags
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

### Miscellaneous

How to read the backtrace logs : convert address to line using `addr2line -e [NonStrippedProgramBinary] [HexAddress]`

### How to serial connect to Kobo ereader in WSL2 

1. install usbipd on windows's side to share usb device through IP with WSL2, more info on <a href="https://github.com/dorssel/usbipd-win/wiki/WSL-support">WSL-support's page</a>.
2. install bunch of necessary software in WSL2 (might be some unnecessary ones depends of your needs, distrib) with : 
`sudo apt install linux-tools-5.10 usbip hwdata usbutils usbserial setserial`
3. In some Admin Powershell (urgh), lis usb devices busids with : `usbipd wsl list`
4. link usb device to WSL2 with `usbipd wsl attach --busid [BUS_ID]`
5. check if /dev/ttyUSBxxx shows up. If not ... guess what ... WSL2 kernel probably doesn't have your device's driver. To be sure, check it out with `ls -l /sys/bus/usb-serial/drivers`. You'll have to build your own kernel then.

### How to build custom WSL2 kernel 

1. `sudo apt install build-essential flex bison libssl-dev libelf-dev git dwarves`
2. `git clone https://github.com/microsoft/WSL2-Linux-Kernel.git`
3. `cd WSL2-Linux-Kernel`
4. `cp Microsoft/config-wsl .config` (might as well change CONFIG_LOCALVERSION in it while you're at it so you can differentiate your kernel later on)
5. `Make menuconfig`
6. Enable your driver : `Device Drivers -> USB Support -> USB Serial Converter support -> [whatever device you're using]`
7. `make -j xxx` (xxx being how many cores you want to build with)
8. `cp arch/x86/boot/bzImage /mnt/c/[USER_NAME]/wsl_kernel`
9. Create the following file `C:\Users\<UserName>\.wslconfig with this content` : 
```
[wsl2]
kernel=C:\\Users\\aramir\\wsl_kernel
```
10. in powershell/cmd `wsl --shutdown` (just to be sure)
11. relaunch your WSL2 and check out your new kernel with : `uname -a` 
12. connect to your kobo with the usual `sudo minicom -D /dev/ttyUSB0` and voila !