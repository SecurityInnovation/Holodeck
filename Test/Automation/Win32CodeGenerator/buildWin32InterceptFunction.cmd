@ECHO OFF

call "%VS71COMNTOOLS%vsvars32.bat"

CL /FeWin32InterceptFunction  *.cpp /OUT:"FunctionIntercepts.exe" /Ot /link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib Lz32.lib 