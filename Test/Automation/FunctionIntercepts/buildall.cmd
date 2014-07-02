@ECHO OFF

ECHO Generating Code...
call .\code_generator\Win32CodeGenerator.exe

cl.exe *.cpp /link ws2_32.lib Lz32.lib Advapi32.lib