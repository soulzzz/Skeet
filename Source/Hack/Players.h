#pragma once

#include <string>
#include <thread>

class Players
{
public:
    static void AddToBlackList(const std::string& name)
    {
        (void)name;
    }

    static void AddToWhiteList(const std::string& name)
    {
        (void)name;
    }

    static void RemoveList(const std::string& name)
    {
        (void)name;
    }

    static void ReadPlayerLists()
    {
    }

    static void UpdatePlayerLists()
    {
    }

    static void UpdateFogPlayers()
    {
        while (true) {
            Sleep(1000);
        }
    }

    static void Update()
    {
        while (true) {
            Sleep(1000);
        }
    }
};
