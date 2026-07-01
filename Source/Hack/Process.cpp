#include "Process.h"
#include <DMALibrary/Memory/Memory.h>
#include <Utils/Utils.h>
#include <Common/Data.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <new>

HANDLE Process::Handle = INVALID_HANDLE_VALUE;
uintptr_t Process::ImageBase = 0;
uint32_t Process::ImageSize = 0;
uint32_t Process::ID = 0;
uint8_t* Process::Dump = nullptr;

bool Process::Init(const std::wstring Process_name)
{
	if (Utils::ValidPtr(GameData.GameBase) || Utils::ValidPtr(GameData.UWorld) || Utils::ValidPtr(GameData.ActorArray)) return false;
	GameData.VirtualProtect = false;

	IMAGE_DOS_HEADER DosHeader = mem.Read<IMAGE_DOS_HEADER>(GameData.GameBase);
	if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE || DosHeader.e_lfanew <= 0 || DosHeader.e_lfanew > 0x1000)
	{
		Utils::Log(2, "Invalid DOS header while dumping image, base=0x%llX", static_cast<unsigned long long>(GameData.GameBase));
		return false;
	}

	IMAGE_NT_HEADERS NtHeader = mem.Read<IMAGE_NT_HEADERS>((GameData.GameBase + DosHeader.e_lfanew));
	if (NtHeader.Signature != IMAGE_NT_SIGNATURE)
	{
		Utils::Log(2, "Invalid NT header while dumping image, base=0x%llX", static_cast<unsigned long long>(GameData.GameBase));
		return false;
	}

	//Utils::Log(1, "DosHeader: %p", DosHeader);
	//Utils::Log(1, "NtHeader.e_lfanew: %p", DosHeader.e_lfanew);
	//Utils::Log(1, "NtHeader: %p", NtHeader);

	ImageSize = NtHeader.OptionalHeader.SizeOfImage;
	if (ImageSize == 0)
	{
		Utils::Log(2, "Invalid image size while dumping image");
		return false;
	}

	delete[] Dump;
	Dump = new (std::nothrow) uint8_t[Process::ImageSize];
	if (Dump == nullptr)
	{
		Utils::Log(2, "Failed to allocate image dump, size=%u", Process::ImageSize);
		return false;
	}

	if (!mem.Read(GameData.GameBase, (void*)(Dump), ImageSize))
	{
		Utils::Log(2, "Failed to read image dump, base=0x%llX size=%u", static_cast<unsigned long long>(GameData.GameBase), ImageSize);
		delete[] Dump;
		Dump = nullptr;
		return false;
	}

	DWORD OldProtect;
	if (!VirtualProtect(Dump, Process::ImageSize, PAGE_EXECUTE_READWRITE, &OldProtect))
	{
		Utils::Log(2, "VirtualProtect failed for image dump, error=%lu", GetLastError());
		delete[] Dump;
		Dump = nullptr;
		return false;
	}

	GameData.VirtualProtect = true;
	Utils::Log(1, "Image dump ready, size=%u", Process::ImageSize);

	return true;
}
