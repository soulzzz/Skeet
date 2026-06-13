#pragma once

#include <Common/Data.h>
#include <Common/Entitys.h>

namespace LineTrace
{
    static bool LineTraceSingle(FVector TraceStart, FVector TraceEnd)
    {
        (void)TraceStart;
        (void)TraceEnd;
        return true;
    }

    static TriangleMeshData* getNextHint()
    {
        return nullptr;
    }
}
