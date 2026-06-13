#pragma once

#include <embree4/rtcore.h>
#include <embree4/rtcore_ray.h>

#include <Hack/Physx.h>

#include <algorithm>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>
#include <vector>

namespace Physics {

using namespace physx;
using namespace std;

static auto prunerPayloadExtractor = [](const TriangleMeshData& mesh) -> PrunerPayload {
    return mesh.UniqueKey1;
};

static auto int64Extractor = [](const TriangleMeshData& mesh) -> uint64_t {
    return mesh.UniqueKey2;
};

static auto normal64Extractor = [](const TriangleMeshData& mesh) -> uint64_t {
    return static_cast<uint64_t>(rand());
};

template <typename T, typename Hash>
class VisibleScene
{
public:
    using KeyExtractor = T(*)(const TriangleMeshData&);

    explicit VisibleScene(KeyExtractor keyExtractor)
        : getKey(keyExtractor) {
    }

    ~VisibleScene() = default;

    void UpdateMesh(const vector<TriangleMeshData>& willAddMeshs, const std::set<T>& removeKeys)
    {
        lock_guard<mutex> lock(dataMutex);

        if (!removeKeys.empty()) {
            meshDatas.erase(
                remove_if(meshDatas.begin(), meshDatas.end(),
                    [&](const shared_ptr<TriangleMeshData>& mesh) {
                        return removeKeys.find(getKey(*mesh)) != removeKeys.end();
                    }),
                meshDatas.end());

            for (auto it = keyToIndex.begin(); it != keyToIndex.end();) {
                if (removeKeys.find(it->first) != removeKeys.end()) {
                    it = keyToIndex.erase(it);
                } else {
                    ++it;
                }
            }
        }

        for (const auto& mesh : willAddMeshs) {
            auto copy = make_shared<TriangleMeshData>(mesh);
            keyToIndex[getKey(*copy)] = static_cast<uint32_t>(meshDatas.size());
            meshDatas.push_back(copy);
        }
    }

    RTCRayHit Raycast(FVector& origin, FVector& target)
    {
        (void)origin;
        (void)target;

        RTCRayHit rayhit{};
        rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
        return rayhit;
    }

    TriangleMeshData* GetGeomeoryData(uint32_t geomId)
    {
        lock_guard<mutex> lock(dataMutex);
        if (geomId >= meshDatas.size()) {
            return nullptr;
        }
        return meshDatas[geomId].get();
    }

    vector<TriangleMeshData*> GetNearMesh(FVector position, double radiusSquare) const
    {
        lock_guard<mutex> lock(dataMutex);
        vector<TriangleMeshData*> result;

        for (const auto& meshData : meshDatas) {
            const auto& meshCenter = meshData->Transform.mPosition;
            Vector3 pos = { position.X, position.Y, position.Z };
            if ((meshCenter - pos).Length2DSquare() <= radiusSquare) {
                result.push_back(meshData.get());
            }
        }

        return result;
    }

private:
    KeyExtractor getKey;
    mutable mutex dataMutex;
    vector<shared_ptr<TriangleMeshData>> meshDatas;
    unordered_map<T, uint32_t, Hash> keyToIndex;
};

} // namespace Physics
