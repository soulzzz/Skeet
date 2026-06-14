#pragma once

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <Common/Data.h>
#include <DMALibrary/Memory/Memory.h>
#include <thread>
#include <Common/Constant.h>
#include <Utils/Utils.h>
#include <Utils/Throttler.h>
#include <Common/Entitys.h>
#include <Common/Offset.h>
#include <Common/VectorHelper.h>
#include <cstdint>

#include <Utils/Engine.h>
#include <Utils/ue4math/rotator.h>
#include <Utils/ue4math/transform.h>
#include <Utils/Timer.h>
#include <Hack/KeyState.h>
#include <Hack/Decrypt.h>
#include <Hack/GNames.h>
#include <Hack/AimBot.h>
#include <Hack/Players.h>
#include <Hack/Actors.h>
#include <Hack/Vehicles.h>
#include <Hack/Items.h>
#include <Hack/Projects.h>
#include <Hack/Radar.h>
#include <Hack/Process.h>
#include <Common/Config.h>
#include <Hack/VisibleCheck.h>
#include <Utils/KmBox.h>
#include <Utils/KmBoxNet.h>
#include <Utils/Lurker.h>
#include <Utils/MoBox.h>
#include "autoRecoil.h"
#include <Hack/Segment.h>
#include <Hack/VisibleCheck.h>
static std::string getCurrentTime2()
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm* now_tm = std::localtime(&now_time);

	std::ostringstream oss1;
	oss1 << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
	return oss1.str();
}

class Hack
{
public:

	static void Update()
	{
		auto hScatter = mem.CreateScatterHandle();
		while (true)
		{
			mem.RefreshTLB();

			GameData.UWorld = Decrypt::Xe(mem.Read<uint64_t>(GameData.GameBase + GameData.Offset["UWorld"]));
			GameData.GameInstance = Decrypt::Xe(mem.Read<uint64_t>(GameData.UWorld + GameData.Offset["GameInstance"]));
			GameData.GNames = GNames::GetGNamesPtr();

			if (Utils::ValidPtr(GameData.UWorld) || GameData.Scene != Scene::Gaming)
			{
				Sleep(GameData.ThreadSleep);
				continue;
			}

			GameData.GameState = Decrypt::Xe(mem.Read<uint64_t>(GameData.UWorld + GameData.Offset["GameState"]));
			GameData.LocalPlayer = Decrypt::Xe(mem.Read<uint64_t>(mem.Read<uint64_t>(GameData.GameInstance + GameData.Offset["LocalPlayer"])));
			GameData.PlayerController = Decrypt::Xe(mem.Read<uint64_t>(GameData.LocalPlayer + GameData.Offset["PlayerController"]));
			GameData.AcknowledgedPawn = Decrypt::Xe(mem.Read<uint64_t>(GameData.PlayerController + GameData.Offset["AcknowledgedPawn"]));
			GameData.CurrentLevel = Decrypt::Xe(mem.Read<uint64_t>(GameData.UWorld + GameData.Offset["CurrentLevel"]));
			GameData.ActorArray = Decrypt::Xe(mem.Read<uint64_t>(GameData.CurrentLevel + GameData.Offset["Actors"]));

			uint64_t PlayerCameraManager;
			uint64_t MyHUD;
			BYTE bShowMouseCursor;
			uint64_t PlayerInput;
			uint64_t AntiCheatCharacterSyncManager;
			uint64_t CacheCameraViewTarget;

			mem.AddScatterRead(hScatter, GameData.PlayerController + GameData.Offset["PlayerCameraManager"], &PlayerCameraManager);
			mem.AddScatterRead(hScatter, GameData.PlayerController + GameData.Offset["MyHUD"], &MyHUD);
			mem.AddScatterRead(hScatter, GameData.PlayerController + GameData.Offset["bShowMouseCursor"], &bShowMouseCursor);
			mem.AddScatterRead(hScatter, GameData.PlayerController + GameData.Offset["PlayerInput"], &PlayerInput);
			mem.AddScatterRead(hScatter, GameData.PlayerController + GameData.Offset["AntiCheatCharacterSyncManager"], &AntiCheatCharacterSyncManager);
			mem.AddScatterRead(hScatter, GameData.PlayerCameraManager + GameData.Offset["ViewTarget"], &CacheCameraViewTarget);
			mem.ExecuteReadScatter(hScatter);

			GameData.PlayerCameraManager = PlayerCameraManager;
			GameData.MyHUD = MyHUD;
			GameData.bShowMouseCursor = bShowMouseCursor == 0x25 ? true : false;
			GameData.PlayerInput = PlayerInput;
			GameData.AntiCheatCharacterSyncManager = AntiCheatCharacterSyncManager;

			//GameData.ActorArrayEmpty = mem.Read<uint64_t>(GameData.LocalPlayerInfo.Entity + GameData.Offset["ActorChildren"]);

			if (!Utils::ValidPtr(CacheCameraViewTarget)) {
				int ViewTargetID = Decrypt::CIndex(mem.Read<int>(CacheCameraViewTarget + GameData.Offset["ObjID"]));
				auto ViewTargetEntityInfo = Data::GetGNameListsByIDItem(ViewTargetID);
				if (ViewTargetEntityInfo.Type == EntityType::Player || ViewTargetEntityInfo.Type == EntityType::AI)
				{
					int TeamID = mem.Read<int>(CacheCameraViewTarget + GameData.Offset["LastTeamNum"]);
					GameData.CameraViewTarget = CacheCameraViewTarget;
					GameData.LocalPlayerTeamID = (TeamID >= 100000) ? (TeamID - 100000) : TeamID;
				}
			}

			Players::UpdatePlayerLists();
			Radar::UpdateWorldMapInfo();

			Sleep(300);
		}
		mem.CloseScatterHandle(hScatter);
	}
	static void ReleaseLoadedModel() {
		// Release loaded scenes in a predictable order.

		// Release dynamic load scene.
		{

			//std::lock_guard<std::mutex> lock(GameData.dynamicLoadMutex);
			try {
				auto dynamicLoadScene = GameData.DynamicLoadScene;
				// Validate the pointer before deleting.
				if (dynamicLoadScene != nullptr) {
					delete dynamicLoadScene;
					GameData.DynamicLoadScene = nullptr;
#ifdef _PHYSX_DEBUG
					Utils::Log(1, "Released dynamic load scene");
#endif
				}
			}
			catch (const std::exception& e) {
				Utils::Log(3, "Failed to release dynamic load scene: %s", e.what());
			}
		}

		// Release height field scene.
		{

			//std::lock_guard<std::mutex> lock(GameData.heightFieldMutex);
			try {
				auto heightFieldScene = GameData.HeightFieldScene;
				// Validate the pointer before deleting.
				if (heightFieldScene != nullptr) {
					delete heightFieldScene;
					GameData.HeightFieldScene = nullptr;
#ifdef _PHYSX_DEBUG
					Utils::Log(1, "Released height field scene");
#endif
				}
			}
			catch (const std::exception& e) {
				Utils::Log(3, "Failed to release height field scene: %s", e.what());
			}
		}

		// Release dynamic rigid scene.
		{

			// std::lock_guard<std::mutex> lock(GameData.globalMutex);
			try {
				auto globalScene = GameData.DynamicRigidScene;
				// Validate the pointer before deleting.
				if (globalScene != nullptr) {
					delete globalScene;
					GameData.DynamicRigidScene = nullptr;
#ifdef _PHYSX_DEBUG
					Utils::Log(1, "Released dynamic rigid scene");
#endif
				}
			}
			catch (const std::exception& e) {
				Utils::Log(3, "Failed to release rigid scene: %s", e.what());
			}
		}
	}

	static void StartLoadMapModel() {
		static std::mutex gameDataMutex;
		//ReleaseLoadedModel();
		Sleep(GameData.ThreadSleep);
		Utils::Log(1, "Starting map model load...");

		// Create the dynamic load scene.
		{
			std::lock_guard<std::mutex> lock(gameDataMutex);
			GameData.DynamicLoadScene = new Physics::VisibleScene<PrunerPayload, PrunerPayloadHash>(Physics::prunerPayloadExtractor);
			std::thread LoadDynamicHeightFieldThread(VisibleCheck::UpdateDynamicHeightField);
			LoadDynamicHeightFieldThread.detach();
		}
		// Create the height field scene.
		{
			std::lock_guard<std::mutex> lock(gameDataMutex);
			GameData.HeightFieldScene = new Physics::VisibleScene<uint64_t, Int64Hash>(Physics::int64Extractor);
			std::thread LoadDynamicRigidThread(VisibleCheck::UpdateDynamicRigid);
			LoadDynamicRigidThread.detach();

		}
		// Create the dynamic rigid scene.
		{
			std::lock_guard<std::mutex> lock(gameDataMutex);
			GameData.DynamicRigidScene = new Physics::VisibleScene<PrunerPayload, PrunerPayloadHash>(Physics::prunerPayloadExtractor);
			std::thread LoadSceneByRangeThread(VisibleCheck::UpdateSceneByRange);

			LoadSceneByRangeThread.detach();
		}

	}
	static void UpdateCamera()
	{
		Throttler Throttlered;
		auto hScatter = mem.CreateScatterHandle();
		CameraData Camera;
		float DeltaSeconds = 0.f;
		float TimeSeconds = 0.f;

		while (true)
		{
			if (GameData.Scene != Scene::Gaming || !GameData.Config.Overlay.UseThread)
			{
				Sleep(GameData.ThreadSleep);
				continue;
			}

			if (!GameData.Config.Overlay.UseLastFrameCameraCache)
			{
				mem.AddScatterRead(hScatter, GameData.PlayerCameraManager + GameData.Offset["CameraCacheLocation"], &Camera.Location);
				mem.AddScatterRead(hScatter, GameData.PlayerCameraManager + GameData.Offset["CameraCacheRotation"], &Camera.Rotation);
				mem.AddScatterRead(hScatter, GameData.PlayerCameraManager + GameData.Offset["CameraCacheFOV"], &Camera.FOV);
			}
			mem.AddScatterRead(hScatter, GameData.UWorld + GameData.Offset["TimeSeconds"], &TimeSeconds);
			mem.ExecuteReadScatter(hScatter);

			GameData.Camera = Camera;
			GameData.WorldTimeSeconds = TimeSeconds;
		}
		mem.CloseScatterHandle(hScatter);
	}

	static void UpdatePID()
	{
		Throttler Throttlered;

		bool GameProcessFound = false;
		bool PrevGameProcessFound = false;

		while (true)
		{
			Throttlered.executeTask("UpdatePID", std::chrono::milliseconds(1500), [&GameProcessFound, &PrevGameProcessFound] {
				DWORD PID = mem.GetTslGamePID();
				if (PID == 0)
				{
					mem.RefreshAll();
					GameData.Scene = Scene::FindProcess;
					GameProcessFound = false;
					GameData.GameBase = 0;
					GameData.PID = 0;
					Decrypt::DestroyXe();

					if (GameProcessFound != PrevGameProcessFound) {
						Utils::Log(2, "The game process has ended");
					}
				}
				else {
					GameProcessFound = true;
					if (GameProcessFound != PrevGameProcessFound) {
						GameData.HookBase = mem.GetHookModuleBase();
						Utils::Log(1, "Successfully found PlayerUnknown's Battlegrounds game progress [%p]", GameData.PID);
						EntityInit();
						Data::SetGNameLists(EntityLists);
						Data::SetGNameListsByID({});
					}
				}
				PrevGameProcessFound = GameProcessFound;
				});

			

			if (GameData.PID != 0) {
				uint64_t currentLevelPtr = Decrypt::Xe(mem.Read<uint64_t>(GameData.UWorld + GameData.Offset["CurrentLevel"]));
				uint64_t gameInstancePtr = Decrypt::Xe(mem.Read<uint64_t>(GameData.UWorld + GameData.Offset["GameInstance"]));
				uint64_t localPlayerArray = mem.Read<uint64_t>(gameInstancePtr + GameData.Offset["LocalPlayer"]);
				uint64_t localPlayerPtr = Decrypt::Xe(mem.Read<uint64_t>(localPlayerArray));
				uint64_t playerControllerPtr = Decrypt::Xe(mem.Read<uint64_t>(localPlayerPtr + GameData.Offset["PlayerController"]));
				uint64_t acknowledgedPawnPtr = Decrypt::Xe(mem.Read<uint64_t>(playerControllerPtr + GameData.Offset["AcknowledgedPawn"]));

				const int worldId1 = Decrypt::CIndex(mem.Read<int>(GameData.UWorld + GameData.Offset["ObjID"]));
				const int worldId2 = Decrypt::CIndex2(mem.Read<int>(GameData.UWorld + GameData.Offset["ObjID2"]));
				int worldId = worldId1;
				int levelId = 0;
				std::string worldName = GNames::GetNameByID(worldId1);
				std::string worldName2 = GNames::GetNameByID(worldId2);
				std::string levelName = "fail";
				std::string levelName2 = "fail";

				if (!Utils::ValidPtr(currentLevelPtr)) {
					const int levelId1 = Decrypt::CIndex(mem.Read<int>(currentLevelPtr + GameData.Offset["ObjID"]));
					const int levelId2 = Decrypt::CIndex2(mem.Read<int>(currentLevelPtr + GameData.Offset["ObjID2"]));
					levelId = levelId1;
					levelName = GNames::GetNameByID(levelId1);
					levelName2 = GNames::GetNameByID(levelId2);

					if (levelName == "fail" && levelName2 != "fail") {
						levelId = levelId2;
						levelName = levelName2;
					}
				}

				if (worldName == "fail" && worldName2 != "fail") {
					worldId = worldId2;
					worldName = worldName2;
				}

				GameData.MapName = (levelName != "fail" && !levelName.empty()) ? levelName : worldName;

				static int lastLoggedWorldId = 0;
				static int lastLoggedWorldId2 = 0;
				static int lastLoggedLevelId = 0;
				static std::string lastLoggedWorldName2;
				static std::string lastLoggedLevelName2;
				static std::string lastLoggedMapName;

				if (worldId != lastLoggedWorldId || worldId2 != lastLoggedWorldId2 || levelId != lastLoggedLevelId || GameData.MapName != lastLoggedMapName || worldName2 != lastLoggedWorldName2 || levelName2 != lastLoggedLevelName2) {
					Utils::Log(0, "Scene probe: UWorld=%p CurrentLevel=%p AckPawn=%p WorldID1=%d WorldID2=%d LevelID=%d WorldName1=[%s] WorldName2=[%s] LevelName1=[%s] LevelName2=[%s] ActiveMap=[%s]",
						GameData.UWorld,
						currentLevelPtr,
						acknowledgedPawnPtr,
						worldId1,
						worldId2,
						levelId,
						GNames::GetNameByID(worldId1).c_str(),
						worldName2.c_str(),
						levelName.c_str(),
						levelName2.c_str(),
						GameData.MapName.c_str());
					lastLoggedWorldId = worldId;
					lastLoggedWorldId2 = worldId2;
					lastLoggedLevelId = levelId;
					lastLoggedWorldName2 = worldName2;
					lastLoggedLevelName2 = levelName2;
					lastLoggedMapName = GameData.MapName;
				}

				const bool hasPawn = !Utils::ValidPtr(acknowledgedPawnPtr);
				const bool hasMapName = !GameData.MapName.empty() && GameData.MapName != "fail";

				if (hasMapName) {
					GameData.Scene = Utils::IsLobby(GameData.MapName) ? Scene::Lobby : Scene::Gaming;
				}
				else if (hasPawn) {
					GameData.Scene = Scene::Gaming;
				}
				else {
					GameData.Scene = Scene::Lobby;
				}
			}

			if (GameData.Scene != GameData.PreviousScene) {
				GameData.PreviousScene = GameData.Scene;
				switch (GameData.Scene) {
				case Scene::FindProcess:
					break;
				case Scene::Lobby:
					//ReleaseLoadedModel();
					break;
				case Scene::Gaming:
	
					Utils::Log(0, "Entered the game scene, World name is [%s]", GameData.MapName.c_str());
					Players::ReadPlayerLists();
					//StartLoadMapModel();
					
					break;
				default:
					break;
				}
			}

			Sleep(1500);
		}
	}

	static void WriteFunction()
	{
		auto hScatter = mem.CreateScatterHandle();
		while (true)
		{
			if (GameData.Scene != Scene::Gaming)
			{
				Sleep(GameData.ThreadSleep);
				continue;
			}

			if (!Utils::ValidPtr(GameData.GameInstance))
			{
				Utils::Log(1, "GameData.GameInstance: %p", GameData.GameInstance);
				//Utils::Log(1, "Read: %d", mem.Read<int>(GameData.GameInstance + 0x17fd));
				int Value = 2;
				mem.AddScatterRead(hScatter, GameData.GameInstance + 0x17fd, (int*)&Value);
				mem.ExecuteWriteScatter(hScatter);
				//Utils::Log(1, "Read: %d", mem.Read<int>(GameData.GameInstance + 0x17fd));
			}

			Sleep(100);

			
		}

		mem.CloseScatterHandle(hScatter);
	}

	static void Init()
	{
		Offset::Sever_Init();

		EntityInit();

		for (const auto& item : EntityLists)
		{
			if (item.second.Type == EntityType::Item)
			{
				ItemDetail Detail;
				Detail.Name = item.first;
				Detail.DisplayName = item.second.DisplayName;
				Detail.Type = item.second.WeaponType;

				if (Detail.Type == WeaponType::AR || Detail.Type == WeaponType::DMR || Detail.Type == WeaponType::SR)
				{
					Detail.Group = 1;
				}
				else if (Detail.Type == WeaponType::Sight)
				{
					Detail.Group = 2;
				}
				else if (Detail.Type == WeaponType::Drug)
				{
					Detail.Group = 4;
				}
				else if (Detail.Type == WeaponType::Armor)
				{
					Detail.Group = 3;
				}

				GameData.Config.Item.Lists[item.first] = Detail;
			}
		}

		Config::Load();

		if (!mem.Init("TslGame.exe", true, false))
		{
			Utils::Log(2, U8("init error! No get TslGame.exe read pid faild"));
		
			//exit(0);
		}
		else {
			Utils::Log(1, U8("init success"));
		}
	

		KeyState::Init();

		Data::SetGNameLists(EntityLists);
		Data::SetGNameListsByID({});

		std::thread UpdatePIDThread(UpdatePID);
		std::thread UpdateThread(Update);
		std::thread UpdateKeyStateThread(KeyState::Update);
		std::thread UpdateActorsThread(Actors::Update);
		std::thread UpdatePlayersThread(Players::Update);
		std::thread UpdateVehiclesThread(Vehicles::Update);
		std::thread AimBotRunThread(AimBot::Run);
		std::thread UpdateCameraThread(UpdateCamera);
		std::thread UpdateItemsThread(Items::Update);
		std::thread UpdateProjectsThread(Projects::Update);
		std::thread UpdateRadarThread(Radar::Update);
		std::thread UpdateRecoilThread(Recoil::autoRecoil);
		//std::thread UpdateRankThread(Segment::Update);
		
		//std::thread WriteFunctionThread(WriteFunction);


		UpdatePIDThread.join();
	}
};
