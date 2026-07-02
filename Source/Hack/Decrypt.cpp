#include "Decrypt.h"
#include <DMALibrary/Memory/Memory.h>
#include "common/Data.h"
#include "common/Offset.h"
#include <Utils/Utils.h>
#include <iostream>
#include <iomanip>

// 诊断开关:1 输出 Xe/CIndex 诊断日志,0 静默(零开销)。
// 排查"解密是否还有效"时保持 1;确认无误后改 0。
#ifndef XE_LOG
#define XE_LOG 0
#endif

#if XE_LOG
// hex dump 一段字节,用于核对 prologue / patch 后 shellcode 头
static void XeDumpBytes(const char* tag, const uint8_t* p, size_t n)
{
	std::cout << "[Xe] " << tag << ":";
	for (size_t i = 0; i < n; ++i)
		std::cout << " " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)p[i];
	std::cout << std::dec << std::endl;
}
#endif

DWORD Decrypt::CIndex(DWORD value)
{
#if XE_LOG
	// 首次调用时打印常量,确认从 offset map 加载成功(非兜底值 30109433)
	static bool logged = false;
	if (!logged)
	{
		logged = true;
		std::cout << "[CIndex] constants:"
			<< " XorKey1=0x" << std::hex << (DWORD)GameData.Offset["DecryptNameIndexXorKey1"]
			<< " XorKey2=0x" << (DWORD)GameData.Offset["DecryptNameIndexXorKey2"]
			<< " XorKey3=0x" << (DWORD)GameData.Offset["DecryptNameIndexXorKey3"]
			<< " Ror=" << std::dec << (DWORD)GameData.Offset["DecryptNameIndexRor"]
			<< " Rval=" << (DWORD)GameData.Offset["DecryptNameIndexRval"]
			<< " Sval=" << (DWORD)GameData.Offset["DecryptNameIndexSval"]
			<< " Dval=" << (DWORD)GameData.Offset["DecryptNameIndexDval"] << std::endl;
	}
#endif

	// 解密 ObjID / NameIndex —— 对应 Xe Decrypt+explain.md 的 xenuine_DecryptCIndex
	if (value == 0)
		return 0;

	DWORD xorKey1 = (DWORD)GameData.Offset["DecryptNameIndexXorKey1"];
	DWORD xorKey2 = (DWORD)GameData.Offset["DecryptNameIndexXorKey2"];
	DWORD xorKey3 = (DWORD)GameData.Offset["DecryptNameIndexXorKey3"];
	DWORD ror     = (DWORD)GameData.Offset["DecryptNameIndexRor"];
	DWORD rval    = (DWORD)GameData.Offset["DecryptNameIndexRval"];
	DWORD sval    = (DWORD)GameData.Offset["DecryptNameIndexSval"];
	DWORD dval    = (DWORD)GameData.Offset["DecryptNameIndexDval"];

	DWORD xor1  = value ^ xorKey1;
	DWORD part1 = (xor1 << dval) | ((xor1 >> sval) & xorKey3);
	DWORD part2 = ror ? _rotr(xor1, rval) : _rotl(xor1, rval);

	return part1 ^ part2 ^ xorKey2;
}


void Decrypt::DestroyXe()
{
#if XE_LOG
	std::cout << "[Xe] DestroyXe() — tearing down decrypt shellcode (DecFunction=" << (void*)DecFunction << ")." << std::endl;
#endif
	DecFunction = reinterpret_cast<uint64_t(*)(uint64_t key, uint64_t base)>(0);
}

uint64_t Decrypt::Xe(uint64_t addr)
{
	try
	{
		static uint64_t Ptr = 0;
		if (DecFunction == nullptr)
		{
			int64_t UWorld = 0x0;
			int64_t DecryptPtr = 0x0;
			while (!UWorld || !DecryptPtr)
			{
				DecryptPtr = mem.Read<uint64_t>((uint64_t)GameData.GameBase + GameData.Offset["XenuineDecrypt"]);
				UWorld = mem.Read<uint64_t>(GameData.GameBase + GameData.Offset["UWorld"]);
#if XE_LOG
				std::cout << "[Xe] waiting for game... UWorld=0x" << std::hex << UWorld
					<< " DecryptPtr=0x" << DecryptPtr << std::dec << std::endl;
#endif
				Sleep(1000);
			}
#if XE_LOG
			std::cout << "[Xe] ready. GameBase=0x" << std::hex << GameData.GameBase
				<< " UWorld=0x" << UWorld
				<< " DecryptPtr=0x" << DecryptPtr << std::dec << std::endl;
#endif

			// dump prologue,核对是否仍是 48 8D 05 ?? ?? ?? ?? (LEA RAX,[rip+disp32])
			uint8_t prologue[16] = { 0 };
			mem.Read(DecryptPtr, prologue, sizeof(prologue));
#if XE_LOG
			XeDumpBytes("prologue @DecryptPtr", prologue, sizeof(prologue));
			if (prologue[0] != 0x48 || prologue[1] != 0x8D || prologue[2] != 0x05)
				std::cout << "[Xe] WARNING: prologue 不是 '48 8D 05'(LEA RAX,[rip+disp32])!patch 大概率误解析。" << std::endl;
#endif

			int32_t Tmp1Add = mem.Read<int32_t>(DecryptPtr + 3);
			Ptr = Tmp1Add + DecryptPtr + 7;
#if XE_LOG
			std::cout << "[Xe] LEA disp32=" << Tmp1Add << " (0x" << std::hex << (uint32_t)Tmp1Add
				<< ") 重建密钥 Ptr=0x" << Ptr << std::dec << std::endl;
#endif

			unsigned char ShellcodeBuff[1024] = { NULL };
			ShellcodeBuff[0] = 0x90;
			ShellcodeBuff[1] = 0x90;
			mem.Read(DecryptPtr, &ShellcodeBuff[2], sizeof(ShellcodeBuff) - 2);

#if XE_LOG
			// 扫描第一个 RET(0xC3),验证例程在 1022 字节内能正常结束
			int retOff = -1;
			for (int i = 2; i < (int)sizeof(ShellcodeBuff); ++i)
				if (ShellcodeBuff[i] == 0xC3) { retOff = i - 2; break; }
			if (retOff < 0)
				std::cout << "[Xe] WARNING: 1022 字节内未找到 RET(0xC3)!例程可能被截断或为 VM 分发。" << std::endl;
			else
				std::cout << "[Xe] 例程内首个 RET 偏移 = 0x" << std::hex << retOff << std::dec << std::endl;

			// 启发式:统计裸 E8/E9 字节命中(相对 call/jmp 的线索,需反汇编确认)
			int relHits = 0;
			for (int i = 2; i < (int)sizeof(ShellcodeBuff); ++i)
				if (ShellcodeBuff[i] == 0xE8 || ShellcodeBuff[i] == 0xE9) ++relHits;
			std::cout << "[Xe] 缓冲区内 E8/E9 裸字节命中 = " << relHits
				<< " (非0表示可能含相对 call/jmp,在新地址执行会错位;需反汇编核实)" << std::endl;
#endif

			ShellcodeBuff[2] = 0x48;
			ShellcodeBuff[3] = 0x8B;
			ShellcodeBuff[4] = 0xC1;
			ShellcodeBuff[5] = 0x90;
			ShellcodeBuff[6] = 0x90;
			ShellcodeBuff[7] = 0x90;
			ShellcodeBuff[8] = 0x90;
			DecFunction = reinterpret_cast<decltype(DecFunction)>(VirtualAlloc(nullptr, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
			if (!DecFunction)
			{
				Utils::LogThrottled("Decrypt.Xe.VirtualAllocFailed", 5000, 2, "Decrypt shellcode allocation failed");
				return 0;
			}

			RtlCopyMemory((LPVOID)DecFunction, (LPVOID)ShellcodeBuff, sizeof(ShellcodeBuff));
			Utils::Log(1, "Decrypt shellcode initialized");

#if XE_LOG
			std::cout << "[Xe] VirtualAlloc -> 0x" << std::hex << (void*)DecFunction;
			if (!DecFunction) std::cout << " (FAILED!)";
			std::cout << std::dec << std::endl;
			XeDumpBytes("patched shellcode head (NOP NOP | 48 8B C1 | NOP*4 | ...)", ShellcodeBuff, 16);
			std::cout << "[Xe] 解密 shellcode 已安装。" << std::endl;
#endif
		}

#if XE_LOG
		// 采样前 5 次调用,看返回值是否像合法指针(高位非0、落在游戏堆区间)
		static int sampleCalls = 0;
		if (sampleCalls < 5)
		{
			++sampleCalls;
			uint64_t result = DecFunction(Ptr, addr);
			std::cout << "[Xe] call#" << sampleCalls << " addr=0x" << std::hex << addr
				<< " key=0x" << Ptr << " -> result=0x" << result << std::dec << std::endl;
			return result;
		}
#endif
		return  DecFunction(Ptr, addr);
	}
	catch (const std::exception& e)
	{
		Utils::LogThrottled("Decrypt.Xe.StdException", 3000, 2, "Decrypt Xe exception: %s", e.what());
#if XE_LOG
		std::cout << "[Xe] EXCEPTION(std): " << e.what()
			<< " (addr=0x" << std::hex << addr << std::dec << ")" << std::endl;
#endif
		return 0;
	}
	catch (...)
	{
		Utils::LogThrottled("Decrypt.Xe.UnknownException", 3000, 2, "Decrypt Xe unknown exception");
#if XE_LOG
		// 走到这说明执行 shellcode 抛了非 std 异常;若进程直接 AV 崩溃,则说明拷贝的例程字节已坏。
		std::cout << "[Xe] EXCEPTION(unknown) (addr=0x" << std::hex << addr << std::dec << ")" << std::endl;
#endif
		return 0;
	}
}
