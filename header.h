#include <Windows.h>
#include <winnetwk.h>
#include <stdio.h>
#pragma comment(lib, "mpr.lib")

BOOL WINAPI enumerateResources(LPNETRESOURCE lpnr, FILE *errlog, DWORD dwNesting);