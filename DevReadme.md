## How to develop for Kobo ?

You can setup a Kobo dev environment by : 
- Looking at the <a href="https://github.com/koreader/koxtoolchain">koxtoolchain instructions</a> and work your way from here. To be honest, I m not totally sure what you'll end up with. No qt that s for sure, some lua support of some sort. Using it alongside some stuff like <a href="https://github.com/NiLuJe/FBInk">FBInk</a> should allow you to print some stuff on screen. To be noted, <a href="https://github.com/baskerville/plato/blob/master/doc/BUILD.md">Plato</a> also has its own toolchain for kobo, 
- If all you're interested in is getting some basic gcc working <a href="https://github.com/NiLuJe/koxtoolchain/"> Niluje's toolchain </a> is the way to go.
- But if you're interested in getting something "to get you coding fast", some Qt binaries along with its <a href="https://github.com/Aryetis/qt5-kobo-platform-plugin"> Qt Platform Abstraction for Kobo</a> and <a href="https://github.com/Aryetis/KoboExtraFunk">KoboExtraFunk library</a> to easily manipulate kobo's specific features (such as wifi, buttons, screen settings, etc...) and a bunch of extra libraries (zlib-ng, libb2, zstd, openssl, pnglib, libjpeg-turbo, expat, pcre, libfreetype and harfbuzz). Then the "easy" route is to use my fork of @Rain92's <a href="https://github.com/Aryetis/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a> to setup everything. This is what we'll discuss with this readme.

## How to setup Obenkyobo dev environment using kobo-qt-setup-scripts ? (as of 5th November 2024, tested with WSL2 Debian Bookworm) 

### 1. Run the following commands :
```bash
# Getting a bunch of stuff necessaries for QtCreator, qt source compilation, additional libraries compilation, deployment, etc
sudo apt-get install build-essential autoconf automake bison flex gawk libtool libtool-bin libncurses-dev curl file git gperf help2man texinfo unzip wget cmake pkg-config python3 mmv lftp clang libclang-dev llvm-dev rsync zip
# Using kobo-qt-setup-scripts to get and install everything
git clone --recurse-submodules git@github.com:Aryetis/kobo-qt-setup-scripts.git # or "git clone --recurse-submodules https://github.com/Aryetis/kobo-qt-setup-scripts.git", if you don't use ssh keys for github
cd kobo-qt-setup-scripts
# Installing Niluje's kobo toolchain
./install_toolchain.sh
# Getting kde's qt, if you want to build qt docs later on, use "koboWithDocs" argument. Adding "experimental" argument will get you the latest version of qt5.15 (not recommended as it will probably break compatibility among all the apps using this workflow)
./get_qt.sh
# Getting and compiling additional libraries, you can also use the "experimental" argument to get the latest libs version (not recommended for the same reasons)
./install_libs.sh
# Setting permanent bash environment stuff
echo $'\n########################################' >> ~/.bashrc
echo 'export PATH="$HOME/x-tools/arm-kobo-linux-gnueabihf/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
# Configuring and building qt-kde, will install everything at $HOME/qt-bin/qt-linux-5.15-kde-kobo/
./build_qt.sh kobo config
./build_qt.sh kobo make
./build_qt.sh kobo install
# Deploy the previously compiled libraries to [kobo-qt-setup-scripts]/deploy/qt-linux-5.15-kde-kobo/lib folder
# Additionaly you can add the libkobo.so path and kobo device's ip to its argument for an immediate deployment over SFTP if needed
./deploy_qt.sh
# Install QtCreator
# Depending of when you re reading this you might want to use an offline installer instead of installing QtCreator from Debian's repositories.
# cf "Setting up gdb" on why that is.
```

That should get you a working cross commpiler and qt binaries configured with a bunch of libraries (most of them only useful for UltimateMangaReader)

### 2. Time to get Obenkyobo's repository : 
- `git clone --recurse-submodules git@github.com:Aryetis/Obenkyobo.git` (or `git clone --recurse-submodules https://github.com/Aryetis/Obenkyobo.git` if you don't use ssh keys for github)
- And then simply open its `ObenkyoboProject.pro` file with QtCreator. Once there you'll have to setup a "kit" using the arm-kobo-linux-gnueabihf-gcc/g++ and qt binaries compiled above. Compilers should be in the `~/x-tools/arm-kobo-linux-gnueabihf` folder and you'll want to add the custom Qt in the "Qt Versions" tab by picking its qmake at `~/qt-bin/qt-linux-5.15-kde-kobo/bin`. "Run device" should be pretty much self explanatory, although now might be a good time to install <a href="https://www.mobileread.com/forums/showthread.php?t=254214">Niluje's kobo stuff</a> to get a working gdb client, rsync, sftp running on your kobo. Gdb can be setup later on (check section "4. Setting up gdb"). Once you've setup everything your kit should look somewhat like this.

<p align="center">
  <img src="DevReadme/KoboQtKit.jpg" width="807" height="528" >
</p>

- You'll have to update the symbolic link at `[Obenkyobo]/Src/Obenkyobo/OtherFiles/Dependencies/qt-linux-5.15-kde-kobo` to point towards `[kobo-qt-setup-scripts]/deploy/qt-linux-5.15-kde-kobo` folder. So we can push/rsync both our program and everything qt related at the press of a single button in QtCreator.
- We also need to indicate to our qtpa that we're using qt-linux-5.15-kde-kobo instead of the usual qt-linux-5.15-kobo. Therefore run this in Obenkyobo's root folder `echo "CUSTOM_QTDIR = /mnt/onboard/.adds/qt-linux-5.15-kde-kobo" > [Obenkyobo]/Src/Libs/qt5-kobo-platform-plugin/koboplatformplugin.pri`
- Next, we need to set what part of the project you actually want to ship. If you're using rsync stick to everything and it will be fine. But if you had to switch to sftp for some reasons then you probably want to pay attention to this. Modify `ObenkyoboProject/Src/Obenkyobo/Obenkyobo.pro` and set the `INSTALLS +=` line to one of the following : 
```
INSTALLS += everything # will ship everything
INSTALLS += everythingButQtLibs  # ship everything but Qt qt-linux-5.15-kde-kobo
INSTALLS += scripts # ship debug scripts, launcher and other scripts  
```

### 3. Setting up QtCreator

Now let's set everything on QtCreator's side (note that some instructions are on a "Per workspace" basis). 
```
Really important, packager.sh scripts expect everything to be there with that exact syntax. 
Settings->Build & Run->Default Build Properties->Default build directory  : 
%{JS: Util.asciify("build-%{Project:Name}-%{Kit:FileSystemName}-%{BuildConfig:Name}")}

Projects->Kobo(Kit)->Run->Deployment->Deploy files and set flags for rsync : --chown=root:root -av
(prior to QtCreator 14.0.1 you can't pass more than one argument without breaking rsync, therefore skip -av)
Projects->Kobo(Kit)->Run->Deployment->Upload files via SFTP instead of rsync

# Killing nickel for our dev session (after your dev session you will have to use start_nickel.sh over ssh or simply reboot the device)
Projects->Kobo(Kit)->Run->Deployment-> Add Run custom remote command with :  
/mnt/onboard/.adds/Obenkyobo/exit_nickel.sh || true

Projects->Kobo(Kit)->Run->Environment->(System Environment)->Add create new variable with at least 
# everything in here is usually set at runtime by Obenkyobo_launcher.sh when running application from device itself. But because we can't source it from QtCreator, we set everything manually in here.
LD_LIBRARY_PATH=/mnt/onboard/.adds/qt-linux-5.15-kde-kobo/lib:/mnt/onboard/.adds/Obenkyobo/lib:
QT_QPA_PLATFORM=kobo   # depending of your device and when you pulled you might want to use this instead "kobo:debug:experimentaltouchhandler"
QT_QPA_EVDEV_DEBUG=true # if you want to debug libkobo.so qpa inputs for instance
```

The preparation of the files for deployment (and creation of a .zip file for release) should be handled by the `Src/Obenkyobo/OtherFiles/packager.sh` (triggered by Obenkyobo.pro's QMAKE_POST_LINK action). Yes, compiling in release should give you a .zip file including everything (program, libs, qt, nm and kfmon entries) that you should be able to release as is for everyone to install in one step. Just unzip it on your device and voilà !  

And that's it! With all of that done, you should now be able to simply click the "Run" button to compile and send eveything necessary to your kobo device over the air. Packager.sh should also retrigger itself when needed automatically (you can always force rebuild to retrigger it manually or call it by yourself). 

If for some reasons you need to debug QtCreator's behavior, just run `export QT_LOGGING_RULES=qtc.*=true` before launching it.

Also to make sure your QtCreator can display japanese text you might have to install a japanese font (eg : `apt install fonts-takao-mincho`)

### 4. Setting up gdb

Additionaly you ll probably want to get a working gdb too.
Two solutions : 
- Use cross compiled arm gdb from <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a>. 
```bash
sudo apt install python3-dev libgmp-dev
cd kobo-qt-setup-scripts
./install_gdb.sh
```
- Install gdb-multiarch, select it in your qt kit and set it up for arm architecture in QtCreator by :  `Tools->Options->Debugger->GDB->Additional Startup Commands : -q --nh -ex 'set architecture arm'`

Please note that there is a <a href="https://bugreports.qt.io/browse/QTCREATORBUG-28505">bug</a> in QtCreator v8 and v9 with gdb's "Debugging Helper". Therefore if like me you re using Debian wsl2, you ll be stuck with a bugged version. Here's three proper solutions :
- Fix the bug manually by following <a href="https://microhobby.com.br/blog/2023/08/08/qt-creator-v9-0-2-debug-helpers-not-working-on-ubuntu-23-04-and-debian-bookworm-12/">this tutorial</a>
- Update to Trixie/unstable which has QtCreator v10.0.2-4. (seems to cause issue with ./install_toolchain.sh ? Will investigate when Trixie is out of testing phase)
- Download standalone QtCreator from <a href="https://download.qt.io/official_releases/qtcreator/">offline installers</a>, in theory it shouldn't mix with system libs and packages... I'm not too fond of it but it looks likes the best bet for now. Here's a bunch of things you'll need to intall to make it work (might be missing some I didn't took notes). `sudo apt install libxkbcommon-x11-0 libxcb-shape0 libxcb-render-util0 libxcb-keysyms1 libxcb-image0 libxcb-icccm4 libdbus-1-dev libwayland-cursor0 libxaw7 libxmu6 libxt6 libxtst6 libxcb-randr0 libxcb-sync1 libxcb-xfixes0 libx11-xcb1 libgl1-mesa-glx mesa-utils libglib2.0-cil libsecret-1-dev` . To create star menu entry/shortcut for it (and others wsl apps) create a file `/usr/share/applications/qtcreator-xxx.desktop` (xxx depending of your qtcreator version) and fill it like below. (And throw the icons from <a href="DevReadme/QtCreatorIcons.zip">QtCreatorIcons.zip archive</a> into `/usr/share/icons/hicolor`). If the shortcut does not work, try correcting its target (in properties, from windows's side) from something like `"C:\Program Files\WSL\wslg.exe" -d Debian --cd "~" -- "~/qtcreator-[XXX].0.0/bin/qtcreator"` to `"C:\Program Files\WSL\wslg.exe" -d Debian -- "/home/[USER]/qtcreator-[XXX].0.0/bin/qtcreator"`. If you also want to run it with some manual DPI scaling use qtcreator.sh instead of qtcreator and prepend `export QT_SCALE_FACTOR=1.3` to it. 
```
[Desktop Entry]
Name=qtcreator
Icon=QtProject-qtcreator
Comment=QtCreator-13.0.0 Qt IDE
Exec="~/qtcreator-13.0.0/bin/qtcreator" %f # You might need to use hardpath here, seems like some wsl does not like using ~/ there
Version=13.0.0
Type=Application
Categories=Development;IDE;
Terminal=false
StartupNotify=true
```

## Setting up the Ereader

I'm assuming that you already have some sort of "App launcher" installed on your ereader, be it <a href="https://www.mobileread.com/forums/showthread.php?t=274231">KFMon</a> or <a href="https://www.mobileread.com/forums/showthread.php?t=329525">NickelMenu</a> and are familiar on how to add entries for them. Please note that <a href="Src/Obenkyobo/OtherFiles/packager.sh">Packager.sh</a> should take care of this for you. If you intend to reuse this repository as a template for your project you will have to change packager.sh's entries.

You ll want to setup a fixed IP address for sure it. Be it to be used with <a href="https://github.com/Rain92/kobo-qt-setup-scripts">kobo-qt-setup-scripts</a>'s deploy_script.sh, or to register your device properly within QtCreator's "run devices"

Is your Ereader rebooting upon Deployment ? That's probably because kfmon/nm is scanning for the freshly installed/deployed stuff and reboots the device to update its nickelMenu entry. You should probably select "INSTALLS += target" in Obenkyobo.pro when working daily. That way it won't resend them everytime.

Note that the wifi indicator on your kobo can sometimes lie to you. To work around this you can either : 
- Turn on developer mode on the kobo by searching for a book named `devmodeon`, then force the wifi to stay on during dev session : Plus->Parameters->Technical informations->Developer options->force Wifi ON. 
- Simply tap the wifi signal icon and keep its wifi list widget open, this should work 99% of the time.

Install <a href="https://www.mobileread.com/forums/showthread.php?t=254214">Niluje's kobo stuff</a> package to get various programs running on the kobo such as : 
- `dropbear` ssh server (connect with root and empty password, yes empty password So just in case, you should probably set one up or keep your wifi turned off. You ve been warned.)
- `nano`, `gdb`, `strace`
- `fbgrab picture.png` to take screenshots
- `rsync`, `sftp` 
- etc 

## Miscellaneous

How to read the backtrace logs : convert address to line using `addr2line -e [NonStrippedProgramBinary] [HexAddress]`

How to snoop in a library : `nm -gD lib.so` `objdump -TC lib.so` `readelf -Ws lib.so` and Ghidra of course

How to double check what kind of file I'm dealing with `file testFile`

How to check input sanity in qpa / LibKoboExtraFunk ? `evdev-dump /dev/input/event0`

Need to scale QtCreator dpi ? put `export QT_SCALE_FACTOR=1.3` in ~/.bashrc 

NEVER modify any of the .sh scripts under windows... Windows end of line will mess things up when ran on linux, or at the very least use `win2unix` afterwards to fix line endings issues.

## How to serial connect to Kobo ereader in WSL2 

1. install usbipd on windows's side to share usb device through IP with WSL2, more info on <a href="https://github.com/dorssel/usbipd-win/wiki/WSL-support">WSL-support's page</a> and <a href="https://learn.microsoft.com/fr-fr/windows/wsl/connect-usb#attach-a-usb-device.">this windows's doc page</a>.
2. install bunch of necessary software in WSL2 with : 
`sudo apt install usbip hwdata usbutils setserial`
3. In some Admin Powershell (urgh), lis usb devices busids with : `usbipd list`
4. link usb device to WSL2 with `usbipd bind --busid=[BUS_ID]` then `usbipd attach --wsl --busid=[BUS_ID]`
5. check with `lsusb` if you're device shows up and if `/dev/ttyUSBxxx` entry is created. If not ... guess what ... WSL2 kernel probably doesn't have your device's driver. To be sure, check it out with `ls -l /sys/bus/usb-serial/drivers`. You'll have to build your own kernel then, cf section below.

## How to build custom WSL2 kernel with Serial module

1. `sudo apt install build-essential flex bison libssl-dev libelf-dev git dwarves bc`
2. `git clone https://github.com/microsoft/WSL2-Linux-Kernel.git`
3. `cd WSL2-Linux-Kernel`
4. `cp Microsoft/config-wsl .config` (might as well change CONFIG_LOCALVERSION in it while you're at it so you can differentiate your kernel later on)
5. `make menuconfig`
6. Navigate to `Device Drivers -> USB Support` and enable the following elements as built-in  (marked with a `*`) :  `Support for Host-side USB`, `USB/IP support`, `VHCI hcd`. 
7. And then in the same menu : `USB Serial Converter support -> [whatever device you're using] + [USB Serial Console device support] + [USB Generic Serial Driver]`. Enable your device driver as built-in too. In my case, the necessary driver is "USB Prolific 2303 Single Port Serial Driver"
8. Exit and save, then `make -j xxx` (xxx being how many cores you want to build with)
9. `cp arch/x86/boot/bzImage /mnt/c/Users/[USER_NAME]/wsl_kernel`
10. Create the following file `C:\Users\<UserName>\.wslconfig with this content` : 
```
[wsl2]
kernel=C:\\Users\\<UserName>\\wsl_kernel\\bzImage
```
10. in powershell/cmd `wsl --shutdown` (just to be sure)
11. relaunch your WSL2 and check out your new kernel with : `uname -a`  (you should see the name you've set up earlier by modifying CONFIG_LOCALVERSION)
12. connect to your kobo with the usual `sudo minicom -D /dev/ttyUSB0` and voila ! (if it's asking for credentials... try admin/admin)

## Which serial port should I solder my UART to debug over wire ?

This <a href="https://web.archive.org/web/20220627124323/http://gethighstayhigh.co.uk/kobo-self-build/">gethighstayhigh blog entry</a> used to host references for a lot of Kobo internal layout. Thanks ! 

But for prosperity I saved all the layout pictures in the  <a href="DevReadme">"DevReadme"</a> folder. And here's what the "serial port" section used to say:
```
Most Kobo's typically have 3 or 4 serial ports, but only one is functional. The serial ports on 6 popular models can be seen in the photographs. Each Serial port has 4 connections,

V         (Power)
Ground
Data in   (or Rx)
Data Out  (or Tx).

Not all instruments require all 4 connections (In our UART debugging case you probably don't need 5v). The ground connection is connected to the wider copper 'ground' of the entire PCB, which means that the heat from a soldering iron is wicked away fast which can compromise a good joint. Instead, it is recommended that the ground is connected to the nearest PCB screw for a more reliable connection.

For the 2020 Kobo Nia. Note that wiring from the 'other’'end (see Clara, below) was susceptible to EMF.

Layout on older Kobo models; the cables colours are: White is V, Yellow is Ground, Black is Data-In/Rx**, Red is Data-Out/Tx**
**at the 'kobo' end
```

## Some Power Consumption measurements (needs to be redone with a proper methodology later on !!!)
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

## How to build and get docs working in QTCreator

- The lazy way, use the compiled .qch (for qt 5.15.15) I've put in the <a href="DevReadme/qt-docs-5.15.15">DevReadme folder</a>
- The <a href="https://wiki.qt.io/Building_Qt_Documentation">official</a> (yet broken) way : 
```
sudo apt install llvm libclang-dev
cd [...]/kobo-qt-setup-scripts/
#add `qttools qtdoc` to MODULES_BASE in get_qt.sh`
./get_qt.sh kobo
./build_qt.sh kobo config (Should state "QtTools\n QtDocs ...... ok" at the end of the summary)
./build_qt.sh kobo make
./build_qt.sh kobo install
cd [...]/kobo-qt-setup-scripts/qt-linux-5.15-kde-kobo
cd [...]/kobo-qt-setup-scripts/qt-linux-5.15-kde-kobo/qttools/src/assistant
make sub-qhelpgenerator
make sub-qhelpgenerator-install_subtargets
cd [...]/kobo-qt-setup-scripts/qt-linux-5.15-kde-kobo/qtdeclarative/src
make sub-qmldevtools
make sub-qmldevtools-install_subtargets
cd [...]/kobo-qt-setup-scripts/qt-linux-5.15-kde-kobo/qttools/src
make sub-qdoc
make sub-qdoc-install_subtargets
cd ~/kobo-qt-setup-scripts/qt-linux-5.15-kde-kobo/qtbase
make docs # it's gonna take about 30 minutes... yes for real...
find ./ -name "*.qch" | grep doc
# Open QtCreator, Edit -> Preferences -> Help -> Documentation and add have fun adding every single .pch listed by the command above :D (qtcore and qtdoc are the two main ones)
```
- The dirty (yet working) way
```
sudo apt install qtbase5-dev qdoc-qt5 qtattributionsscanner-qt5 qhelpgenerator-qt5
ln -s /usr/lib/qt5/bin/qdoc ~/qt-bin/qt-linux-5.15-kde-kobo/bin/qdoc
ln -s /usr/lib/qt5/bin/qtattributionsscanner ~/qt-bin/qt-linux-5.15-kde-kobo/bin/qtattributionsscanner
ln -s /usr/lib/qt5/bin/qhelpgenerator ~/qt-bin/qt-linux-5.15-kde-kobo/bin/qhelpgenerator
cd [...]kobo-qt-setup-scripts/qt-linux-5.15-kde-kobo/qtbase
mkdir LICENSES
touch LICENSES/NONE.txt
cd ..
make docs # it's gonna take about 30 minutes... yes for real...
find ./ -name "*.qch" | grep doc
# Open QtCreator, Edit -> Preferences -> Help -> Documentation and add have fun adding every single .pch listed by the command above :D
# Ok time to clean our mess
rm ~/qt-bin/qt-linux-5.15-kde-kobo/bin/qdoc
rm ~/qt-bin/qt-linux-5.15-kde-kobo/bin/qtattributionsscanner
rm ~/qt-bin/qt-linux-5.15-kde-kobo/bin/qhelpgenerator
```

In QtCreator, Edit->Preferences->Help->Documentation click "Add..." to install all your .qch doc files.

Now when selecting a QtClass in your code and pressing f1 it should display the local doc associated to said QtClass.

Fonts and size fonts randomly reset now ? Yes it's a known bug, <a href="https://bugreports.qt.io/browse/QTCREATORBUG-12271">it's been known for at least 10 years</a>. Simply zooming in/out (with ctrl+mousewheel for instance) should reset everything to its correct state. It's fugly but I'm done jumping through hoops, so I call it "good enough" for now.
