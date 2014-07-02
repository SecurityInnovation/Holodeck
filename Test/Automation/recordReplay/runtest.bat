@ECHO OFF

%RECORDEDSESSIONLOCATION%="C:\Recorded Sessions"

REM Erase the existing recorded sessions file...
echo erase "%RECORDEDSESSIONLOCATION%\*" /q /f
erase "%RECORDEDSESSIONLOCATION%\*" /q /f

echo CoreTester.exe Original\test%1.xml
CoreTester.exe Original\test%1.xml

REM Copy the recorded session file to a local folder...
copy "%RECORDEDSESSIONLOCATION%\Recorded Session #1.xml" ".\Results\test%1.xml"

REM Run the RecOmp.exe comparison application...
echo %ERRORLEVEL% = RecOmp.exe -f Original\test%1.xml Results\test%1.xml > log%1.txt
%ERRORLEVEL% = call ( RecOmp.exe -f Original\test%1.xml Results\test%1.xml > log%1.txt )

