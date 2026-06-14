#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <thread>
#include <unordered_map>

#include <Common/Data.h>
#include <DMALibrary/Memory/Memory.h>
#include <Hack/Decrypt.h>
#include <Hack/GNames.h>
#include <Common/VectorHelper.h>

class Players
{
private:
    static constexpr size_t kMaxBoneReadCount = 190;

    static int ResolveSkeletonBoneIndex(EGender gender, int bone)
    {
        if (gender != EGender::Female)
        {
            return bone;
        }

        switch (bone)
        {
        case EBoneIndex::Thigh_L:
            return 180;
        case EBoneIndex::Calf_L:
            return 183;
        case EBoneIndex::Foot_L:
            return 181;
        case EBoneIndex::Ball_L:
            return 182;
        case EBoneIndex::Thigh_R:
            return 186;
        case EBoneIndex::Calf_R:
            return 189;
        case EBoneIndex::Foot_R:
            return 187;
        case EBoneIndex::Ball_R:
            return 188;
        default:
            return bone;
        }
    }

    static uint64_t ResolveCandidatePointer(uint64_t value)
    {
        if (!Utils::ValidPtr(value))
        {
            return value;
        }

        uint64_t decrypted = Decrypt::Xe(value);
        if (!Utils::ValidPtr(decrypted))
        {
            return decrypted;
        }

        return 0;
    }

    static uint64_t ResolveSceneComponent(uint64_t entity, uint64_t offset, bool useTransformCheck)
    {
        uint64_t rawPtr = mem.Read<uint64_t>(entity + offset);
        uint64_t decryptedPtr = Decrypt::Xe(rawPtr);

        auto isUsable = [&](uint64_t ptr) -> bool
        {
            if (Utils::ValidPtr(ptr))
            {
                return false;
            }

            if (useTransformCheck)
            {
                FTransform transform = mem.Read<FTransform>(ptr + GameData.Offset["ComponentToWorld"]);
                return IsReasonableLocation(transform.Translation);
            }

            FVector location = mem.Read<FVector>(ptr + GameData.Offset["ComponentLocation"]);
            return IsReasonableLocation(location);
        };

        if (isUsable(rawPtr))
        {
            return rawPtr;
        }

        if (isUsable(decryptedPtr))
        {
            return decryptedPtr;
        }

        if (!Utils::ValidPtr(rawPtr))
        {
            return rawPtr;
        }

        if (!Utils::ValidPtr(decryptedPtr))
        {
            return decryptedPtr;
        }

        return 0;
    }

    static int NormalizeTeamId(int teamId)
    {
        return teamId >= 100000 ? (teamId - 100000) : teamId;
    }

    static bool IsReasonableLocation(const FVector& location)
    {
        return std::isfinite(location.X) &&
            std::isfinite(location.Y) &&
            std::isfinite(location.Z) &&
            std::abs(location.X) < 2000000.0f &&
            std::abs(location.Y) < 2000000.0f &&
            std::abs(location.Z) < 200000.0f;
    }

    static bool ReadBoneTransforms(Player& player, const std::string& entityName)
    {
        uint64_t boneArray = ResolveCandidatePointer(mem.Read<uint64_t>(player.MeshComponent + GameData.Offset["StaticMesh"]));
        if (Utils::ValidPtr(boneArray))
        {
            return false;
        }

        player.BoneArray = boneArray;
        const bool isFemale = entityName.find("Female") != std::string::npos;
        const EGender gender = isFemale ? EGender::Female : EGender::Male;
        const size_t boneCount = isFemale ? 190 : 183;
        if (boneCount == 0 || boneCount > kMaxBoneReadCount)
        {
            return false;
        }

        std::array<FTransform, kMaxBoneReadCount> boneBuffer{};
        mem.Read((uintptr_t)(boneArray + sizeof(FTransform)), boneBuffer.data(), static_cast<DWORD>(sizeof(FTransform) * boneCount));

        for (int bone : BoneIndex)
        {
            const int sourceBone = ResolveSkeletonBoneIndex(gender, bone);
            if (sourceBone <= 0 || static_cast<size_t>(sourceBone) > boneCount)
            {
                continue;
            }

            player.Skeleton.Bones[bone] = boneBuffer[sourceBone - 1];
        }

        return true;
    }

    static bool BuildPlayer(uint64_t entity, Player& player, bool isLocalPlayer, const std::string& entityName = "")
    {
        if (Utils::ValidPtr(entity))
        {
            return false;
        }

        player = Player{};
        player.Entity = entity;
        player.IsMe = isLocalPlayer;
        player.Type = EntityType::Player;
        player.State = CharacterState::Alive;
        player.Health = 100.0f;
        player.GroggyHealth = 100.0f;
        player.Distance = 0.0f;
        player.Name = "Unknown";
        std::string resolvedEntityName = entityName;
        if (resolvedEntityName.empty())
        {
            const int objId = Decrypt::CIndex(mem.Read<int>(entity + GameData.Offset["ObjID"]));
            resolvedEntityName = GNames::GetNameByID(objId);
            player.ObjID = objId;
        }

        player.EntityName = resolvedEntityName;
        if (resolvedEntityName.find("Female") != std::string::npos)
        {
            player.Gender = EGender::Female;
        }
        else
        {
            player.Gender = EGender::Male;
        }

        player.RootComponent = ResolveSceneComponent(entity, GameData.Offset["RootComponent"], false);
        player.MeshComponent = ResolveSceneComponent(entity, GameData.Offset["Mesh"], true);
        if (Utils::ValidPtr(player.RootComponent) || Utils::ValidPtr(player.MeshComponent))
        {
            return false;
        }

        player.TeamID = NormalizeTeamId(mem.Read<int>(entity + GameData.Offset["LastTeamNum"]));
        player.ComponentToWorld = mem.Read<FTransform>(player.MeshComponent + GameData.Offset["ComponentToWorld"]);
        player.Location = player.ComponentToWorld.Translation;

        if (!IsReasonableLocation(player.Location))
        {
            FVector rootLocation = mem.Read<FVector>(player.RootComponent + GameData.Offset["ComponentLocation"]);
            if (!IsReasonableLocation(rootLocation))
            {
                return false;
            }

            player.Location = rootLocation;
            player.ComponentToWorld.Translation = rootLocation;
        }

        player.LastSubmitTime = mem.Read<float>(player.MeshComponent + GameData.Offset["LastSubmitTime"]);
        player.LastRenderTimeOnScreen = mem.Read<float>(player.MeshComponent + GameData.Offset["LastRenderTimeOnScreen"]);
        player.IsVisible = player.LastRenderTimeOnScreen + 0.06f >= player.LastSubmitTime;
        player.bVisible = player.IsVisible ? 1 : 0;

        FText characterName = mem.Read<FText>(entity + GameData.Offset["CharacterName"]);
        if (characterName.buffer[0] != '\0')
        {
            player.Name = characterName.buffer;
        }

        if (!ReadBoneTransforms(player, resolvedEntityName))
        {
            return false;
        }

        player.Skeleton.LocationBones[EBoneIndex::Head] = VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[EBoneIndex::Head], player.ComponentToWorld);
        player.Skeleton.LocationBones[EBoneIndex::Root] = player.Location;
        player.Skeleton.LocationBones[EBoneIndex::ForeHead] = VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[EBoneIndex::ForeHead], player.ComponentToWorld);
        player.Skeleton.LocationBones[EBoneIndex::Foot_L] = VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[EBoneIndex::Foot_L], player.ComponentToWorld);
        player.Skeleton.LocationBones[EBoneIndex::Foot_R] = VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[EBoneIndex::Foot_R], player.ComponentToWorld);
        player.Skeleton.LocationBones[EBoneIndex::Pelvis] = VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[EBoneIndex::Pelvis], player.ComponentToWorld);

        player.Skeleton.ScreenBones[EBoneIndex::Head] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[EBoneIndex::Head]);
        player.Skeleton.ScreenBones[EBoneIndex::Root] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[EBoneIndex::Root]);
        player.Skeleton.ScreenBones[EBoneIndex::ForeHead] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[EBoneIndex::ForeHead]);
        player.Skeleton.ScreenBones[EBoneIndex::Foot_L] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[EBoneIndex::Foot_L]);
        player.Skeleton.ScreenBones[EBoneIndex::Foot_R] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[EBoneIndex::Foot_R]);
        player.Skeleton.ScreenBones[EBoneIndex::Pelvis] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[EBoneIndex::Pelvis]);

        player.InScreen = VectorHelper::IsInScreen(player.Skeleton.ScreenBones[EBoneIndex::Head]) &&
            VectorHelper::IsInScreen(player.Skeleton.ScreenBones[EBoneIndex::Root]);

        return player.TeamID > 0 || isLocalPlayer;
    }

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
        if (GameData.Scene != Scene::Gaming || Utils::ValidPtr(GameData.AcknowledgedPawn))
        {
            return;
        }

        Player localPlayer;
        if (BuildPlayer(GameData.AcknowledgedPawn, localPlayer, true))
        {
            GameData.LocalPlayerInfo = localPlayer;
            GameData.LocalPlayerTeamID = localPlayer.TeamID;
        }
        else
        {
            static DWORD lastLocalLogTick = 0;
            DWORD now = GetTickCount();
            if (now - lastLocalLogTick >= 2000)
            {
                uint64_t rawRoot = mem.Read<uint64_t>(GameData.AcknowledgedPawn + GameData.Offset["RootComponent"]);
                uint64_t rawMesh = mem.Read<uint64_t>(GameData.AcknowledgedPawn + GameData.Offset["Mesh"]);
                uint64_t decRoot = Decrypt::Xe(rawRoot);
                uint64_t decMesh = Decrypt::Xe(rawMesh);
                Utils::Log(2, "Local player probe failed: pawn=%p rawRoot=%p decRoot=%p rawMesh=%p decMesh=%p team=%d",
                    GameData.AcknowledgedPawn,
                    rawRoot,
                    decRoot,
                    rawMesh,
                    decMesh,
                    mem.Read<int>(GameData.AcknowledgedPawn + GameData.Offset["LastTeamNum"]));
                lastLocalLogTick = now;
            }
        }
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
            if (GameData.Scene != Scene::Gaming || Utils::ValidPtr(GameData.ActorArray))
            {
                Sleep(100);
                continue;
            }

            UpdatePlayerLists();

            std::unordered_map<uint64_t, Player> cachePlayers;
            TArray<uint64_t> actorArray = mem.Read<TArray<uint64_t>>(GameData.ActorArray);

            int scannedActors = 0;
            int candidatePlayers = 0;

            for (uint64_t entity : actorArray.GetVector())
            {
                ++scannedActors;

                if (Utils::ValidPtr(entity) || entity == GameData.AcknowledgedPawn)
                {
                    continue;
                }

                Player player;
                if (!BuildPlayer(entity, player, false))
                {
                    continue;
                }

                ++candidatePlayers;
                player.IsMyTeam = GameData.LocalPlayerTeamID > 0 && player.TeamID == GameData.LocalPlayerTeamID;
                player.Distance = GameData.LocalPlayerInfo.Location.Distance(player.Location) / 100.0f;
                cachePlayers[player.Entity] = player;
            }

            Data::SetCachePlayers(cachePlayers);
            Data::SetPlayersData(cachePlayers);
            Data::SetPlayers(cachePlayers);

            static DWORD lastLogTick = 0;
            DWORD now = GetTickCount();
            if (now - lastLogTick >= 2000)
            {
                Utils::Log(0, "Player update: ActorArray=%p scanned=%d players=%d localPawn=%p localTeam=%d localPos=(%.1f, %.1f, %.1f)",
                    GameData.ActorArray,
                    scannedActors,
                    candidatePlayers,
                    GameData.AcknowledgedPawn,
                    GameData.LocalPlayerTeamID,
                    GameData.LocalPlayerInfo.Location.X,
                    GameData.LocalPlayerInfo.Location.Y,
                    GameData.LocalPlayerInfo.Location.Z);
                lastLogTick = now;
            }

            Sleep(45);
        }
    }
};
