### How to develop for Kobo ?

You can setup a very basic Kobo dev environment by following either 
- the <a href="https://github.com/koreader/koxtoolchain">koxtoolchain instructions</a> and work your way from here by cross-compiling qt for ARM and the QTPA kobo platform plugin. To validate your arm gcc, run your first non graphical "hello world" throught an ssh-server or use koreader "run command" option.
- Or you can go the "easy" route and use @Rain92's <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a> (or <a href="https://github.com/Aryetis/kobo-qt-setup-scripts">my fork of it</a> for now) to setup everything "libs, Qt binaries, deployment scripts".

### How to setup Obenkyobo dev environment using kobo-qt-setup-scripts ?
1. Run the following commands :
```
sudo apt-get install build-essential autoconf automake bison flex gawk libtool libtool-bin libncurses-dev curl file git gperf help2man texinfo unzip wget cmake pkg-config
git clone git@github.com:Rain92/qt5-kobo-platform-plugin.git
cd kobo-qt-setup-scripts
git submodule update --init --recursive
./install_toolchain.sh
./get_qt.sh kobo
########################################
# you probably want to add this one to your bashrc too
export PATH="$HOME/kobo/x-tools/arm-kobo-linux-gnueabihf/bin:$PATH"
########################################
./build_qt.sh kobo config
./build_qt.sh kobo make
./build_qt.sh kobo install
./deploy_qt.sh
```

2. For Obenkyobo to work and to debug it properly you'll also have to get and compile <a href="https://github.com/Rain92/qt5-kobo-platform-plugin">qt5-kobo-platform-plugin</a>. While setting it up, don't forget to `git submodule update --init --recursive`). 
Don't forget to configure its deploy script correctly by :
    - setting the correct IP at the end
    - setting correct QTBINPATH and kobopluginpath

3. ~~fix for libfreetype with harfbuzz not compiling due to pthreads issues, run the following commands (will very probably break at some point in the future) :~~ 

    **Useless step, fix should now be included in my fork**.
```
export PKG_CONFIG_PATH=""
export PKG_CONFIG_LIBDIR="${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/lib/pkgconfig:${HOME}/x-tools/arm-kobo-linux-gnueabihf/arm-kobo-linux-gnueabihf/sysroot/usr/share/pkgconfig"
export PKG_CONFIG="pkg-config"
```

4. The packager.sh scripts except to have access to a few things at specific places for now. So here are all the symbolink links you'll have to set :
    - to Qt Platform Plugin Base folder `[Obenkyobo]/Libs/qt5-kobo-platform-plugin` -> `[qt5-kobo-platform-plugin]`
    - from Qt Platform builds (release and debug) to Qt Platform Plugin Base 
        - `[qt5-kobo-platform-plugin]/libkobo.so` -> `[ReleaseBuildFolder]/libkobo.so`
        - `[qt5-kobo-platform-plugin]/libkobo.so.debug` -> `[DebugBuildFolder]/libkobo.so`
    - to Qt deployed binaries `[Obenkyobo]/OtherFiles/Dependencies/qt-linux-5.15-kde-kobo` -> `[kobo-qt-setup-scripts]/deploy/qt-linux-5.15-kde-kobo/`

5. Fill the following Obenkyobo.pro variables correctly : 
```
INCLUDEPATH += $$PWD/libs/qt5-kobo-platform-plugin/src # should link to the libkobo.so git local repo
INSTALLS += target everything thumbnail # use only this for full deploy, to save time set it to += target afterwards  
```

### Setup QtCreator

For a better workflow and one click build+deploy+launch from within QtCreator : 
```
Settings->Build & Run->Default Build Properties->Default build directory  : 
%{JS: Util.asciify("build-%{Project:Name}-%{Kit:FileSystemName}-%{BuildConfig:Name}")}

Projects->Kobo(Kit)->Build->Add Custom Process Step (in both Release and debug) with : 
Command : %{sourceDir}/OtherFiles/packager.sh
Arguments : %{ActiveProject:BuildConfig:Type} %{sourceDir} %{ActiveProject:BuildConfig:Path} %{ActiveProject:Name}
Working Directory : %{sourceDir}

Projects->Kobo(Kit)->Run->Deployment-> Upload files via SFTP instead of rsync
Projects->Kobo(Kit)->Run->Deployment-> Add Run custom remote command with :  
/mnt/onboard/.adds/Obenkyobo/debugEnv.sh

Projects->Kobo(Kit)->Run->Environment->(System Environment)->Add create new variable with at least 
LD_LIBRARY_PATH = /mnt/onboard/.adds/qt-linux-5.15-kde-kobo/lib:lib:
QT_QPA_PLATFORM = kobo_obenkyobo (keep an eye on https://github.com/Rain92/qt5-kobo-platform-plugin to switch back to official qpa plugin once issues are resolved over here)
```

NEVER modify any of the .sh scripts under windows... Windows end of line will mess things up when ran on linux, or at the very least use `win2unix` afterwards to fix line endings issues.

### Setup gdb

Two solutions : 
- Use cross compiled arm gdb from <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a>. You might have to `apt install python2/3 python2/3-dev` and tweak the ./install_gdb.sh to add "--with-python" to the ./configure flags
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

### Which serial port should I solder onto ?

This <a href="http://gethighstayhigh.co.uk/kobo-self-build/">gethighstayhigh blog entry</a> references a lot of Kobo internal layout. Thanks !

### Some Power Consumption measurements (needs to be redone with a proper methodology later on !!!)
```
Consumption at sleep and 100% battery (numbers read after a minute of stabilized consumption at most... Needs to redo tests with stricter methodology) : 
============= Kobo Glo HD =============
Nickel : 0.0129 A (takes a little longer to stabilize)
Obenkyobo : 0.0129 A (after the minute of Fake Sleep)
============== Kobo Glo ===============
Nickel : 0.0100 A (stable)
Obenkyobo : 0.0101 A ( (after the minute of Fake Sleep))
Inkbox OS : 0.0101 A (after enabling "sleep while charging" in experimental settings)
=========== Kobo Libra h2o ============
Nickel : 0.0102 A (takes longer to stabilize) 
Obenkyobo : 0.0101 A ( (after the minute of Fake Sleep)) 
UMR : 0.0425 -> 0.0429 A (oscillation for long time) then 0.0103 A
```

### How to build and get docs working in QTCreator (the dirty way)

set ~/qt-bin/qt-linux-5.15-kde-kobo/bin symbolink links to system bin for ; qdoc, qhelpconverter, qhelpgenerator, qtattributionsscanner (or modify kobo-qt-setup-scripts to compile them yourself)

```
export QT_VER=5.15.8
export BUILDDIR=/home/aramir/qt-docs #probably doesn't matter actually
cd [...]/kobo-qt-setup-scripts/qt-linux-5.15-kde-kobo
make docs #gonna take about 30 minutes... yes for real...
cd [...]/kobo-qt-setup-scripts/
find . -type f -name "*.qch"
```

Open QtCreator, Tools->Help->Documentation->Add.. add every single generated .qch.
Now when selecting a QtClass in your code and pressing f1 it should display the local doc associated to said QtClass.