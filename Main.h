
#pragma once
#include <winsock2.h>
#include <windows.h>
#include <Overlay/Overlay.h>
#include <Common/Data.h>
#include <Utils/Utils.h>
#include <Hack/Hack.h>
#include <Common/Offset.h>
#include <thread>
#include <DMALibrary/Memory/Memory.h>
#include <cstdint>
#include <Common/Config.h>

#include <Utils/MachineCodeGenerator.h>

#include <string>
#include <random>
#include <iterator>
#include <algorithm>
#include <cstring>

//
#include "VMP/VMProtectSDK.h"
#include <Utils/w3c/w3c.h>
extern int currentLanguageIndex;  // Global selection index.
enum class Language1
{
    Chinese,
    English
};

extern Language1 currentLanguage1;
inline std::string kamisj = "";
using namespace std;
static std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);
    return strTo;
}

wchar_t s_buffer[33];
WNDPROC OldEditProc;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// 锟斤拷锟斤拷锟缴拷谋锟斤拷锟斤拷裕锟斤拷锟缴拷谋锟斤拷锟斤拷锟缴拷锟斤拷锟斤拷锟?
WORD redTextAttr = FOREGROUND_RED | FOREGROUND_INTENSITY;
WORD greenTextAttr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
// 锟斤拷锟斤拷默锟斤拷锟侥憋拷锟斤拷锟皆ｏ拷锟斤拷色锟侥憋拷锟斤拷锟斤拷色锟斤拷锟斤拷锟斤拷
WORD defaultTextAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static std::wstring stringToWString(std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}
// 锟斤拷示锟斤拷锟截斤拷锟斤拷锟斤拷锟侥猴拷锟斤拷
static void ShowProgress(HWND hwnd, DWORD totalSize, DWORD downloadedSize) {
    int progress = static_cast<int>((static_cast<float>(downloadedSize) / totalSize) * 100);
    std::wstring progressText = L"锟斤拷锟截斤拷锟斤拷: " + std::to_wstring(progress) + L"%";

    // 锟斤拷锟铰达拷锟节憋拷锟斤拷锟斤拷示锟斤拷锟截斤拷锟斤拷
    SetWindowTextW(hwnd, progressText.c_str()); // 锟斤拷锟斤拷锟斤拷锟节达拷锟节憋拷锟斤拷锟斤拷锟斤拷示锟斤拷锟斤拷
}
// 锟斤拷锟斤拷锟侥硷拷锟斤拷锟斤拷示锟斤拷锟斤拷
static bool DownloadNewVersionWithProgress(HWND hwnd, const std::string& downloadUrl, const std::string& savePath) {
    (void)hwnd;
    (void)downloadUrl;
    (void)savePath;
    return true;
}

static bool IsRunningAsAdmin()
{
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;

    // 锟斤拷取锟斤拷锟斤拷员锟斤拷锟?SID
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&NtAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &adminGroup))
    {
        // 锟斤拷榈鼻帮拷锟斤拷锟斤拷欠锟斤拷锟斤拷锟斤拷锟斤拷员 SID
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }

    return isAdmin;
}





const char* FILE_NAME = "kami.txt"; // Local cache file.
static void SaveKami(const char* kami) {
    std::ofstream outfile(FILE_NAME);
    if (outfile.is_open()) {
        outfile << kami;
        outfile.close();
    }
}

static void LoadKami(char* kami, int size) {
    std::ifstream infile(FILE_NAME);
    if (infile.is_open()) {
        infile.getline(kami, size);
        infile.close();
    }
}
HWND hwndEdit;
HBRUSH hbrBackground;
HBRUSH hbrBtnBackground;
HBRUSH hbrStaticBackground;
HBRUSH hbrEditBackground;
HWND hwndLanguageComboBox; // Language combobox handle.
HWND hwndStatic;
HWND hLoginButton;
HWND hUnbindButton;
HWND Language23;










// 锟斤拷锟侥硷拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
static int LoadLanguageSetting() {
    FILE* file = fopen("language_setting.txt", "r");
    int languageIndex = 0; // 默锟斤拷锟斤拷锟斤拷
    if (file) {
        fscanf(file, "%d", &languageIndex); // 锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷
        fclose(file);
    }
    return languageIndex;
}




static void SetFontForButton(HWND hButton, const wchar_t* fontName, int fontSize) {
    LOGFONTW logFont = { 0 };
    logFont.lfHeight = fontSize;
    wcscpy_s(logFont.lfFaceName, LF_FACESIZE, fontName);
    HFONT hFont = CreateFontIndirectW(&logFont);
    SendMessageW(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

}


static void ConvertWCharArrayToCharArray(wchar_t* wstr, char* str, int strSize) {
    // Convert a wide-character string to UTF-8.
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (size_needed == 0 || size_needed > strSize) {
        std::cerr << "Please restart the application." << std::endl;
        return;
    }

    // Perform the conversion.
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, strSize, NULL, NULL);
}
// Replacement window procedure for key filtering.

static std::string ConvertUTF8toGBK(const std::string& strUTF8) {
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wstr, len);

    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len];
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);

    std::string strGBK(str);
    delete[] wstr;
    delete[] str;
    return strGBK;
}


// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷时锟斤拷锟街凤拷锟斤拷转锟斤拷为time_t锟斤拷锟斤拷
time_t StringToTimeT(const std::string& dateTimeStr) {
    struct tm tm = { 0 }; // 锟斤拷始锟斤拷
    // 锟斤拷锟斤拷锟斤拷锟斤拷时锟斤拷锟绞轿?YYYY-MM-DD HH:MM:SS
    sscanf_s(dateTimeStr.c_str(), "%d-%d-%d %d:%d:%d",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
        &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    tm.tm_year -= 1900; // tm_year 锟角达拷1900锟疥开始锟斤拷锟斤拷
    tm.tm_mon -= 1; // tm_mon 锟角达拷0锟斤拷始锟斤拷锟斤拷
    return mktime(&tm); // 锟斤拷锟斤拷time_t时锟斤拷
}



