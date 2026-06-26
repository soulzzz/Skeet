#pragma once
#include <cstdint>
#include "common/Data.h"
#include <string>

namespace Offset
{
	// =========================================================================
	//  硬编码偏移常量
	//  原 http://localhost/offset.txt 网络获取逻辑已移除,所有偏移在此集中维护。
	//  更新时直接改本文件对应的 constexpr 即可。
	// =========================================================================
	namespace C
	{
		// ── Dynamic globals ──
		constexpr uint64_t XenuineDecrypt                   = 0x10EA8D28;
		constexpr uint64_t UWorld                           = 0x12AAFD88;
		constexpr uint64_t GNames                           = 0x12D3B030;
		constexpr uint64_t GNamesPtr                        = 0x10;
		constexpr uint64_t ChunkSize                        = 0x3E4C;
		constexpr uint64_t GObjects                         = 0x12CF81A8;
		constexpr uint64_t Unreal_Engine                    = 0x125D9810;

		// ── Struct offsets (SIG-based) ──
		constexpr uint64_t CurrentLevel                     = 0x800;
		constexpr uint64_t Actors                           = 0x38;
		constexpr uint64_t GameInstance                     = 0x3B0;
		constexpr uint64_t LocalPlayer                      = 0xF0;
		constexpr uint64_t AcknowledgedPawn                 = 0x4A8;
		constexpr uint64_t PlayerCameraManager              = 0x4D0;
		constexpr uint64_t WeaponProcessor                  = 0x968;
		constexpr uint64_t EquippedWeapons                  = 0x208;
		constexpr uint64_t Mesh                             = 0x4A0;
		constexpr uint64_t AnimScriptInstance               = 0xE30;
		constexpr uint64_t LastTeamNum                      = 0x2B18;
		constexpr uint64_t GroggyHealth                     = 0x1530;
		constexpr uint64_t ComponentToWorld                 = 0x320;
		constexpr uint64_t RecoilADSRotation_CP             = 0x824;
		constexpr uint64_t CharacterName                    = 0x1E00;
		constexpr uint64_t LeanLeftAlpha_CP                 = 0x69C;
		constexpr uint64_t LeanRightAlpha_CP                = 0x6A0;
		constexpr uint64_t AntiCheatCharacterSyncManager    = 0x1368;
		constexpr uint64_t DormantCharacterClientList       = 0x78;
		constexpr uint64_t DormantSoundOnClient             = 0xD0;
		constexpr uint64_t AntiCheatCharacterSyncManagerEncryption        = 0x48;
		constexpr uint64_t AntiCheatCharacterSyncManagerEncryption_offset = 0xCC;
		constexpr uint64_t AntiCheatCharacterSyncManagerShift            = 0x68;

		// ── Static offsets ──
		constexpr uint64_t ActorsForGC                      = 0x308;
		constexpr uint64_t GameState                        = 0x278;
		constexpr uint64_t PlayerController                 = 0x38;
		constexpr uint64_t ViewTarget                       = 0x1050;
		constexpr uint64_t CameraCacheLocation              = 0xA30;
		constexpr uint64_t CameraCacheRotation              = 0xA10;
		constexpr uint64_t CameraCacheFOV                   = 0xA2C;
		constexpr uint64_t TeamNumber                       = 0x7C4;
		constexpr uint64_t MyHUD                            = 0x4C8;
		constexpr uint64_t BlockInputWidgetList             = 0x5B0;
		constexpr uint64_t bShowMouseCursor                 = 0x658;
		constexpr uint64_t ComponentLocation                = 0x330;
		constexpr uint64_t CharacterState                   = 0x10A0;
		constexpr uint64_t CharacterMovement                = 0x490;
		constexpr uint64_t WorldToMap                       = 0x4A8;
		constexpr uint64_t LayoutData                       = 0x40;
		constexpr uint64_t Offsets                          = 0x0;
		constexpr uint64_t Alignment                        = 0x20;
		constexpr uint64_t Visibility                       = 0xA9;
		constexpr uint64_t SelectMinimapSizeIndex           = 0x5C8;
		constexpr uint64_t Slot                             = 0x38;
		constexpr uint64_t WidgetStateMap                   = 0x538;
		constexpr uint64_t FeatureRepObject                 = 0xCF0;
		constexpr uint64_t SafetyZonePosition               = 0xB0;
		constexpr uint64_t SafetyZoneRadius                 = 0xBC;
		constexpr uint64_t BlueZoneRadius                   = 0xCC;
		constexpr uint64_t BlueZonePosition                 = 0xC0;
		constexpr uint64_t NumAliveTeams                    = 0x480;
		constexpr uint64_t HeaFlag                          = 0x3B9;
		constexpr uint64_t Health1                          = 0xA3C;
		constexpr uint64_t Health2                          = 0xA38;
		constexpr uint64_t Health3                          = 0xA24;
		constexpr uint64_t Health4                          = 0xA10;
		constexpr uint64_t Health5                          = 0xA25;
		constexpr uint64_t Health6                          = 0xA20;
		constexpr uint64_t PlayerArray                      = 0x410;
		constexpr uint64_t AccountId                        = 0x810;
		constexpr uint64_t PlayerName                       = 0x420;
		constexpr uint64_t PlayerStatusType                 = 0x468;
		constexpr uint64_t SquadMemberIndex                 = 0xA1C;
		constexpr uint64_t PlayerState                      = 0x418;
		constexpr uint64_t PlayerStatistics                 = 0xA10;
		constexpr uint64_t DamageDealtOnEnemy               = 0x804;
		constexpr uint64_t SpectatedCount                   = 0x11BC;
		constexpr uint64_t ping                             = 0x3F8;
		constexpr uint64_t MatchId                          = 0x518;
		constexpr uint64_t PartnerLevel                     = 0x716;
		constexpr uint64_t SurvivalTier                     = 0xCC0;
		constexpr uint64_t SurvivalLevel                    = 0xCC4;
		constexpr uint64_t PubgIdData                       = 0xCB8;
		constexpr uint64_t CharacterClanInfo                = 0x838;
		constexpr uint64_t CurrentWeaponIndex               = 0x319;
		constexpr uint64_t WeaponTrajectoryData             = 0x11A8;
		constexpr uint64_t TrajectoryGravityZ               = 0x10EC;
		constexpr uint64_t FiringAttachPoint                = 0x8C0;
		constexpr uint64_t ScopingAttachPoint               = 0xB28;
		constexpr uint64_t TrajectoryConfig                 = 0x108;
		constexpr uint64_t BallisticCurve                   = 0x28;
		constexpr uint64_t FloatCurves                      = 0x38;
		constexpr uint64_t Mesh3P                           = 0x800;
		constexpr uint64_t Keys                             = 0x60;
		constexpr uint64_t AttachedStaticComponentMap       = 0x1508;
		constexpr uint64_t WeaponConfig_WeaponClass         = 0x798;
		constexpr uint64_t ElapsedCookingTime               = 0xB50;
		constexpr uint64_t PlayerInput                      = 0x548;
		constexpr uint64_t InputAxisProperties              = 0x138;
		constexpr uint64_t LastUpdateVelocity               = 0x3E0;
		constexpr uint64_t RootComponent                    = 0x308;
		constexpr uint64_t StaticMesh                       = 0xAE8;
		constexpr uint64_t Eyes                             = 0xBD8;
		constexpr uint64_t bAlwaysCreatePhysicsState        = 0x488;
		constexpr uint64_t VehicleMovement                  = 0x468;
		constexpr uint64_t VehicleRiderComponent            = 0x20E0;
		constexpr uint64_t ReplicatedMovement               = 0xD0;
		constexpr uint64_t LastVehiclePawn                  = 0x270;
		constexpr uint64_t SeatIndex                        = 0x230;
		constexpr uint64_t Wheels                           = 0x328;
		constexpr uint64_t WheelLocation                    = 0x100;
		constexpr uint64_t DampingRate                      = 0x54;
		constexpr uint64_t ShapeRadius                      = 0x48;
		constexpr uint64_t DroppedItemGroup                 = 0x1C0;
		constexpr uint64_t ItemPackageItems                 = 0x578;
		constexpr uint64_t DroppedItemGroupUItem            = 0x870;
		constexpr uint64_t AttachedItems                    = 0x858;
		constexpr uint64_t WeaponAttachmentData             = 0x128;
		constexpr uint64_t ItemTable                        = 0xB0;
		constexpr uint64_t ItemID                           = 0x244;
		constexpr uint64_t DroppedItem                      = 0x458;
		constexpr uint64_t PreEvalPawnState                 = 0x300;
		constexpr uint64_t bIsInVehicle_CP                  = 0x954;
		constexpr uint64_t bIsParachuting_CP                = 0x92E;
		constexpr uint64_t bIsFreefalling_CP                = 0x92D;
		constexpr uint64_t bEmergencyPickup_Flying_CP       = 0x92F;
		constexpr uint64_t bIsReviving_CP                   = 0x932;
		constexpr uint64_t bIsSwimming_CP                   = 0x935;
		constexpr uint64_t bIsScoping_CP                    = 0x85D;
		constexpr uint64_t bIsReloading_CP                  = 0x73D;
		constexpr uint64_t ControlRotation_CP               = 0x654;
		constexpr uint64_t CurrentAmmoData                  = 0xE68;
		constexpr uint64_t StaticSockets                    = 0xC8;
		constexpr uint64_t StaticSocketName                 = 0x30;
		constexpr uint64_t StaticRelativeScale              = 0x50;
		constexpr uint64_t StaticRelativeLocation           = 0x38;
		constexpr uint64_t StaticRelativeRotation           = 0x44;
		constexpr uint64_t InputYawScale                    = 0x64C;
		constexpr uint64_t AimOffsets                       = 0x1B48;
		constexpr uint64_t TimeSeconds                      = 0x810;
		constexpr uint64_t TimeTillExplosion                = 0x824;
		constexpr uint64_t ExplodeState                     = 0x628;
		constexpr uint64_t TrainingMapGrid                  = 0x5B0;
		constexpr uint64_t RecentlyRendered                 = 0xBD8;
		constexpr uint64_t MortarRotation                   = 0x520;
		constexpr uint64_t MortarLocation                   = 0x300;
		constexpr uint64_t MortarEntity                     = 0x108;
		constexpr uint64_t MapGrid_Map                      = 0x4A8;
		constexpr uint64_t Gender                           = 0xB48;
		constexpr uint64_t MouseX                           = 0x4C8F;
		constexpr uint64_t MouseY                           = 0x4C90;
		constexpr uint64_t PhysicsScene                     = 0x3A0;
		constexpr uint64_t mPhysXScene                      = 0xD0;
		constexpr uint64_t rigid_dynamics                   = 0x3B98;
		constexpr uint64_t CurrentMinimapViewScale1D        = 0x4A4;
		constexpr uint64_t LastMinimapPos                   = 0x4B8;
		constexpr uint64_t Minimap                          = 0x480;
		constexpr uint64_t RecoilValueVector                = 0x1198;
		constexpr uint64_t VerticalRecovery                 = 0x10D8;
		constexpr uint64_t ControlRotation                  = 0x654;
		constexpr uint64_t Character                        = 0x438;
		constexpr uint64_t Pawn                             = 0x448;
		constexpr uint64_t SpectatorPawn                    = 0x760;
		constexpr uint64_t DefaultFOV                       = 0x828;
		constexpr uint64_t CurrentMinimapViewScale          = 0x4A4;
		constexpr uint64_t ScreenPosX                       = 0x4B8;
		constexpr uint64_t ScreenPosY                       = 0x4BC;
		constexpr uint64_t ScreenSize                       = 0x4A8;
		constexpr uint64_t BoneArray                        = 0x10;
		constexpr uint64_t BoneCount                        = 0xAF0;
		constexpr uint64_t InventoryFacade                  = 0x17A8;
		constexpr uint64_t Inventory                        = 0x400;
		constexpr uint64_t InventoryItems                   = 0x690;
		constexpr uint64_t InventoryItemCount               = 0x698;
		constexpr uint64_t InventoryItemTagItemCount        = 0x40;
		constexpr uint64_t Equipment                        = 0x408;
		constexpr uint64_t ItemsArray                       = 0x4F8;
		constexpr uint64_t Durability                       = 0x1E4;
		constexpr uint64_t Durabilitymax                    = 0x1E0;
		constexpr uint64_t VehicleCommonComponent           = 0xB30;
		constexpr uint64_t FloatingVehicleCommonComponent   = 0x4D8;
		constexpr uint64_t FloatingComponent                = 0x4D8;
		constexpr uint64_t VehicleFuel                      = 0x2E0;
		constexpr uint64_t VehicleFuelMax                   = 0x2E4;
		constexpr uint64_t VehicleHealth                    = 0x2D8;
		constexpr uint64_t VehicleHealthMax                 = 0x2DC;

		// ── Engine globals ──
		constexpr uint64_t PhysxSDK                         = 0x125C59C0;

		// ── FName / decrypt constants ──
		constexpr uint64_t ObjID                            = 0x20;
		constexpr uint64_t DecryptNameIndexRor              = 0x1;
		constexpr uint64_t DecryptNameIndexRval             = 0x17;
		constexpr uint64_t DecryptNameIndexSval             = 0x7;
		constexpr uint64_t DecryptNameIndexXorKey1          = 0x7360F24;
		constexpr uint64_t DecryptNameIndexXorKey2          = 0xB621EC05;
		constexpr uint64_t DecryptNameIndexXorKey3          = 0x1FF0000;
		constexpr uint64_t DecryptNameIndexDval             = 0x19;

		// ── Health keys ──
		constexpr uint64_t Health_keys0  = 0xCEC7A591;
		constexpr uint64_t Health_keys1  = 0x9B63B27E;
		constexpr uint64_t Health_keys2  = 0xCA7F1AA5;
		constexpr uint64_t Health_keys3  = 0x7E3848E7;
		constexpr uint64_t Health_keys4  = 0x60911D0A;
		constexpr uint64_t Health_keys5  = 0x23DDA35C;
		constexpr uint64_t Health_keys6  = 0x945E7C8;
		constexpr uint64_t Health_keys7  = 0xA521B821;
		constexpr uint64_t Health_keys8  = 0xBA37A58;
		constexpr uint64_t Health_keys9  = 0xB0EF7E87;
		constexpr uint64_t Health_keys10 = 0xE2756BB8;
		constexpr uint64_t Health_keys11 = 0x878ADB7C;
		constexpr uint64_t Health_keys12 = 0xBD60BBD5;
		constexpr uint64_t Health_keys13 = 0x7E93E707;
		constexpr uint64_t Health_keys14 = 0xE7099E38;
		constexpr uint64_t Health_keys15 = 0xECD92AA4;
	}

	// =========================================================================
	//  用硬编码常量填充 GameData.Offset,保持原有 map 访问方式不变
	//  原 Sever_Init 经 http://localhost/offset.txt 拉取并文本解析,现已去除网络依赖
	//  并重命名为 Init()。
	// =========================================================================
	inline void Init() {
		// ── Health 派生项 ──
		GameData.Offset["Health"]                = C::Health4;
		GameData.Offset["bEncryptedHealth"]      = C::Health4 + 0x15;
		GameData.Offset["EncryptedHealthOffset"] = C::Health4 + 0x14;
		GameData.Offset["DecryptedHealthOffset"] = C::Health4 + 0x10;

		// ── 载具 ──
		GameData.Offset["VehicleFuel"]             = C::VehicleFuel;
		GameData.Offset["VehicleCommonComponent"]  = C::VehicleCommonComponent;
		GameData.Offset["FloatingComponent"]       = C::FloatingComponent;
		GameData.Offset["VehicleFuelMax"]          = C::VehicleFuelMax;
		GameData.Offset["VehicleHealth"]           = C::VehicleHealth;
		GameData.Offset["VehicleHealthMax"]        = C::VehicleHealthMax;

		// ── 背包/物品 ──
		GameData.Offset["InventoryFacade"]               = C::InventoryFacade;
		GameData.Offset["Inventory"]                     = C::Inventory;
		GameData.Offset["InventoryItems"]                = C::InventoryItems;
		GameData.Offset["InventoryItemCount"]            = C::InventoryItemCount;
		GameData.Offset["InventoryItemTagItemCount"]     = C::InventoryItemTagItemCount;

		// ── Health 字段与解密 key ──
		GameData.Offset["HeaFlag"] = C::HeaFlag;
		GameData.Offset["Health1"] = C::Health1;
		GameData.Offset["Health2"] = C::Health2;
		GameData.Offset["Health3"] = C::Health3;
		GameData.Offset["Health4"] = C::Health4;
		GameData.Offset["Health5"] = C::Health5;
		GameData.Offset["Health6"] = C::Health6;
		GameData.Offset["DecryptedHealthOffsets0"]  = C::Health_keys0;
		GameData.Offset["DecryptedHealthOffsets1"]  = C::Health_keys1;
		GameData.Offset["DecryptedHealthOffsets2"]  = C::Health_keys2;
		GameData.Offset["DecryptedHealthOffsets3"]  = C::Health_keys3;
		GameData.Offset["DecryptedHealthOffsets4"]  = C::Health_keys4;
		GameData.Offset["DecryptedHealthOffsets5"]  = C::Health_keys5;
		GameData.Offset["DecryptedHealthOffsets6"]  = C::Health_keys6;
		GameData.Offset["DecryptedHealthOffsets7"]  = C::Health_keys7;
		GameData.Offset["DecryptedHealthOffsets8"]  = C::Health_keys8;
		GameData.Offset["DecryptedHealthOffsets9"]  = C::Health_keys9;
		GameData.Offset["DecryptedHealthOffsets10"] = C::Health_keys10;
		GameData.Offset["DecryptedHealthOffsets11"] = C::Health_keys11;
		GameData.Offset["DecryptedHealthOffsets12"] = C::Health_keys12;
		GameData.Offset["DecryptedHealthOffsets13"] = C::Health_keys13;
		GameData.Offset["DecryptedHealthOffsets14"] = C::Health_keys14;
		GameData.Offset["DecryptedHealthOffsets15"] = C::Health_keys15;

		// ── 输入/渲染杂项 ──
		GameData.Offset["MouseX"]                    = C::MouseX;
		GameData.Offset["MouseY"]                    = C::MouseY;
		GameData.Offset["bAlwaysCreatePhysicsState"] = C::bAlwaysCreatePhysicsState;

		// ── 动态全局 / FName 解密 ──
		GameData.Offset["XenuineDecrypt"]            = C::XenuineDecrypt;
		GameData.Offset["UWorld"]                    = C::UWorld;
		GameData.Offset["GNames"]                    = C::GNames;
		GameData.Offset["GNamesPtr"]                 = C::GNamesPtr;
		GameData.Offset["ChunkSize"]                 = C::ChunkSize;
		GameData.Offset["DecryptNameIndexRor"]       = C::DecryptNameIndexRor;
		GameData.Offset["DecryptNameIndexXorKey1"]   = C::DecryptNameIndexXorKey1;
		GameData.Offset["DecryptNameIndexXorKey2"]   = C::DecryptNameIndexXorKey2;
		GameData.Offset["DecryptNameIndexRval"]      = C::DecryptNameIndexRval;
		GameData.Offset["DecryptNameIndexSval"]      = C::DecryptNameIndexSval;
		GameData.Offset["DecryptNameIndexDval"]      = C::DecryptNameIndexDval;
		GameData.Offset["DecryptNameIndexXorKey3"]   = C::DecryptNameIndexXorKey3;
		GameData.Offset["ObjID"]                     = C::ObjID;

		// ── 结构偏移 ──
		GameData.Offset["GameInstance"]          = C::GameInstance;
		GameData.Offset["LocalPlayer"]           = C::LocalPlayer;
		GameData.Offset["Actors"]                = C::Actors;
		GameData.Offset["NumAliveTeams"]         = C::NumAliveTeams;
		GameData.Offset["ComponentToWorld"]      = C::ComponentToWorld;
		GameData.Offset["ComponentLocation"]     = C::ComponentLocation;
		GameData.Offset["TimeSeconds"]           = C::TimeSeconds;
		GameData.Offset["WorldToMap"]            = C::MapGrid_Map;
		GameData.Offset["CurrentLevel"]          = C::CurrentLevel;
		GameData.Offset["Mesh"]                  = C::Mesh;
		GameData.Offset["CharacterMovement"]     = C::CharacterMovement;
		GameData.Offset["LastUpdateVelocity"]    = C::LastUpdateVelocity;
		GameData.Offset["RootComponent"]         = C::RootComponent;
		GameData.Offset["PlayerController"]      = C::PlayerController;
		GameData.Offset["PlayerInput"]           = C::PlayerInput;
		GameData.Offset["InputYawScale"]         = C::InputYawScale;
		GameData.Offset["bShowMouseCursor"]      = C::bShowMouseCursor;
		GameData.Offset["AcknowledgedPawn"]      = C::AcknowledgedPawn;
		GameData.Offset["MyHUD"]                 = C::MyHUD;
		GameData.Offset["InputAxisProperties"]   = C::InputAxisProperties;
		GameData.Offset["GameState"]             = C::GameState;
		GameData.Offset["SafetyZoneRadius"]      = C::SafetyZoneRadius;
		GameData.Offset["SafetyZonePosition"]    = C::SafetyZonePosition;
		GameData.Offset["BlueZoneRadius"]        = C::BlueZoneRadius;
		GameData.Offset["BlueZonePosition"]      = C::BlueZonePosition;
		GameData.Offset["PlayerArray"]           = C::PlayerArray;
		GameData.Offset["PlayerCameraManager"]   = C::PlayerCameraManager;
		GameData.Offset["ViewTarget"]            = C::ViewTarget;
		GameData.Offset["CameraCacheFOV"]        = C::CameraCacheFOV;
		GameData.Offset["CameraCacheRotation"]   = C::CameraCacheRotation;
		GameData.Offset["CameraCacheLocation"]   = C::CameraCacheLocation;
		GameData.Offset["GroggyHealth"]          = C::GroggyHealth;
		GameData.Offset["CharacterName"]         = C::CharacterName;
		GameData.Offset["LastTeamNum"]           = C::LastTeamNum;
		GameData.Offset["SpectatedCount"]        = C::SpectatedCount;
		GameData.Offset["PlayerState"]           = C::PlayerState;
		GameData.Offset["CharacterState"]        = C::CharacterState;
		GameData.Offset["Gender"]                = C::Gender;
		GameData.Offset["AnimScriptInstance"]    = C::AnimScriptInstance;
		GameData.Offset["PreEvalPawnState"]      = C::PreEvalPawnState;
		GameData.Offset["StaticMesh"]            = C::StaticMesh;
		GameData.Offset["Eyes"]                  = C::Eyes;
		GameData.Offset["AimOffsets"]            = C::AimOffsets;
		GameData.Offset["PlayerStatistics"]      = C::PlayerStatistics;
		GameData.Offset["DamageDealtOnEnemy"]    = C::DamageDealtOnEnemy;
		GameData.Offset["PartnerLevel"]          = C::PartnerLevel;
		GameData.Offset["TeamNumber"]            = C::TeamNumber;
		GameData.Offset["PlayerName"]            = C::PlayerName;
		GameData.Offset["PlayerStatusType"]      = C::PlayerStatusType;
		GameData.Offset["SquadMemberIndex"]      = C::SquadMemberIndex;
		GameData.Offset["AccountId"]             = C::AccountId;
		GameData.Offset["SurvivalTier"]          = C::SurvivalTier;
		GameData.Offset["SurvivalLevel"]         = C::SurvivalLevel;
		GameData.Offset["CharacterClanInfo"]     = C::CharacterClanInfo;
		GameData.Offset["VehicleRiderComponent"] = C::VehicleRiderComponent;
		GameData.Offset["ReplicatedMovement"]    = C::ReplicatedMovement;
		GameData.Offset["SeatIndex"]             = C::SeatIndex;
		GameData.Offset["LastVehiclePawn"]       = C::LastVehiclePawn;
		GameData.Offset["VehicleMovement"]       = C::VehicleMovement;
		GameData.Offset["Wheels"]                = C::Wheels;
		GameData.Offset["WheelLocation"]         = C::WheelLocation;
		GameData.Offset["DampingRate"]           = C::DampingRate;
		GameData.Offset["ShapeRadius"]           = C::ShapeRadius;
		GameData.Offset["WidgetStateMap"]        = C::WidgetStateMap;
		GameData.Offset["TrainingMapGrid"]       = C::TrainingMapGrid;
		GameData.Offset["MapGrid_Map"]           = C::MapGrid_Map;
		GameData.Offset["LayoutData"]            = C::LayoutData;
		GameData.Offset["Visibility"]            = C::Visibility;
		GameData.Offset["Slot"]                  = C::Slot;
		GameData.Offset["Offsets"]               = C::Offsets;
		GameData.Offset["Alignment"]             = C::Alignment;
		GameData.Offset["BlockInputWidgetList"]  = C::BlockInputWidgetList;
		GameData.Offset["SelectMinimapSizeIndex"]= C::SelectMinimapSizeIndex;
		GameData.Offset["ItemID"]                = C::ItemID;
		GameData.Offset["ItemTable"]             = C::ItemTable;
		GameData.Offset["ItemPackageItems"]      = C::ItemPackageItems;
		GameData.Offset["DroppedItemGroup"]      = C::DroppedItemGroup;
		GameData.Offset["DroppedItem"]           = C::DroppedItem;
		GameData.Offset["DroppedItemGroupUItem"] = C::DroppedItemGroupUItem;
		GameData.Offset["TimeTillExplosion"]     = C::TimeTillExplosion;
		GameData.Offset["ExplodeState"]          = C::ExplodeState;
		GameData.Offset["WeaponProcessor"]       = C::WeaponProcessor;
		GameData.Offset["CurrentAmmoData"]       = C::CurrentAmmoData;
		GameData.Offset["CurrentWeaponIndex"]    = C::CurrentWeaponIndex;
		GameData.Offset["EquippedWeapons"]       = C::EquippedWeapons;
		GameData.Offset["WeaponTrajectoryData"]  = C::WeaponTrajectoryData;
		GameData.Offset["TrajectoryGravityZ"]    = C::TrajectoryGravityZ;
		GameData.Offset["TrajectoryConfig"]      = C::TrajectoryConfig;
		GameData.Offset["BallisticCurve"]        = C::BallisticCurve;
		GameData.Offset["FloatCurves"]           = C::FloatCurves;
		GameData.Offset["Keys"]                  = C::Keys;
		GameData.Offset["WeaponConfig_WeaponClass"] = C::WeaponConfig_WeaponClass;
		GameData.Offset["Mesh3P"]                = C::Mesh3P;
		GameData.Offset["FiringAttachPoint"]     = C::FiringAttachPoint;
		GameData.Offset["AttachedStaticComponentMap"] = C::AttachedStaticComponentMap;
		GameData.Offset["AttachedItems"]         = C::AttachedItems;
		GameData.Offset["WeaponAttachmentData"]  = C::WeaponAttachmentData;
		GameData.Offset["ScopingAttachPoint"]    = C::ScopingAttachPoint;
		GameData.Offset["ElapsedCookingTime"]    = C::ElapsedCookingTime;
		GameData.Offset["ControlRotation_CP"]    = C::ControlRotation_CP;
		GameData.Offset["RecoilADSRotation_CP"]  = C::RecoilADSRotation_CP;
		GameData.Offset["LeanLeftAlpha_CP"]      = C::LeanLeftAlpha_CP;
		GameData.Offset["LeanRightAlpha_CP"]     = C::LeanRightAlpha_CP;
		GameData.Offset["bIsScoping_CP"]         = C::bIsScoping_CP;
		GameData.Offset["bIsReloading_CP"]       = C::bIsReloading_CP;
		GameData.Offset["StaticSockets"]         = C::StaticSockets;
		GameData.Offset["StaticSocketName"]      = C::StaticSocketName;
		GameData.Offset["StaticRelativeLocation"]= C::StaticRelativeLocation;
		GameData.Offset["StaticRelativeRotation"]= C::StaticRelativeRotation;
		GameData.Offset["StaticRelativeScale"]   = C::StaticRelativeScale;
		GameData.Offset["AntiCheatCharacterSyncManager"] = C::AntiCheatCharacterSyncManager;

		return;
	}
}
