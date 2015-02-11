cl /nologo /MT /O2 -c -Fosettime.obj -I"%LUA_DEV%\include"  settime.c /D __WINNT__ /D _WIN32 /D _MBCS
link Shlwapi.lib -nologo -subsystem:console -out:"settime.exe" "%LUA_DEV%\lib\lua5.1.lib" settime.obj

mkdir socket
copy "%LUA_DEV%\clibs\socket\core.dll" socket
