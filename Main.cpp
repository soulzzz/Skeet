#define _PHYSX_DEBUG
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
#include <Utils/w3c/w3c.h>
#include <Utils/MachineCodeGenerator.h>
//#include "QPVerify.h"
#include "resource.h"
#include"Main.h"
FGameData GameData;
using namespace std;
void SetConsoleStyle()
{
	SetConsoleOutputCP(CP_UTF8);

	SetConsoleTitle(L"AKM");

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	if (!(dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
	{
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}
}

int DENGLU;

static int Refresh()
{
	while (true)
	{
		mem.RefreshAll();

		Sleep(1000 * 60 * 15);
	}
}






BOOL WINAPI ConsoleHandler(DWORD event) {
	if (event == CTRL_CLOSE_EVENT) {
		HWND Progman = FindWindowA("Progman", NULL);
		HWND TrayWnd = FindWindowA("Shell_TrayWnd", NULL);
		ShowWindow(Progman, SW_SHOW);
		ShowWindow(TrayWnd, SW_SHOW);
		return TRUE;
	}
	return FALSE;
}

bool DeleteFile(const std::string& file_path) {
	std::wstring wfile_path = std::wstring(file_path.begin(), file_path.end());
	return DeleteFileW(wfile_path.c_str()) != 0;
}

void OpenConsole() {
	// Allocate a fresh console window.
	AllocConsole();

	// Redirect stdio to the console.
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

	//std::cout << "Console window opened." << std::endl;
}

bool DownloadFileWithProgress(const std::string& url, const std::string& output_file) {
	(void)url;
	(void)output_file;
	return true;
}
// Convert a wide string to a multibyte string.
std::string WideStringToMultiByte(const std::wstring& wideString) {
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wideString[0], (int)wideString.size(), NULL, 0, NULL, NULL);
	std::string multiByteString(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wideString[0], (int)wideString.size(), &multiByteString[0], size_needed, NULL, NULL);
	return multiByteString;
}

std::wstring convertToWString(const std::string& str) {
	int len;
	int strLength = (int)str.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), strLength, 0, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), strLength, &wstr[0], len);
	return wstr;
}

bool ExtractZipArchive(const std::string& zip_file) {
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, currentDir);

	std::wstring outputDir = currentDir;
	std::wstring wzip_file = std::wstring(zip_file.begin(), zip_file.end());
	std::wstring command = L"powershell.exe -Command \"Expand-Archive -Path '" + wzip_file + L"' -DestinationPath '" + outputDir + L"'\"";

	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = L"open";
	sei.lpFile = L"powershell.exe";

	sei.lpParameters = command.c_str();
	sei.nShow = SW_HIDE;

	if (!ShellExecuteEx(&sei)) {
		std::cerr << "Failed to execute PowerShell command. Error: " << GetLastError() << std::endl;
		return false;
	}

	WaitForSingleObject(sei.hProcess, INFINITE);

	DWORD exitCode;
	if (!GetExitCodeProcess(sei.hProcess, &exitCode)) {
		std::cerr << "Failed to get exit code of PowerShell process. Error: " << GetLastError() << std::endl;
		CloseHandle(sei.hProcess);
		return false;
	}

	CloseHandle(sei.hProcess);

	return (exitCode == 0);
}

// Set console text color.
void SetConsoleColor(WORD color) {
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleOutput, color);
}
// Draw the menu and highlight the selected item.
void DisplayMenu(const std::vector<std::string>& menuItems, int selectedItem, const std::string g) {
	system("cls");
	printf("%s\n", g.c_str());
	for (int i = 0; i < menuItems.size(); ++i) {
		if (i == selectedItem) {
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else {
			SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
		printf("%s\n", menuItems[i].c_str());
	}
}

int main() {

	if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE))
	{
		return -1;
	}

	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL)
	{
		ShowWindow(hwnd, SW_SHOW);//SW_SHOW显示   SW_HIDE隐藏
	}

	

	//Utils::Log(0, U8("AKM-----助手加载成功......"));

	GameData.Config.Window.IsLogin = true;//

	Config::Load();

	std::thread HackThread(Hack::Init);

	Overlay::Init(0, DrawMain);
	while (true)
	{
		if (GameData.Config.Window.Main) {
			Sleep(500);
			continue;
		}
		break;
	}



	
	return 0;
}

