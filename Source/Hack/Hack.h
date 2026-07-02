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
#include "Webpageradar.h"
#include "autoRecoil.h"
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
		// 順序刪除場景

		// 刪除 Dynamic Load Scene
		{

			//std::lock_guard<std::mutex> lock(GameData.dynamicLoadMutex);
			try {
				auto dynamicLoadScene = GameData.DynamicLoadScene;
				// 檢查指標是否有效
				if (dynamicLoadScene != nullptr) {
					delete dynamicLoadScene;
					GameData.DynamicLoadScene = nullptr; // 確保指標清空
#ifdef _PHYSX_DEBUG
					Utils::Log(1, "释放静态加载场景");
#endif
				}
			}
			catch (const std::exception& e) {
				Utils::Log(3, "Failed to release dynamic load scene: %s", e.what());
			}
		}

		// 刪除 Height Field Scene
		{

			//std::lock_guard<std::mutex> lock(GameData.heightFieldMutex);
			try {
				auto heightFieldScene = GameData.HeightFieldScene;
				// 檢查指標是否有效
				if (heightFieldScene != nullptr) {
					delete heightFieldScene;
					GameData.HeightFieldScene = nullptr; // 確保指標清空
#ifdef _PHYSX_DEBUG
					Utils::Log(1, "释放高度场场景");
#endif
				}
			}
			catch (const std::exception& e) {
				Utils::Log(3, "Failed to release height field scene: %s", e.what());
			}
		}

		// 刪除 Dynamic Rigid Scene
		{

			// std::lock_guard<std::mutex> lock(GameData.globalMutex);
			try {
				auto globalScene = GameData.DynamicRigidScene;
				// 檢查指標是否有效
				if (globalScene != nullptr) {
					delete globalScene;
					GameData.DynamicRigidScene = nullptr; // 確保指標清空
#ifdef _PHYSX_DEBUG
					Utils::Log(1, "释放动态加载场景");
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
		// 调用释放模型的函数
		//ReleaseLoadedModel();
		Sleep(GameData.ThreadSleep);
		// 记录日志，表示开始加载地图模型...
		Utils::Log(1, "开始加载地图模型...");

		// 加互斥锁，确保创建新场景时的线程安全
		{
			std::lock_guard<std::mutex> lock(gameDataMutex);
			// 创建动态加载场景，使用 PrunerPayload 和 PrunerPayloadHash
			GameData.DynamicLoadScene = new Physics::VisibleScene<PrunerPayload, PrunerPayloadHash>(Physics::prunerPayloadExtractor);

			// 创建并启动线程以更新动态高度场
			std::thread LoadDynamicHeightFieldThread(VisibleCheck::UpdateDynamicHeightField);
			LoadDynamicHeightFieldThread.detach();
		}
		// 加互斥锁，确保创建新场景时的线程安全
		{
			std::lock_guard<std::mutex> lock(gameDataMutex);


			// 创建高度场场景，使用 uint64_t 和 Int64Hash
			GameData.HeightFieldScene = new Physics::VisibleScene<uint64_t, Int64Hash>(Physics::int64Extractor);
			// 创建并启动线程以更新动态刚体
			std::thread LoadDynamicRigidThread(VisibleCheck::UpdateDynamicRigid);
			LoadDynamicRigidThread.detach();

		}
		// 加互斥锁，确保创建新场景时的线程安全
		{
			std::lock_guard<std::mutex> lock(gameDataMutex);


			// 创建动态刚体场景，使用 PrunerPayload 和 PrunerPayloadHash
			GameData.DynamicRigidScene = new Physics::VisibleScene<PrunerPayload, PrunerPayloadHash>(Physics::prunerPayloadExtractor);
			// 创建并启动线程以按范围更新场景
			std::thread LoadSceneByRangeThread(VisibleCheck::UpdateSceneByRange);

			LoadSceneByRangeThread.detach();
		}

	}
	static void UpdateCamera()
	{
		auto hScatter = mem.CreateScatterHandle();
		CameraData Camera;
		float TimeSeconds = 0.f;

		while (true)
		{
			if (GameData.Scene != Scene::Gaming || !GameData.Config.Overlay.UseThread)
			{
				Sleep(GameData.ThreadSleep);
				continue;
			}

			if (!GameData.PlayerCameraManager || !GameData.UWorld)
			{
				Sleep(1);
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

			Sleep(1);
		}
		mem.CloseScatterHandle(hScatter);
	}

	static void UpdatePID()
	{
		Throttler Throttlered;

		bool GameProcessFound = false;
		bool PrevGameProcessFound = false;
		int LastMapID = -1;
		std::string LastMapName;

		while (true)
		{
			Throttlered.executeTask("UpdatePID", std::chrono::milliseconds(1500), [&GameProcessFound, &PrevGameProcessFound, &LastMapID, &LastMapName] {
				DWORD PID = mem.GetTslGamePID();
				if (PID == 0)
				{
					mem.RefreshAll();
					GameData.Scene = Scene::FindProcess;
					GameProcessFound = false;
					GameData.GameBase = 0;
					GameData.PID = 0;
					Decrypt::DestroyXe();
					LastMapID = -1;
					LastMapName.clear();

					if (GameProcessFound != PrevGameProcessFound) {
						Utils::Log(2, "The game process has ended");
					}
				}
				else {
					GameProcessFound = true;
					if (GameProcessFound != PrevGameProcessFound) {
						GameData.HookBase = mem.GetHookModuleBase();
						Utils::Log(1, "Game process found: pid=%lu base=0x%llX", GameData.PID, static_cast<unsigned long long>(GameData.GameBase));
						EntityInit();
						Data::SetGNameLists(EntityLists);
						Data::SetGNameListsByID({});
					}
				}
				PrevGameProcessFound = GameProcessFound;
				});

			

			if (GameData.PID != 0) {
				if (Utils::ValidPtr(GameData.UWorld)) {
					Sleep(1500);
					continue;
				}

				int MapID = Decrypt::CIndex(mem.Read<uint64_t>(GameData.UWorld + GameData.Offset["ObjID"]));
				const std::string MapName = GNames::GetNameByID(MapID);
				if (MapID != LastMapID || MapName != LastMapName) {
					Utils::Log(1, "Map changed: id=%d name=%s", MapID, MapName.c_str());
					LastMapID = MapID;
					LastMapName = MapName;
				}

				GameData.MapName = MapName;
				if (Utils::IsLobby(GameData.MapName)) {
					GameData.Scene = Scene::Lobby;
					/*if (MapID == 446822) {
						GameData.Scene = Scene::Lobby;
					}
					else {
						GameData.Scene = Scene::Gaming;
					}*/
				}
				else {
					GameData.Scene = Scene::Gaming;
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
	
					Utils::Log(1, "Entered gaming scene: %s", GameData.MapName.c_str());
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
		Offset::Init();

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
		std::thread UpdatePlayerFogsThread(Players::UpdateFogPlayers);
		std::thread UpdateVehiclesThread(Vehicles::Update);
		std::thread AimBotRunThread(AimBot::Run);
		std::thread UpdateCameraThread(UpdateCamera);
		std::thread UpdateItemsThread(Items::Update);
		std::thread UpdateProjectsThread(Projects::Update);
		std::thread UpdateRadarThread(Radar::Update);
		//std::thread UpdateWebRadarThread(WebRadar::Rundata);
		std::thread UpdateRecoilThread(Recoil::autoRecoil);
		Utils::Log(1, "Worker threads started: pid/core/keys/actors/players/fog/vehicles/aimbot/camera/items/projects/radar/recoil");

		//std::thread WriteFunctionThread(WriteFunction);


		UpdatePIDThread.join();
	}
};
