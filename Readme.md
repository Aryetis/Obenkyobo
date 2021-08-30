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
/mnt/onboard/.adds/Obenkyobo/debugEnv.sh & > /mnt/onboard/.adds/Obenkyobo/log.txt 2>&1
(for both Release and debug)

-----------

Projects->Kobo(Kit)->Run->Environment->(System Environment)->Add create new variable with at least 
QT_QPA_PLATFORM=kobo
(for both Release and debug)


Should create everyfile needed and send them over to the kobo, stop nickel and start the application upon clicking the run button. Ok bye !
