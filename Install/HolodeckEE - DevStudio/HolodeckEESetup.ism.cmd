REM Save the current search path
Set SavePath=%Path%

attrib -R "c:\sisvn\trunk\commercial\Holodeck\Install\HolodeckEE - DevStudio\Script Files\Setup.rul"

REM Set the search path to find InstallShield's command line tools
Path C:\Progra~1\Common~1\Instal~1\IScript;%PATH%
Path C:\Program Files\InstallShield\DevStudio 9\System;%PATH%

REM Execute the command line compiler
Compile "c:\sisvn\trunk\commercial\Holodeck\Install\HolodeckEE - DevStudio\Script Files\setup.rul" "C:\Program Files\InstallShield\DevStudio 9\Script\Ifx\lib\Ifx.obl" "C:\Program Files\InstallShield\DevStudio 9\Script\isrt\lib\Isrt.obl" -i"C:\Program Files\InstallShield\DevStudio 9\Script\Ifx\Include" -i"C:\Program Files\InstallShield\DevStudio 9\Script\Isrt\Include"> ISBuild.rpt

REM Test for success. Quit if there was a compiler error.
If errorlevel 1 goto CompilerErrorOccurred

REM Build the media
ISbuild -p"c:\sisvn\trunk\commercial\Holodeck\Install\HolodeckEE - DevStudio\HolodeckEESetup.ism" -m"CD-ROM Media" > ISBuild.rpt

REM Test for success. Quit if there was a build error.
If errorlevel 1 goto BuildErrorOccurred

REM Skip over the error handling and exit
Goto Done

REM Report the compiler error; then exit
:CompilerErrorOccurred
Echo Error on compile; media not built. 
Pause Press any key to view report.
Type ISBuild.rpt | More
Goto Done

REM Report the build error; then exit
:BuildErrorOccurred
Echo Error on build; media not built. 
Pause Press any key to view report.
Type ISBuild.rpt | More

:Done
REM Restore the search path
Path=%SavePath%
Set SavePath=

attrib +R "c:\sisvn\trunk\commercial\Holodeck\Install\HolodeckEE - DevStudio\Script Files\Setup.rul"