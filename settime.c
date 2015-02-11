#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <time.h>
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

int load(lua_State* L, const char* func, unsigned int* utc) {
    lua_getglobal(L, func);
    if (lua_pcall(L, 0, 1, 0)) {
         printf("Error Msg pcall %s.\n", lua_tostring(L, -1));
         return -1;
    }

    if (!lua_isnumber(L,-1)) {
        printf("time should be a number\n" );
        return -2;
    }

    *utc = lua_tonumber(L,-1);
    lua_pop(L, -1);
    return 0;
}

void TimetToFileTime( time_t t, LPFILETIME pft )
{
    LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
    pft->dwLowDateTime = (DWORD) ll;
    pft->dwHighDateTime = ll >>32;
}

int main()
{
    lua_State* L = luaL_newstate();
    unsigned int utc = 0;
    TCHAR szModulePath[256];

    luaL_openlibs(L);

    GetModuleFileName(NULL, szModulePath, 256);
    PathAppend(szModulePath, _T("..\\"));
    SetCurrentDirectory(szModulePath);

    if (luaL_loadfile(L, "nettime.lua") || lua_pcall(L, 0, 0, 0)) {
        printf("Error Msg load %s.\n", lua_tostring(L, -1));
        return -1;
    }
    do {
        if(load(L,"nettime", &utc) == 0) {
            time_t tt = utc - 2208988800L;
            SYSTEMTIME st;
            FILETIME ft;
            TimetToFileTime(tt, &ft);
            if (FileTimeToSystemTime(&ft, &st))
            {
                printf("Today is: %d-%d-%d %d:%d:%d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
                SetSystemTime(&st);
            }
            break;
        } else {
            puts("No network!");
            Sleep(10000);
        }
    } while (1);
    lua_close(L);
    return 0;
}