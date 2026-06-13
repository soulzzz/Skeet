#pragma once

#include "Common/Data.h"

class MeshPatcher {
public:
    static MeshPatcher* GetInstance() {
        static MeshPatcher instance;
        return &instance;
    }

    void PatchMesh(TriangleMeshData& mesh) {
        (void)mesh;
    }

    MeshPatcher(const MeshPatcher&) = delete;
    MeshPatcher& operator=(const MeshPatcher&) = delete;

private:
    MeshPatcher() = default;
};
