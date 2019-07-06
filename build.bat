REM Make sure you run C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat
REM or the script corresponding to your toolchain before trying to build.

cl.exe /nologo /EHsc scancode_viewer.cpp /link User32.lib Gdi32.lib
cl.exe /nologo /EHsc capslock2ctrl.cpp /link User32.lib Advapi32.lib