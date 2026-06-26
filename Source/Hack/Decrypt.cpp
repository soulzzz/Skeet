#include "Decrypt.h"
#include <DMALibrary/Memory/Memory.h>
#include "common/Data.h"
#include "common/Offset.h"

DWORD Decrypt::CIndex(DWORD value)
{
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
                Sleep(1000);
            }

            int32_t Tmp1Add = mem.Read<uint32_t>(DecryptPtr + 3);
            Ptr = Tmp1Add + DecryptPtr + 7;
            unsigned char ShellcodeBuff[1024] = { NULL };
            ShellcodeBuff[0] = 0x90;
            ShellcodeBuff[1] = 0x90;
            mem.Read(DecryptPtr, &ShellcodeBuff[2], sizeof(ShellcodeBuff) - 2);
            ShellcodeBuff[2] = 0x48;
            ShellcodeBuff[3] = 0x8B;
            ShellcodeBuff[4] = 0xC1;
            ShellcodeBuff[5] = 0x90;
            ShellcodeBuff[6] = 0x90;
            ShellcodeBuff[7] = 0x90;
            ShellcodeBuff[8] = 0x90;
            DecFunction = reinterpret_cast<decltype(DecFunction)> (VirtualAlloc(nullptr, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
            RtlCopyMemory((LPVOID)DecFunction, (LPVOID)ShellcodeBuff, sizeof(ShellcodeBuff));
        }

        return  DecFunction(Ptr, addr);
    }
    catch (const std::exception&)
    {
        return 0;
    }
}
