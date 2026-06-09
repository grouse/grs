#ifndef WIN32_SHLWAPI_H
#define WIN32_SHLWAPI_H

#include "win32_lite.h"

extern "C" {
    BOOL PathFileExistsA(LPCSTR pszPath);
    int SHCreateDirectoryExA(
        HWND                      hwnd,
        LPCSTR                    pszPath,
        const SECURITY_ATTRIBUTES *psa);
}

#endif // WIN32_SHLWAPI_H
