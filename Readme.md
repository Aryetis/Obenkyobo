 Projects->Kobo(Kit)->Build->Add Custom Process Step with : 
 Command : %{sourceDir}/OtherFiles/packager.sh.
 Arguments : %{CurrentBuild:Type} %{sourceDir} %{CurrentProject:BuildPath} %{CurrentProject:Name}
 Working Directory : %{sourceDir}
 
Projects->Kobo(Kit)->Run->Deployment-> Add Run custom remote command : 
/mnt/onboard/.adds/Obenkyobo/debugEnv.sh &

Should create everyfile needed and send them over to the kobo, stop nickel and start the application upon clicking the run button. Ok bye !