#pragma once

#include <string>

class WebRadar
{
public:
    static bool sendHttpPostRequest(const std::wstring& ip, int port, const std::wstring& path, const std::string& data) {
        (void)ip;
        (void)port;
        (void)path;
        (void)data;
        return false;
    }

    static void Rundata() {
    }
};
