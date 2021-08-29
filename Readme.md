QtCreator specific setup steps : 
Tools -> Options -> Debugger-> Additional Startup Commands : 
 -q --nh -ex 'set architecture arm'
 
 Projects->Kobo(Kit)->Build->Add Custom Process Step with : 
 Command : %{sourceDir}/OtherFiles/packager.sh.
 Arguments : %{CurrentBuild:Type} %{sourceDir} %{CurrentProject:BuildPath} %{CurrentProject:Name}
 Working Directory : %{sourceDir}
 
  Projects->Kobo(Kit)->Run->Deployment-> Add Run custom remote command : 
  /mnt/onboard/.adds/Obenkyobo/debugEnv.sh &
